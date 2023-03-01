#include "GameScene.h"
#include <cassert>


GameScene::GameScene() {

}

GameScene::~GameScene() {
	delete player;

	soundManager_.SoundUnload(soundData1);
	soundManager_.SoundUnload(selectSound);
}

void GameScene::Initialize(WinApp* winApp) {

	//透視投影変換行列の計算
	matProjection_ = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0),
		(float)winApp->window_width / winApp->window_height,
		0.1f, 1000.0f
	);

	viewProjection_.Initialize();

	viewProjection_.eye = { 0 , 100 , -100 };


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


	player = new Player();
	player->Initialize(&viewProjection_, &matProjection_);
	loadEnemyPopData();
}

void GameScene::Update() {

	viewProjection_.UpdateView();

	//シーン管理
	
	//プレイヤーの更新処理
	player->Update();

	//敵の更新処理
	for (std::unique_ptr<Enemy>& enemy : enemys1) {
		enemy->Update(&viewProjection_, &matProjection_, L"Resources/white1x1.png",0);
#pragma region makeEnemyBullet
		if (enemy->GetAttackSpeed() <= 0.0f) {
			//弾を生成
			std::unique_ptr<EnemyBullet> bullet = std::make_unique<EnemyBullet>();
			//初期化
			bullet->Initialize(&viewProjection_, &matProjection_, L"Resources/white1x1.png");
			bullet->SetTransform(enemy->GetWorldTransform().translation);
			//使う弾の設定
			bullet->SetBullet(0);
			bullets1.push_back(std::move(bullet));
			enemy->SetAttackSpeed(150.0f);
			if (enemy->GetIsAttack() == false) {
				enemy->SetIsAttack(true);
			}
		}

		if (enemy->GetIsAttack() == true) {
			for (std::unique_ptr<EnemyBullet>& bullet : bullets1) {
				bullet->Update(enemy->GetWorldTransform());
			}
		}
#pragma endregion

		

		//player->SetEnemy(enemy);
	}
	for (std::unique_ptr<Enemy>& enemy : enemys2) {
		enemy->Update(&viewProjection_, &matProjection_, L"Resources/white1x1.png",1);
#pragma region makeEnemyBullet
		if (enemy->GetAttackSpeed() <= 0.0f) {
			//弾を生成
			std::unique_ptr<EnemyBullet> bullet = std::make_unique<EnemyBullet>();
			//初期化
			bullet->Initialize(&viewProjection_, &matProjection_, L"Resources/white1x1.png");
			bullet->SetTransform(enemy->GetWorldTransform().translation);
			//使う弾の設定
			bullet->SetBullet(1);
			bullets2.push_back(std::move(bullet));
			enemy->SetAttackSpeed(25.0f);
			if (enemy->GetIsAttack() == false) {
				enemy->SetIsAttack(true);
			}
		}

		if (enemy->GetIsAttack() == true) {
			for (std::unique_ptr<EnemyBullet>& bullet : bullets2) {
				bullet->Update(enemy->GetWorldTransform());
			}
		}
#pragma endregion

	}
	UpdateEnemyPopCommand();

	viewProjection_.target = { player->GetWorldTransform().translation.x, player->GetWorldTransform().translation.y, player->GetWorldTransform().translation.z };
	viewProjection_.eye = { player->GetWorldTransform().translation.x, player->GetWorldTransform().translation.y, player->GetWorldTransform().translation.z - 30 };
	player->Update();
	if (player->GetIsDead() == false) {
		//enemy->Update(player->GetWorldTransform().translation, enemy->GetWorldTransform().translation);
	}


	particle->Update();
	particle2->Update2();
	player->NewBullet(&viewProjection_, &matProjection_);

	if (input_.PushKey(DIK_R)) {
		Reset();
	}

	if (player->GetIsDead() == true && particle->GetIsDead() == true) {
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
	}

}

void GameScene::Draw() {
	//3D描画
	//プレイヤー描画
	player->Draw();
	//敵の描画
	for (std::unique_ptr<Enemy>& enemy : enemys1) {
		enemy->Draw();	
	}
	for (std::unique_ptr<EnemyBullet>& bullet : bullets1) {
		bullet->Draw();
	}
	for (std::unique_ptr<Enemy>& enemy : enemys2) {
		enemy->Draw();
	}
	for (std::unique_ptr<EnemyBullet>& bullet : bullets2) {
		bullet->Draw();
	}


	//player->Draw();
	//enemy->Draw();
	particle->Draw();
	particle2->Draw();


	//スプライト描画
	Sprite::PreDraw(dx12base_.GetCmdList().Get());


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
	while (std::getline(enemyPopCommand,line)){
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
		if (world.find("Enemy1") == 0){
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
			newEnemy->Initialize(&viewProjection_, &matProjection_,L"Resources/white1x1.png");
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
	//enemy->Reset();
	
}

