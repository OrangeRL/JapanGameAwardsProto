#include "GameScene.h"
#include <cassert>

GameScene::GameScene() {

}

GameScene::~GameScene() {
	delete rhythm;
	for (int i = 0; i < 10; i++) {
		delete num_[i];
	}
	delete player;
	delete playerBullet;
	delete skydome;
	delete particle;
	delete particle2;
	delete reilCamera;
}

void GameScene::Initialize(WinApp* winApp)
{
	// デバッグテキスト用テクスチャ読み込み
	Sprite::LoadTexture(10, L"Resources/debugfont.png");
	// デバッグテキスト初期化
	debugText.Initialize(10);



	//透視投影変換行列の計算
	matProjection_ = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0),
		(float)winApp->window_width / winApp->window_height,
		0.1f, 1000.0f
	);

	num_[0]->LoadTexture(0, L"Resources/0.png");
	num_[1]->LoadTexture(1, L"Resources/1.png");
	num_[2]->LoadTexture(2, L"Resources/2.png");
	num_[3]->LoadTexture(3, L"Resources/3.png");
	num_[4]->LoadTexture(4, L"Resources/4.png");
	num_[5]->LoadTexture(5, L"Resources/5.png");
	num_[6]->LoadTexture(6, L"Resources/6.png");
	num_[7]->LoadTexture(7, L"Resources/7.png");
	num_[8]->LoadTexture(8, L"Resources/8.png");
	num_[9]->LoadTexture(9, L"Resources/9.png");

	for (int i = 0; i < 10; i++) {
		num_[i] = new Sprite(i, { 0,0 }, { 64,64 }, { 1.0f,1.0f,1.0f,1.0f }, { 0,0 }, 0, 0);
		num_[i]->Initialize();
	}

	viewProjection_.Initialize();

	//XAudioエンジンのインスタンスを生成
	soundManager_.Initialize();

	skydome = new GameObject3D();
	skydome->PreLoadModel("Resources/skydome/skydome.obj");
	skydome->PreLoadTexture(L"Resources/skydome/Fine_Basin.jpg");
	skydome->SetViewProjection(&viewProjection_);
	skydome->SetMatProjection(&matProjection_);
	skydome->Initialize();
	skydome->worldTransform.scale = { 1000.0f,1000.0f,1000.0f };

	//レールカメラ
	reilCamera = new ReilCamera();
	reilCamera->Initialize({ 0,0,-50 }, { 0,0,0 });

	player = new Player();
	player->Initialize(&viewProjection_, &matProjection_);
	player->SetPos({ 0.0f, 0.0f, 20.0f });

	viewProjection_ = reilCamera->GetViewProjection();

	//player->SetMap(map);
	//player->SetGoal(goal);

	particle = new Particle;
	particle->Initialize(&viewProjection_, &matProjection_, player);

	particle2 = new Particle;
	particle2->Initialize(&viewProjection_, &matProjection_, player);

	loadEnemyPopData();

	rhythm = new Rhythm();
	rhythm->Initialize();
}

void GameScene::Update()
{
	viewProjection_ = reilCamera->GetViewProjection();

	viewProjection_.UpdateView();
	if (input_.PushKey(DIK_P)) {
		//player->OnCollision();
	}

	player->Update(reilCamera->GetWorldTransform());
	reilCamera->Update(&input_);

	particle->Update();
	particle2->Update2();

	skydome->Update();

	//敵の更新処理
	for (std::unique_ptr<Enemy>& enemy : enemys1) {
		enemy->Update(&viewProjection_, &matProjection_, 0);
#pragma region makeEnemyBullet
		if (enemy->GetAttackSpeed() <= 0.0f && player->GetPos().z <= enemy->GetWorldTransform().translation.z) {
			//弾を生成
			std::unique_ptr<EnemyBullet> bullet = std::make_unique<EnemyBullet>();
			//初期化
			bullet->Initialize(&viewProjection_, &matProjection_, L"Resources/white1x1.png", player->GetPos(), enemy->GetWorldTransform().translation);
			bullet->SetTransform(enemy->GetWorldTransform().translation);
			//使う弾の設定
			bullet->SetBullet(0);
			bullets1.push_back(std::move(bullet));
			//攻撃頻度の設定 1(速い)~ >1(遅い)
			enemy->SetAttackSpeed(150.0f);

			if (enemy->GetIsAttack() == false) {
				enemy->SetIsAttack(true);
			}
		}
		if (enemy->GetIsAttack() == true) {

			for (std::unique_ptr<EnemyBullet>& bullet : bullets1) {
				bullet->Update();
			}
		}

		//弾&敵を削除する
		bullets1.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->IsDead(); });
		enemyPos = enemy->GetWorldTransform().translation;
#pragma endregion
		
	}
	for (std::unique_ptr<Enemy>& enemy : enemys2) {
		enemy->Update(&viewProjection_, &matProjection_, 1);
#pragma region makeEnemyBullet
		if (enemy->GetAttackSpeed() <= 0.0f && enemy->GetCoolDown() == false) {
			//弾を生成
			std::unique_ptr<EnemyBullet> bullet = std::make_unique<EnemyBullet>();
			//初期化
			bullet->Initialize(&viewProjection_, &matProjection_, L"Resources/white1x1.png", player->GetPos(), enemy->GetWorldTransform().translation);
			bullet->SetTransform(enemy->GetWorldTransform().translation);
			//使う弾の設定
			bullet->SetBullet(1);
			bullets2.push_back(std::move(bullet));
			//攻撃頻度の設定 1(速い)~ >1(遅い)
			enemy->SetAttackSpeed(15.0f);
			if (enemy->GetIsAttack() == false) {
				enemy->SetIsAttack(true);
			}
		}

		if (enemy->GetIsAttack() == true) {
			for (std::unique_ptr<EnemyBullet>& bullet : bullets2) {
				bullet->Update();
			}
		}
		//弾を削除する
		bullets2.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {return bullet->IsDead(); });
#pragma endregion
	}
	//敵の削除
	enemys2.remove_if([](std::unique_ptr<Enemy>& enemy) {return enemy->IsDead(); });
	enemys1.remove_if([](std::unique_ptr<Enemy>& enemy) {return enemy->IsDead(); });

	UpdateEnemyPopCommand();

	if (input_.PushKey(DIK_R)) {
		Reset();
	}

	rhythm->Update(&input_);
	//プレイヤーの弾発射処理
	if (input_.TriggerKey(DIK_SPACE) && rhythm->GetSoundState().isFireSucces) {
		player->NewBullet(&viewProjection_, &matProjection_, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f });
		player->NewBullet(&viewProjection_, &matProjection_, enemyPos, player->GetWorldTransform().translation);
	}


	Collision();
#pragma region DebugText
	debugText.Printf(0, 100, 1.0f, 18, " Q,E...offset:%f", rhythm->GetSoundState().offset);
	debugText.Printf(0, 140, 1.0f, 25, " Up,Dawn...BGMVolume:%f", rhythm->GetSoundState().BGMVolume);
	debugText.Printf(0, 160, 1.0f, 32, " Left,Right...guideSEVolume:%f", rhythm->GetSoundState().guideSEVolume);
	debugText.Printf(0, 120, 1.0f, 10, " Timer:%f", rhythm->GetSoundState().timer);
	debugText.Printf(0, 180, 1.0f, 17, " measureCount:%d", rhythm->GetSoundState().measureCount);
	debugText.Printf(0, 200, 1.0f, 9, " weapon:%d", rhythm->GetSoundState().weapon);

	debugText.Printf(0, 220, 1.0f, 27, " %f,%f,%f",
		player->GetWorldTransform().matWorld.m[3][0],
		player->GetWorldTransform().matWorld.m[3][1],
		player->GetWorldTransform().matWorld.m[3][2]);
#pragma endregion
}

void GameScene::Draw() {
	//3D描画
	//プレイヤー描画
	player->Draw();
	particle->Draw();
	particle2->Draw();
	//敵の描画
	for (std::unique_ptr<Enemy>& enemy : enemys1) {
		enemy->Draw();
	}
	for (std::unique_ptr<Enemy>& enemy : enemys2) {
		enemy->Draw();
	}
	for (std::unique_ptr<EnemyBullet>& bullet : bullets1) {
		bullet->Draw();
	}
	for (std::unique_ptr<EnemyBullet>& bullet : bullets2) {
		bullet->Draw();
	}


	skydome->Draw();

	//スプライト描画
	Sprite::PreDraw(dx12base_.GetCmdList().Get());


	// デバッグテキストの描画
	debugText.DrawAll(dx12base_.GetCmdList().Get());

	Sprite::PostDraw();

}

void GameScene::loadEnemyPopData()
{
	//ファイルを開く
	std::ifstream file;
	file.open("Resources/enemy.csv");
	assert(file.is_open());
	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommand << file.rdbuf();
	//ファイルを閉じる
	file.close();
}

void GameScene::UpdateEnemyPopCommand()
{
	if (waitFlag) {
		waitTime_--;
		if (waitTime_ <= 0.0f) {
			//待機完了
			waitFlag = false;
		}
		return;
	}
	//1行分の文字列を入れる
	std::string line;
	//コマンド実行
	while (std::getline(enemyPopCommand, line)) {
		//1行分の文字列をストリームに変換
		std::istringstream line_stream(line);
		std::string world;
		// ,区切りで行の先頭文字列を取得
		std::getline(line_stream, world, ',');
		//"//"から始まる行はコメント
		if (world.find("//") == 0) {
			continue;
		}
		//POPコマンド
		if (world.find("Enemy1") == 0) {
			//CSVに書いてある値を変数に入れる
			//x座標
			std::getline(line_stream, world, ',');
			float x = (float)std::atof(world.c_str());
			//y座標
			std::getline(line_stream, world, ',');
			float y = (float)std::atof(world.c_str());
			//z座標
			std::getline(line_stream, world, ',');
			float z = (float)std::atof(world.c_str());
			//移動速度
			std::getline(line_stream, world, ',');
			float speedX = (float)std::atof(world.c_str());
			std::getline(line_stream, world, ',');
			float speedY = (float)std::atof(world.c_str());
			std::getline(line_stream, world, ',');
			float speedZ = (float)std::atof(world.c_str());
			//敵を発生させる
			//-------ここにEnemy発生関数---------//
			//複数化するためにuniq_ptrに変更
			std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
			newEnemy->Initialize(&viewProjection_, &matProjection_, L"Resources/red.png");
			//上で書いてある物をEnemyの座標としてセットする
			newEnemy->Settransform(x, y, z);
			newEnemy->SetSpeed(speedX, speedY, speedZ);
			//敵を登録
			enemys1.push_back(std::move(newEnemy));
		}
		if (world.find("Enemy2") == 0) {
			//CSVに書いてある値を変数に入れる
			//x座標
			std::getline(line_stream, world, ',');
			float x = (float)std::atof(world.c_str());
			//y座標
			std::getline(line_stream, world, ',');
			float y = (float)std::atof(world.c_str());
			//z座標
			std::getline(line_stream, world, ',');
			float z = (float)std::atof(world.c_str());
			//移動速度
			std::getline(line_stream, world, ',');
			float speedX = (float)std::atof(world.c_str());
			std::getline(line_stream, world, ',');
			float speedY = (float)std::atof(world.c_str());
			std::getline(line_stream, world, ',');
			float speedZ = (float)std::atof(world.c_str());
			//敵を発生させる
			//-------ここにEnemy発生関数---------//
			//複数化するためにuniq_ptrに変更
			std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
			newEnemy->Initialize(&viewProjection_, &matProjection_, L"Resources/red.png");
			//上で書いてある物をEnemyの座標としてセットする
			newEnemy->Settransform(x, y, z);
			newEnemy->SetSpeed(speedX, speedY, speedZ);
			//敵を登録
			enemys2.push_back(std::move(newEnemy));
		}
		//WAITコマンド
		else if (world.find("WAIT") == 0)
		{
			std::getline(line_stream, world, ',');
			//待ち時間
			int32_t waitTime = atoi(world.c_str());
			//待ち時間
			waitFlag = true;
			waitTime_ = waitTime;
			//コマンドループを抜ける
			break;
		}
	}
}

void GameScene::Reset() {

	player->Reset();
	particle->Reset();
	particle2->Reset();

}

void GameScene::Collision() {

#pragma region PlayerToEnemyCollision
	const std::list < std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullets();
		for (std::unique_ptr<Enemy>& enemy : enemys1) {
			//enemy-player
			if (enemy->GetWorldTransform().translation.x - player->GetPos().x < 2 &&
				-2 < enemy->GetWorldTransform().translation.x - player->GetPos().x) {
				if (enemy->GetWorldTransform().translation.y - player->GetPos().y < 2 &&
					-2 < enemy->GetWorldTransform().translation.y - player->GetPos().y) {
					if (enemy->GetWorldTransform().translation.z - player->GetPos().z < 2 &&
						-2 < enemy->GetWorldTransform().translation.z - player->GetPos().z) {

						player->OnCollision();

					}
				}
			}
#pragma endregion

#pragma region bulletToEnemyCollisions
			const std::list < std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullets();
			for (const std::unique_ptr<PlayerBullet>& bulletA : playerBullets) {
				if (enemy->GetWorldTransform().translation.x - bulletA->GetWorldTransform().translation.x < 2 &&
					-2 < enemy->GetWorldTransform().translation.x - bulletA->GetWorldTransform().translation.x) {
					if (enemy->GetWorldTransform().translation.y - bulletA->GetWorldTransform().translation.y < 2 &&
						-2 < enemy->GetWorldTransform().translation.y - bulletA->GetWorldTransform().translation.y) {
						if (enemy->GetWorldTransform().translation.z - bulletA->GetWorldTransform().translation.z < 2 &&
							-2 < enemy->GetWorldTransform().translation.z - bulletA->GetWorldTransform().translation.z) {

							bulletA->OnCollision();
							enemy->OnCollision();

						}
					}
				}
			}
		}

		//bullet-enemy2
		for (std::unique_ptr<Enemy>& enemy : enemys2) {
			for (const std::unique_ptr<PlayerBullet>& bulletA : playerBullets) {
				if (enemy->GetWorldTransform().translation.x - bulletA->GetWorldTransform().translation.x < 2 &&
					-2 < enemy->GetWorldTransform().translation.x - bulletA->GetWorldTransform().translation.x) {
					if (enemy->GetWorldTransform().translation.y - bulletA->GetWorldTransform().translation.y < 2 &&
						-2 < enemy->GetWorldTransform().translation.y - bulletA->GetWorldTransform().translation.y) {
						if (enemy->GetWorldTransform().translation.z - bulletA->GetWorldTransform().translation.z < 2 &&
							-2 < enemy->GetWorldTransform().translation.z - bulletA->GetWorldTransform().translation.z) {

							bulletA->OnCollision();
							enemy->OnCollision();
						}
					}
				}
			}
		}
#pragma endregion

#pragma region bulletToEnemyBulletCollisions
		//bullet-enemyBullet
		for (const std::unique_ptr<EnemyBullet>& bulletB : bullets1) {
			for (const std::unique_ptr<PlayerBullet>& bulletA : playerBullets) {
				if (input_.PushKey(DIK_P)) {
					//player->OnCollision();
					bulletA->OnCollision();
				}
				if (bulletB->GetWorldTransform().translation.x - bulletA->GetWorldTransform().translation.x < 2 &&
					-2 < bulletB->GetWorldTransform().translation.x - bulletA->GetWorldTransform().translation.x) {
					if (bulletB->GetWorldTransform().translation.y - bulletA->GetWorldTransform().translation.y < 2 &&
						-2 < bulletB->GetWorldTransform().translation.y - bulletA->GetWorldTransform().translation.y) {
						if (bulletB->GetWorldTransform().translation.z - bulletA->GetWorldTransform().translation.z < 2 &&
							-2 < bulletB->GetWorldTransform().translation.z - bulletA->GetWorldTransform().translation.z) {

							bulletA->OnCollision();
							bulletB->OnCollision();

						}
					}
				}
			}
		}

		//bullet-enemyBullet
		for (const std::unique_ptr<EnemyBullet>& bulletB : bullets2) {
			for (const std::unique_ptr<PlayerBullet>& bulletA : playerBullets) {
				if (input_.PushKey(DIK_P)) {
					//player->OnCollision();
					bulletA->OnCollision();
				}
				if (bulletB->GetWorldTransform().translation.x - bulletA->GetWorldTransform().translation.x < 2 &&
					-2 < bulletB->GetWorldTransform().translation.x - bulletA->GetWorldTransform().translation.x) {
					if (bulletB->GetWorldTransform().translation.y - bulletA->GetWorldTransform().translation.y < 2 &&
						-2 < bulletB->GetWorldTransform().translation.y - bulletA->GetWorldTransform().translation.y) {
						if (bulletB->GetWorldTransform().translation.z - bulletA->GetWorldTransform().translation.z < 2 &&
							-2 < bulletB->GetWorldTransform().translation.z - bulletA->GetWorldTransform().translation.z) {

							bulletA->OnCollision();
							bulletB->OnCollision();

						}
					}
				}
			}
		}
#pragma endregion

#pragma region enemyBulletToPlayerCollisions
		//player-enemybullet
		for (const std::unique_ptr<EnemyBullet>& bulletB : bullets1) {

			if (player->GetPos().x - bulletB->GetWorldTransform().translation.x < 2 &&
				-2 < player->GetPos().x - bulletB->GetWorldTransform().translation.x) {
				if (player->GetPos().y - bulletB->GetWorldTransform().translation.y < 2 &&
					-2 < player->GetPos().y - bulletB->GetWorldTransform().translation.y) {
					if (player->GetPos().z - bulletB->GetWorldTransform().translation.z < 2 &&
						-2 < player->GetPos().z - bulletB->GetWorldTransform().translation.z) {

						//bulletB->OnCollision();
						//enemy->Reset();
						player->OnCollision();
					}
				}
			}
		}
		for (const std::unique_ptr<EnemyBullet>& bulletC : bullets2) {
			if (player->GetPos().x - bulletC->GetWorldTransform().translation.x < 2 &&
				-2 < player->GetPos().x - bulletC->GetWorldTransform().translation.x) {
				if (player->GetPos().y - bulletC->GetWorldTransform().translation.y < 2 &&
					-2 < player->GetPos().y - bulletC->GetWorldTransform().translation.y) {
					if (player->GetPos().z - bulletC->GetWorldTransform().translation.z < 2 &&
						-2 < player->GetPos().z - bulletC->GetWorldTransform().translation.z) {

						//bulletB->OnCollision();
						//enemy->Reset();
						player->OnCollision();
					}
				}
			}
		}
#pragma endregion
}
