#include "GameScene.h"
#include <cassert>

GameScene::GameScene() {

}

GameScene::~GameScene() {
	delete rhythm;
	delete player;
	delete playerBullet;
	delete skydome;
	delete particle;
	delete particle2;
	delete reilCamera;
	//delete UIManager;

}

void GameScene::Initialize(WinApp* winApp)
{
	// デバッグテキスト用テクスチャ読み込み
	Sprite::LoadTexture(0, L"Resources/debugfont.png");
	// デバッグテキスト初期化
	debugText.Initialize(0);


	//透視投影変換行列の計算
	matProjection_ = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0),
		(float)winApp->window_width / winApp->window_height,
		0.1f, 1000.0f
	);

	// UI用テクスチャ読み込み
	Sprite::LoadTexture(1, L"Resources/gamefont.png");
	Sprite::LoadTexture(2, L"Resources/text.png");

	crosshair->LoadTexture(11, L"Resources/crosshair.png");
	crosshair = Sprite::Create(11, { 0,0 });

	for (int i = 0; i < 10; i++) {
		num_[i] = new Sprite(i, { 0,0 }, { 64,64 }, { 1.0f,1.0f,1.0f,0.5f }, { 0,0 }, 0, 0);
		num_[i]->Initialize();
	}
  
	//UI初期化
	UIManager.Initialize(1);


	viewProjection_.Initialize();

	//XAudioエンジンのインスタンスを生成
	//soundManager_ = new SoundManager();
	//soundManager_->Initialize();

	//天球
	skydome = new GameObject3D();
	skydome->PreLoadModel("Resources/skydome/skydome.obj");
	skydome->PreLoadTexture(L"Resources/skydome/skydome.png");
	skydome->SetViewProjection(&viewProjection_);
	skydome->SetMatProjection(&matProjection_);
	skydome->Initialize();
	skydome->worldTransform.scale = { 2000.0f,2000.0f,2000.0f };

	//レールカメラ
	reilCamera = new ReilCamera();
	reilCamera->Initialize({ 0,0,-50 }, { 0,0,0 });

	//アイテム
	//item = new Item();
	//item->Initialize(&viewProjection_, &matProjection_, L"Resources/white1x1.png", {0.0f,0.0f,50.0f},2);

	player = new Player();
	player->Initialize(&viewProjection_, &matProjection_);
	player->SetPos({ 0.0f, 0.0f, 20.0f });

	boss = new Boss();
	boss->Initialize(&viewProjection_, &matProjection_);
	boss2 = new Boss2();
	boss2->Initialize(&viewProjection_, &matProjection_);

	viewProjection_ = reilCamera->GetViewProjection();

	//player->SetMap(map);
	//player->SetGoal(goal);

	particle = new Particle;
	particle->Initialize(&viewProjection_, &matProjection_, player);

	particle2 = new Particle;
	particle2->Initialize(&viewProjection_, &matProjection_, player);

	particle3 = new Particle;
	particle3->Initialize(&viewProjection_, &matProjection_, player);

	loadEnemyPopData(2);
	//ボスの雑魚敵の配置
	loadBossPopData(1);

	rhythm = new Rhythm();
	rhythm->Initialize(&viewProjection_, &matProjection_);

}

void GameScene::Update()
{

	//ランダムな整数
	std::default_random_engine engine(seed_gen());

	viewProjection_ = reilCamera->GetViewProjection();

	viewProjection_.UpdateView();

	//UI更新
	UIManager.Update(rhythm, player->GetIsDead());

	if (rhythm->GetSoundState().isPause == 0) {


		player->Update(reilCamera->GetWorldTransform(), reilCamera->GetWorldTransform().rotation);

		if (player->GetIsDead() == false) {
			reilCamera->Update(&input_,rhythm->GetSoundState().wave);
		}

		particle->Update();
		particle2->Update2();

		skydome->Update();
		player->Aim(player->GetWorldTransform().translation, {0,0,100});
	
		crosshair->SetPosition({ player->GetWorldTransform().translation.x, player->GetWorldTransform().translation.y });
		crosshair->Sprite::SetSize({ 1,1 });


		//アイテムの更新処理
		for (std::unique_ptr<Item>& item : items_) {
			item->Update();
		}

		//デスフラグの立ったアイテムを削除
		items_.remove_if([](std::unique_ptr<Item>& item) {
			return item->GetIsDead();
			});

		//アイテム生成
		if (input_.TriggerKey(DIK_T)) {
			std::uniform_int_distribution<> dist(0, 4);
			int value = dist(engine);
			//アイテムを生成し、初期化
			std::unique_ptr<Item>item = std::make_unique<Item>();
			item->Initialize(&viewProjection_, &matProjection_, L"Resources/white1x1.png", { player->GetPos().x,player->GetPos().y,player->GetPos().z + 20.0f }, value);

			//アイテムを登録する
			items_.push_back(std::move(item));
		}

		//敵の更新処理
		for (std::unique_ptr<Enemy>& enemy : enemys1) {
			enemy->Update(&viewProjection_, &matProjection_, 0);
#pragma region makeEnemyBullet
			if (enemy->GetAttackSpeed() <= 0.0f && enemy->GetPhase() == Phase::move) {
				//弾を生成
				std::unique_ptr<EnemyBullet> bullet = std::make_unique<EnemyBullet>();
				//初期化
				bullet->Initialize(&viewProjection_, &matProjection_, L"Resources/white1x1.png", player->GetPos(), enemy->GetWorldTransform().translation);
				bullet->SetTransform(enemy->GetWorldTransform().translation);
				//使う弾の設定
				bullet->SetBullet(0);
				bullets1.push_back(std::move(bullet));
				//攻撃頻度の設定 1(速い)~ >1(遅い)
				enemy->SetAttackSpeed(100.0f);

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
#pragma endregion
		enemyPos = enemy->GetWorldTransform().translation;
#pragma endregion
	}
	//敵1の削除
	enemys1.remove_if([](std::unique_ptr<Enemy>& enemy) {return enemy->IsDead(); });

	for (std::unique_ptr<Enemy>& enemy : enemys2) {
		enemy->Update(&viewProjection_, &matProjection_, 1);
	}
	//敵の削除
	enemys2.remove_if([](std::unique_ptr<Enemy>& enemy) {return enemy->IsDead(); });

	for (std::unique_ptr<Enemy>& enemy : enemys3) {
		enemy->Update(&viewProjection_, &matProjection_, 2);
#pragma region makeEnemyBullet
		if (enemy->GetAttackSpeed() <= 0.0f && enemy->GetPhase() != Phase::spown) {
			//弾を生成
			std::unique_ptr<EnemyBullet> bullet = std::make_unique<EnemyBullet>();
			//初期化
			bullet->Initialize(&viewProjection_, &matProjection_, L"Resources/white1x1.png", player->GetPos(), enemy->GetWorldTransform().translation);
			bullet->SetScale({ 0.5f,0.5f,0.5f });
			bullet->SetTransform(enemy->GetWorldTransform().translation);
			//使う弾の設定
			bullet->SetBullet(1);
			bullets2.push_back(std::move(bullet));
			//攻撃頻度の設定 1(速い)~ >1(遅い)
			enemy->SetAttackSpeed(5.0f);

			if (enemy->GetIsAttack() == false) {
				enemy->SetIsAttack(true);
			}
		}
		if (enemy->GetIsAttack() == true) {

			for (std::unique_ptr<EnemyBullet>& bullet : bullets2) {
				bullet->Update();
			}
		}

		//弾&敵を削除する
		bullets2.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->IsDead(); });
#pragma endregion
	}
	//敵の削除
	enemys3.remove_if([](std::unique_ptr<Enemy>& enemy) {return enemy->IsDead(); });

	//ボス関連

	if (rhythm->GetSoundState().wave == 3) {
		boss2->Update(player->GetPos());
//		boss->Update();
//#pragma region made BossBullet
//		if (boss->GetIsDead() == false) {
//			if (boss->GetPhase() == BossPhase::attack && boss->GetAttackSpeed() <= 0.0f) {
//				//弾を生成
//				std::unique_ptr<BossBullet> bullet = std::make_unique<BossBullet>();
//				bullet->Initialize(&viewProjection_, &matProjection_, player->GetPos(), boss->GetWorldTransform().translation);
//				bullet->SetTransform(boss->GetWorldTransform().translation);
//				bossBullet1.push_back(std::move(bullet));
//				boss->SetAttackSpeed(50.0f);
//				if (boss->GetIsAttack() == false) {
//					boss->SetIsAttack(true);
//				}
//			}
//			if (boss->GetPhase() == BossPhase::attack2 && boss->GetAttackSpeed() <= 0.0f) {
//				//弾を生成
//				std::unique_ptr<BossBullet> bullet = std::make_unique<BossBullet>();
//				bullet->Initialize(&viewProjection_, &matProjection_, player->GetPos(), boss->GetWorldTransform().translation);
//				bullet->SetTransform(boss->GetWorldTransform().translation);
//				bossBullet2.push_back(std::move(bullet));
//				boss->SetAttackSpeed(150.0f);
//				if (boss->GetIsAttack() == false) {
//					boss->SetIsAttack(true);
//				}
//			}
//			
//			if (boss->GetIsAttack() == true) {
//				for (std::unique_ptr<BossBullet>& bullet : bossBullet1) {
//					bullet->Update(boss->GetPhase());
//				}
//				for (std::unique_ptr<BossBullet>& bullet : bossBullet2) {
//					bullet->Update(boss->GetPhase());
//				}
//			}
//			//弾&敵を削除する
//			bossBullet1.remove_if([](std::unique_ptr<BossBullet>& bullet) { return bullet->IsDead(); });
//			bossBullet2.remove_if([](std::unique_ptr<BossBullet>& bullet) { return bullet->IsDead(); });
//			
//		}
//#pragma endregion
//		if(boss->GetPhase() == BossPhase::defence) {
//			UpdateBossPopCommand();
//		}
	}
	if (player->GetIsDead() == false) {
		UpdateEnemyPopCommand();
	}
	


	if (player->GetIsDead() == false) {
		//enemy->Update(player->GetWorldTransform().translation, enemy->GetWorldTransform().translation);
	}

	if (player->GetIsDead() == false) {
		//enemy->Update(player->GetWorldTransform().translation, enemy->GetWorldTransform().translation);
	}

	if (input_.PushKey(DIK_R)) {
		Reset();
    
	}

	/*if (player->GetIsDead() == true && particle->GetIsDead() == true) {
		if (gameoverTimer <= 0) {
			gameoverTimer = 5;
		}
		else {
			gameoverTimer--;
			if (gameoverTimer <= 0) {
				stage = 1;
				Reset();
				scene_ = Scene::Title;
			}
		}
	}*/
  
}
	else {
		//debugText.Printf(window_width/2, window_height/2, 1.0f, 6, " PAUSE");
	}

	rhythm->Update(&input_, player->GetPos(), reilCamera->GetWorldTransform().rotation,player->GetIsDead(),stage);

	//プレイヤーの弾発射処理
	if (input_.TriggerKey(DIK_SPACE) && rhythm->GetSoundState().isFireSucces) {
		player->NewBullet(&viewProjection_, &matProjection_, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f },rhythm->GetSoundState().weapon);
	}

	//player->NewBulletAim(&viewProjection_, &matProjection_, enemyPos, player->GetWorldTransform().translation);
	

	Collisions();
#pragma region DebugText
	//Collision();

	debugText.Printf(0, 100, 1.0f, 18, " Q,E...offset:%f", rhythm->GetSoundState().offset);
	debugText.Printf(0, 140, 1.0f, 25, " Up,Dawn...BGMVolume:%f", rhythm->GetSoundState().BGMVolume);
	debugText.Printf(0, 160, 1.0f, 32, " Left,Right...guideSEVolume:%f", rhythm->GetSoundState().guideSEVolume);
	debugText.Printf(0, 120, 1.0f, 10, " Timer:%f", rhythm->GetSoundState().timer);
	debugText.Printf(0, 180, 1.0f, 17, " measureCount:%d", rhythm->GetSoundState().measureCount);
	debugText.Printf(0, 200, 1.0f, 9, " weapon:%d", rhythm->GetSoundState().weapon);
	debugText.Printf(0, 220, 1.0f, 7, " wave:%f", rhythm->GetSoundState().wave);
	debugText.Printf(0, 240, 1.0f, 11, " rotY:%f", reilCamera->GetRotation().x);

	debugText.Printf(0, 260, 1.0f, 27, " %f,%f,%f",
		player->GetWorldTransform().matWorld.m[3][0],
		player->GetWorldTransform().matWorld.m[3][1],
		player->GetWorldTransform().matWorld.m[3][2]);

	debugText.Printf(0, 280, 1.0f, 11, " Phase : %d", boss->GetPhase());
#pragma endregion
}

void GameScene::Draw() {
	//3D描画
	//プレイヤー描画
	player->Draw();

	rhythm->Draw(player->GetIsDead());
	particle->Draw();
	particle2->Draw();
	
	//敵の描画
	for (std::unique_ptr<Enemy>& enemy : enemys1) {
		enemy->Draw();
	}
	for (std::unique_ptr<Enemy>& enemy : enemys2) {
		enemy->Draw();
	}
	for (std::unique_ptr<Enemy>& enemy : enemys3) {
		enemy->Draw();
	}

	if (rhythm->GetSoundState().wave == 3) {
		boss2->Draw();
		/*boss->Draw();
		for (std::unique_ptr<BossBullet>& bullet : bossBullet1) {
			bullet->Draw();
		}

		for (std::unique_ptr<BossBullet>& bullet : bossBullet2) {
			bullet->Draw();
		}*/
	}

	for (std::unique_ptr<EnemyBullet>& bullet : bullets1) {
		bullet->Draw();
	}
	for (std::unique_ptr<EnemyBullet>& bullet : bullets2) {
		bullet->Draw();
	}

	//アイテム描画
	for (std::unique_ptr<Item>& item : items_) { item->Draw(); }
	skydome->Draw();

	//スプライト描画
	Sprite::PreDraw(dx12base_.GetCmdList().Get());


	crosshair->Draw();
	for (int i = 0; i < 10; i++) {
		num_[i]->Draw();
	}
  
	UIManager.Draw(rhythm);
  
	// デバッグテキストの描画
	debugText.DrawAll(dx12base_.GetCmdList().Get());

	Sprite::PostDraw();

}

void GameScene::loadEnemyPopData(int stageNum)
{

	if (stageNum == 1) {
		//ファイルを開く
		std::ifstream file;
		file.open("Resources/csv/enemy.csv");
		assert(file.is_open());
		//ファイルの内容を文字列ストリームにコピー
		enemyPopCommand << file.rdbuf();
		//ファイルを閉じる
		file.close();
	}
	
	if (stageNum == 2) {
		//ファイルを開く
		std::ifstream file;
		file.open("Resources/csv/enemy2.csv");
		assert(file.is_open());
		//ファイルの内容を文字列ストリームにコピー
		enemyPopCommand << file.rdbuf();
		//ファイルを閉じる
		file.close();
	}
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
		if (world.find("Enemy1") == 0) {	//固定砲台
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
			newEnemy->Initialize(&viewProjection_, &matProjection_, L"Resources/enemy/enemy1.png");
			//上で書いてある物をEnemyの座標としてセットする
			newEnemy->Settransform(x, y, z);
			newEnemy->SetSpeed(speedX, speedY, speedZ);
			//敵を登録
			enemys1.push_back(std::move(newEnemy));
		}
		if (world.find("Enemy2") == 0) {	//移動のみ
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
			newEnemy->Initialize(&viewProjection_, &matProjection_, L"Resources/enemy/enemy2.png");
			//上で書いてある物をEnemyの座標としてセットする
			newEnemy->Settransform(x, y, z);
			newEnemy->SetSpeed(speedX, speedY, speedZ);
			//敵を登録
			enemys2.push_back(std::move(newEnemy));
		}
		if (world.find("Enemy3") == 0) {	//移動も攻撃もしない
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
			newEnemy->Initialize(&viewProjection_, &matProjection_, L"Resources/red1x1.png");
			//上で書いてある物をEnemyの座標としてセットする
			newEnemy->Settransform(x, y, z);
			newEnemy->SetSpeed(speedX, speedY, speedZ);
			//敵を登録
			enemys3.push_back(std::move(newEnemy));
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

void GameScene::loadBossPopData(int stageNum)
{
	if (stageNum == 1) {
		//ファイルを開く
		std::ifstream file;
		file.open("Resources/csv/bossAttack.csv");
		assert(file.is_open());
		//ファイルの内容を文字列ストリームにコピー
		bossPopCommand << file.rdbuf();
		//ファイルを閉じる
		file.close();
	}
}

void GameScene::UpdateBossPopCommand()
{
	if (bossWaitFlag) {
		bossWaitTime_--;
		if (bossWaitTime_ <= 0.0f) {
			//待機完了
			bossWaitFlag = false;
		}
		return;
	}
	//1行分の文字列を入れる
	std::string line;
	//コマンド実行
	while (std::getline(bossPopCommand, line)) {
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
		if (world.find("Enemy1") == 0) {	//固定砲台
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
		//WAITコマンド
		else if (world.find("WAIT") == 0)
		{
			std::getline(line_stream, world, ',');
			//待ち時間
			int32_t waitTime = atoi(world.c_str());
			//待ち時間
			bossWaitFlag = true;
			bossWaitTime_ = waitTime;
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

void GameScene::Collisions() {

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
							bulletA->OnCollision();
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

#pragma region bossBulletToPlayerCollisions
		//player-enemybullet
		for (const std::unique_ptr<BossBullet>& bulletB : bossBullet1) {

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
		for (const std::unique_ptr<BossBullet>& bulletC : bossBullet2) {
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

		for (std::unique_ptr<Enemy>& enemy : enemys1) {
			if (enemy->GetWorldTransform().translation.x - player->GetAimPos().x < 2 &&
				-2 < enemy->GetWorldTransform().translation.x - player->GetAimPos().x) {
				if (enemy->GetWorldTransform().translation.y - player->GetAimPos().y < 2 &&
					-2 < enemy->GetWorldTransform().translation.y - player->GetAimPos().y) {
					if (enemy->GetWorldTransform().translation.z - player->GetAimPos().z < 200 &&
						-200 < enemy->GetWorldTransform().translation.z - player->GetAimPos().z) {

						player->AimHit();

					}
				}
			}
		}

		const std::list < std::unique_ptr<Pattern2>>& playerAim = player->GetAim();
		for (std::unique_ptr<Enemy>& enemy : enemys2) {
			for (const std::unique_ptr<Pattern2>& bulletA : playerAim) {
				if (enemy->GetWorldTransform().translation.x - bulletA->GetWorldTransform().translation.x < 4 &&
					-4 < enemy->GetWorldTransform().translation.x - bulletA->GetWorldTransform().translation.x) {
					if (enemy->GetWorldTransform().translation.y - bulletA->GetWorldTransform().translation.y < 4 &&
						-4 < enemy->GetWorldTransform().translation.y - bulletA->GetWorldTransform().translation.y) {
						if (enemy->GetWorldTransform().translation.z - bulletA->GetWorldTransform().translation.z < 5 &&
							-5 < enemy->GetWorldTransform().translation.z - bulletA->GetWorldTransform().translation.z) {

							bulletA->OnCollision();
						}
					}
				}
			}
		}

		for (std::unique_ptr<Enemy>& enemy : enemys1) {
			for (const std::unique_ptr<Pattern2>& bulletA : playerAim) {
				if (enemy->GetWorldTransform().translation.x - bulletA->GetWorldTransform().translation.x < 4 &&
					-4 < enemy->GetWorldTransform().translation.x - bulletA->GetWorldTransform().translation.x) {
					if (enemy->GetWorldTransform().translation.y - bulletA->GetWorldTransform().translation.y < 4 &&
						-4 < enemy->GetWorldTransform().translation.y - bulletA->GetWorldTransform().translation.y) {
						if (enemy->GetWorldTransform().translation.z - bulletA->GetWorldTransform().translation.z < 5 &&
							-5 < enemy->GetWorldTransform().translation.z - bulletA->GetWorldTransform().translation.z) {

							bulletA->OnCollision();
						}
					}
				}
			}
		}

		//自機とアイテムの当たり判定
		for (const std::unique_ptr<Item>& item : items_) {
			if (player->GetPos().x - item->GetPos().x < 2 &&
				-2 < player->GetPos().x - item->GetPos().x) {
				if (player->GetPos().y - item->GetPos().y < 2 &&
					-2 < player->GetPos().y - item->GetPos().y) {
					if (player->GetPos().z - item->GetPos().z < 2 &&
						-2 < player->GetPos().z - item->GetPos().z) {

						if (item->GetIsDead() == false) {
							item->OnCollision();

							if (item->GetWeapon() == 0) {
								rhythm->SetWeapon(Weapons::Normal);
							}
							else if (item->GetWeapon() == 1) {
								rhythm->SetWeapon(Weapons::Rapid);
							}
							else if (item->GetWeapon() == 2) {
								rhythm->SetWeapon(Weapons::ThreeWay);
							}
							else if (item->GetWeapon() == 3) {
								rhythm->SetWeapon(Weapons::Explosion);
							}
							else if (item->GetWeapon() == 4) {
								rhythm->SetWeapon(Weapons::Laser);
							}

							rhythm->ItemSoundPlay(1.0f);
						}
					}
				}
			}
		}

//ヒットボックスの最大値、最小値を定義
Vector3 Min = { 
	player->GetWorldTransform().translation.x - player->GetWorldTransform().scale.x,
	player->GetWorldTransform().translation.y - player->GetWorldTransform().scale.y,
	player->GetWorldTransform().translation.z - player->GetWorldTransform().scale.z};
Vector3 Max = { 
	player->GetWorldTransform().translation.x + player->GetWorldTransform().scale.x,
	player->GetWorldTransform().translation.y + player->GetWorldTransform().scale.y,
	player->GetWorldTransform().translation.z + player->GetWorldTransform().scale.z };
//for (std::unique_ptr<Collision>& collision : collisionsEnemy)
//{
//	if (collision->Update(player->GetWorldTransform().translation, player->GetWorldTransform().scale) == 1)
//	{
//		player->OnCollision();
//	}
//}

}

void GameScene::SetCollisionEnemy(Vector3 position, Vector3 scale)
{
	std::unique_ptr<Collision>newCollision = std::make_unique<Collision>();
	newCollision->SetObject(position, scale);
	collisionsEnemy.push_back(std::move(newCollision));
};