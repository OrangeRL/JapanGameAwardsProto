#include "GameScene.h"
#include <cassert>

GameScene::GameScene() {

}

GameScene::~GameScene() {
	delete rhythm;
	delete enemy;
	for (int i = 0; i < 10; i++) {
		delete num_[i];
	}
	delete player;

}

void GameScene::Initialize(WinApp* winApp) {

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

	rhythm = new Rhythm();
	rhythm->Initialize();
  
  	//XAudioエンジンのインスタンスを生成
	soundManager_.Initialize();

	player = new Player();
	player->Initialize(&viewProjection_, &matProjection_);

	player->SetMap(map);
	player->SetGoal(goal);

	particle = new Particle;
	particle->Initialize(&viewProjection_, &matProjection_, player);

	particle2 = new Particle;
	particle2->Initialize(&viewProjection_, &matProjection_, player);

	loadEnemyPopData();

}

void GameScene::Update() {
	rhythm->Update(&input_);

	if (input_.TriggerKey(DIK_O) && offset > 0) {
		offset--;
	}
	else if (input_.TriggerKey(DIK_P) && offset < 10) {
		offset++;
	}

	debugText.Printf(0, 100, 1.0f, 10," O,P...offset:%d",offset);
	debugText.Printf(0, 140, 1.0f, 25," Up,Dawn...BGMVolume:%f", rhythm->GetSoundState().BGMVolume);
	debugText.Printf(0, 160, 1.0f, 32," Left,Right...guideSEVolume:%f", rhythm->GetSoundState().guideSEVolume);
	debugText.Printf(0, 120, 1.0f, 10," Timer:%f",rhythm->GetSoundState().timer);
	debugText.Printf(0, 180, 1.0f, 15," measureCount:%d", rhythm->GetSoundState().measureCount);
	debugText.Printf(0, 200, 1.0f, 9," weapon:%d", rhythm->GetSoundState().weapon);
  
	viewProjection_.eye = { 0 , 100 , -100 };
  
	Collision();
  
	viewProjection_.UpdateView();
	if (input_.PushKey(DIK_P)) {
		//player->OnCollision();
	}
	//シーン管理

	//プレイヤーの更新処理
	player->Update();
	particle->Update();
	particle2->Update2();

	//敵の更新処理
	for (std::unique_ptr<Enemy>& enemy : enemys1) {
		enemy->Update(&viewProjection_, &matProjection_, L"Resources/white1x1.png", 0);
		enemyPos = enemy->GetWorldTransform().translation;
		//player->SetEnemy(enemy);
		//player->NewBullet(&viewProjection_, &matProjection_, enemyPos, player->GetWorldTransform().translation);
	}

	for (std::unique_ptr<Enemy>& enemy : enemys2) {
		enemy->Update(&viewProjection_, &matProjection_, L"Resources/white1x1.png", 1);
		//player->NewBullet(&viewProjection_, &matProjection_, enemy->GetWorldTransform().translation, player->GetWorldTransform().translation);
	}
	UpdateEnemyPopCommand();



	viewProjection_.target = { player->GetWorldTransform().translation.x, player->GetWorldTransform().translation.y, player->GetWorldTransform().translation.z };
	viewProjection_.eye = { player->GetWorldTransform().translation.x, player->GetWorldTransform().translation.y, player->GetWorldTransform().translation.z - 30 };

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

	//スプライト描画
	Sprite::PreDraw(dx12base_.GetCmdList().Get());


	for (int i = 0; i < 10; i++) {
		if (offset == i) {
			//num_[i]->Draw();
		}
	}

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

			//敵を発生させる
			//-------ここにEnemy発生関数---------//
			//複数化するためにuniq_ptrに変更
			std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
			newEnemy->Initialize(&viewProjection_, &matProjection_, L"Resources/white1x1.png");
			//上で書いてある物をEnemyの座標としてセットする
			newEnemy->Settransform(x, y, z);
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
			float speed = (float)std::atof(world.c_str());
			//敵を発生させる
			//-------ここにEnemy発生関数---------//
			//複数化するためにuniq_ptrに変更
			std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
			newEnemy->Initialize(&viewProjection_, &matProjection_, L"Resources/e.png");
			//上で書いてある物をEnemyの座標としてセットする
			newEnemy->Settransform(x, y, z);
			newEnemy->SetSpeed(speed);
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
	//enemy->Reset();

}

void GameScene::Collision() {

	const std::list < std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullets();
	for (const std::unique_ptr<PlayerBullet>& bulletA : playerBullets) {
		if (input_.PushKey(DIK_P)) {
			player->OnCollision();
			bulletA->OnCollision();
			break;
		}
	}
	for (std::unique_ptr<Enemy>& enemy : enemys1) {

		//enemy-player
		if (enemy->GetWorldTransform().translation.x - player->GetWorldTransform().translation.x < 2 &&
			-2 < enemy->GetWorldTransform().translation.x - player->GetWorldTransform().translation.x) {
			if (enemy->GetWorldTransform().translation.y - player->GetWorldTransform().translation.y < 3 &&
				-3 < enemy->GetWorldTransform().translation.y - player->GetWorldTransform().translation.y) {
				if (enemy->GetWorldTransform().translation.z - player->GetWorldTransform().translation.z < 3 &&
					-3 < enemy->GetWorldTransform().translation.z - player->GetWorldTransform().translation.z) {

					player->OnCollision();

				}
			}
		}
		//bullet-enemy

		const std::list < std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullets();
		for (const std::unique_ptr<PlayerBullet>& bulletA : playerBullets) {
			if (input_.PushKey(DIK_P)) {
				//player->OnCollision();
				bulletA->OnCollision();
			}
			if (enemy->GetWorldTransform().translation.x - bulletA->GetWorldTransform().translation.x < 2 &&
				-2 < enemy->GetWorldTransform().translation.x - bulletA->GetWorldTransform().translation.x) {
				if (enemy->GetWorldTransform().translation.y - bulletA->GetWorldTransform().translation.y < 2 &&
					-2 < enemy->GetWorldTransform().translation.y - bulletA->GetWorldTransform().translation.y) {
					if (enemy->GetWorldTransform().translation.z - bulletA->GetWorldTransform().translation.z < 2 &&
						-2 < enemy->GetWorldTransform().translation.z - bulletA->GetWorldTransform().translation.z) {

						bulletA->OnCollision();
						//enemy->Reset();
						//player->OnCollision();
					}
				}
			}
		}
		//player-enemybullet
		const std::list < std::unique_ptr<EnemyBullet>>& enemyBullets = enemy->GetBullets();
		for (const std::unique_ptr<EnemyBullet>& bulletB : enemyBullets) {

			if (player->GetWorldTransform().translation.x - bulletB->GetWorldTransform().translation.x < 2 &&
				-2 < player->GetWorldTransform().translation.x - bulletB->GetWorldTransform().translation.x) {
				if (player->GetWorldTransform().translation.y - bulletB->GetWorldTransform().translation.y < 3 &&
					-3 < player->GetWorldTransform().translation.y - bulletB->GetWorldTransform().translation.y) {
					if (player->GetWorldTransform().translation.z - bulletB->GetWorldTransform().translation.z < 3 &&
						-3 < player->GetWorldTransform().translation.z - bulletB->GetWorldTransform().translation.z) {

						//bulletB->OnCollision();
						enemy->Reset();
						player->OnCollision();
					}
				}
			}
		}
	}
}
