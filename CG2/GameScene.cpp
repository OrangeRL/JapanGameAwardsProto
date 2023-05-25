#include "GameScene.h"
#include <cassert>
#include <chrono>
#include <thread>

GameScene::GameScene() {

}

GameScene::~GameScene() {
	delete rhythm;
	delete player;
	delete playerBullet;
	delete boss;
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
	Sprite::LoadTexture(3, L"Resources/white1x1.png");
	Sprite::LoadTexture(4, L"Resources/optionBackground.png");
	Sprite::LoadTexture(5, L"Resources/sceneChange.png");

	crosshair->LoadTexture(11, L"Resources/crosshair.png");
	crosshair = Sprite::Create(11, { 0,0 });

	sceneChangeSprite = new Sprite(5, { -window_width,0.0f }, { window_width,window_height }, { 1.0f,1.0f,1.0f,1.0f }, { 0,0 }, 0, 0);
	sceneChangeSprite->Initialize();

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
	skydome->color = { 0.8f,0.8f,0.8f,1.0f };

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

	viewProjection_ = reilCamera->GetViewProjection();

	//player->SetMap(map);
	//player->SetGoal(goal);

	particle = new Particle;
	particle->Initialize(&viewProjection_, &matProjection_, player);

	particle2 = new Particle;
	particle2->Initialize(&viewProjection_, &matProjection_, player);

	particle3 = new Particle;
	particle3->Initialize(&viewProjection_, &matProjection_, player);

	//loadEnemyPopData();

	rhythm = new Rhythm();
	rhythm->Initialize(&viewProjection_, &matProjection_);
	spawntime = 0;
#pragma region enemy初期化

	for (int i = 1; i <= enemyVal; i++)
	{
		std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
		newEnemy->Initialize(&viewProjection_, &matProjection_, L"Resources/red.png");
		newEnemy->Settransform({ 0.0f, 0.0f, -1000.0f });
		//pManager[i].Initialize(&viewProjection_, &matProjection_, L"Resources/purple1x1.png");
		//敵を登録
		enemys1.push_back(std::move(newEnemy));
	}
	//loadEnemyPopData(1);
	loadCount = false;
}

void GameScene::Update() {

	sceneChangeSprite->SetPosition({ MathFunc::easeInQuint(sceneShiftFlame / maxFlame) * -1300 ,0.0f });

	if (startTimer <= 0) {
		//シーン切り替え処理
		if (isSceneChange == true) {

			if (sceneShiftFlame > 0) {
				sceneShiftFlame--;
			}

			if (sceneShiftFlame <= 0) {
				if (scene_ == Scene::Title) {
					Reset();
					scene_ = Scene::Stage;
				}
				else if (scene_ == Scene::Stage) {
					Reset();
					scene_ = Scene::Title;
				}

				isSceneChange = false;
			}
		}
		else {
			if (sceneShiftFlame < maxFlame) {
				sceneShiftFlame++;
			}
		}

		//タイトル画面の更新処理
		if (scene_ == Scene::Title) {
			loadCount = false;
			TitleUpdate();
		}
		//ゲーム画面の更新処理
		else if (scene_ == Scene::Stage) {
			loadCount = true;
			StageUpdate();
		}
	}
	else {
		startTimer--;
	}

}
void GameScene::TitleUpdate() {
	UIManager.TitleUpdate(rhythm, &input_);

	skydome->worldTransform.rotation.z += 0.001f;
	skydome->worldTransform.rotation.y += 0.002f;
	skydome->Update();

	player->SetPos({ 0.0f,0.0f,20.0f });
	player->Update(reilCamera->GetWorldTransform(), reilCamera->GetWorldTransform().rotation);
	boss->SetHP(25);
	spawntime = 0;
	viewProjection_.UpdateView();

	rhythm->Update(&input_, { 4.3f - UIManager.GetOptionPos() / 90.0f,7.5f + UIManager.GetOptionPos() / 150.0f,-25.0f }, { 0.0f,0.0f,0.0f }, player->GetIsDead(), stage, scene_, UIManager.GetSceneInTitle());

	if (UIManager.GetSceneInTitle() == 5) {
		isSceneChange = true;
	}

	if (sceneShiftFlame <= 0) {
		Reset();
	}

	if (rhythm->GetSoundState().timer == 0) {
		if (rhythm->GetSoundState().measureCount == 16) {
			skydome->color = { 0.0f,0.0f,1.0f,1.0f };
		}
		else if (rhythm->GetSoundState().measureCount == 32) {
			skydome->color = { 1.0f,0.0f,0.0f,1.0f };
		}
		else if (rhythm->GetSoundState().measureCount == 48) {
			skydome->color = { 0.8f,0.8f,0.8f,1.0f };
		}
	}


}

void GameScene::StageUpdate()
{
	skydome->worldTransform.rotation = { 0.0f,0.0f,0.0f };
	skydome->color = { 0.8f,0.8f,0.8f,1.0f };

	spawntime += 1;
	if (loadCount = true) {
		LoadCsv(enemyVal);
		LoadCsv2(enemyVal);
	}
	else if (loadCount == false)
	{
		loadCount = true;
	}

	//ランダムな整数
	std::default_random_engine engine(seed_gen());

	viewProjection_ = reilCamera->GetViewProjection();

	viewProjection_.UpdateView();

	//UI更新
	UIManager.Update(rhythm, player, &input_, player->GetIsDead(), boss->GetHP());

	rhythm->Update(&input_, player->GetPos(), reilCamera->GetWorldTransform().rotation, player->GetIsDead(), stage, scene_, UIManager.GetSceneInTitle());

	if (rhythm->GetSoundState().isPause == 0) {

		player->Update(reilCamera->GetWorldTransform(), reilCamera->GetWorldTransform().rotation);

		if (player->GetIsDead() == false) {
			reilCamera->Update(&input_, rhythm->GetSoundState().wave);
		}
		else
		{
			if (input_.TriggerKey(DIK_R)) {
				isSceneChange = true;
				rhythm->DecisionSoundPlay();
				/*scene_ = Scene::Title;
				rhythm->ResetRhythm();
				viewProjection_.Initialize();
				UIManager.Init();*/
			}
		}

		particle->Update();
		particle2->Update2();

		skydome->Update();
		player->Aim(player->GetWorldTransform().translation, { 0,0,100 }, reilCamera->GetWorldTransform().rotation, 0.0f);

		crosshair->SetPosition({ player->GetWorldTransform().translation.x, player->GetWorldTransform().translation.y });
		crosshair->Sprite::SetSize({ 1,1 });

		//敵の更新処理
		for (std::unique_ptr<Enemy>& enemy : enemys1) {
			enemy->Update(&viewProjection_, &matProjection_, 0);
#pragma region makeEnemyBullet
			if (enemy->GetBulletNum() < 3)
			{
				if (enemy->GetAttackSpeed() <= 0.0f && enemy->GetPhase() == Phase::Attack) {
					//弾を生成
					std::unique_ptr<EnemyBullet> bullet = std::make_unique<EnemyBullet>();
					//初期化
					bullet->Initialize(&viewProjection_, &matProjection_, L"Resources/white1x1.png", player->GetPos(), enemy->GetWorldTransform().translation);
					bullet->SetTransform(enemy->GetWorldTransform().translation);
					//使う弾の設定
					bullet->SetBullet(enemy->GetBulletNum());
					bullets1.push_back(std::move(bullet));
					//攻撃頻度の設定 1(速い)~ >1(遅い)
					enemy->SetAttackSpeed(100.0f);
					if (enemy->GetIsAttack() == false) {
						enemy->SetIsAttack(true);
					}
				}

			}
			for (std::unique_ptr<EnemyBullet>& bullet : bullets1) {
				bullet->Update(enemy->GetIsDead());
			}

			//弾&敵を削除する
			//rhythm->StopBGM();
			bullets1.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->IsDead(); });
			//rhythm->PlayBGM();
#pragma endregion
		}

		for (std::unique_ptr<EnemyBullet>& bullet : bullets1) {
			bullet->Update();
		}

		//弾&敵を削除する
		//rhythm->StopBGM();
		bullets1.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->IsDead(); });
		//rhythm->PlayBGM();
    
    //音ズレ関係
		rhythm->StopBGM();

		//敵の削除
		enemys1.remove_if([](std::unique_ptr<Enemy>& enemy) {return enemy->IsDead(); });
		rhythm->PlayBGM();

		//ボス関連
		if (rhythm->GetSoundState().wave == 3) {
			boss->Update();
#pragma region made BossBullet
			if (boss->GetIsDead() == false) {
				if (boss->GetPhase() == BossPhase::attack && boss->GetAttackSpeed() <= 0.0f) {
					//弾を生成
					std::unique_ptr<BossBullet> bullet = std::make_unique<BossBullet>();
					bullet->Initialize(&viewProjection_, &matProjection_, player->GetPos(), boss->GetWorldTransform().translation);
					bullet->SetTransform(boss->GetWorldTransform().translation);
					bossBullet1.push_back(std::move(bullet));
					boss->SetAttackSpeed(20.0f);
					if (boss->GetIsAttack() == false) {
						boss->SetIsAttack(true);
					}
				}
				if (boss->GetPhase() == BossPhase::attack2) {

				}
				for (std::unique_ptr<BossBullet>& bullet : bossBullet1) {
					bullet->Update();
				}

				//弾&敵を削除する
				bossBullet1.remove_if([](std::unique_ptr<BossBullet>& bullet) { return bullet->IsDead(); });

			}
#pragma endregion
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

		Collisions();
		if (waitTime > 0) {
			waitTime--;
		}
		//プレイヤーの弾発射処理
		const std::list < std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullets();
		if (input_.TriggerKey(DIK_SPACE) && rhythm->GetSoundState().isFireSucces) {

			for (std::unique_ptr<Enemy>& enemy : enemys1) {
				if (enemy->GetAimFlag() == true)
				{
					player->NewBullet(&viewProjection_, &matProjection_, enemy->GetWorldTransform().translation, player->GetWorldTransform().translation, rhythm->GetSoundState().weapon);
				}
			}
			if (rhythm->GetSoundState().wave == 3) {
				player->NewBullet(&viewProjection_, &matProjection_, boss->GetWorldTransform().translation, player->GetWorldTransform().translation, rhythm->GetSoundState().weapon);
			}
			//player->NewBulletAim(&viewProjection_, &matProjection_, enemyPos, player->GetWorldTransform().translation);
		}

		if (boss->GetSceneChange() == true)
		{
			//scene_ = Scene::Title;
			//rhythm->ResetRhythm();
			//viewProjection_.Initialize();
			//UIManager.Init();
		}

		//クリア後タイトルに戻る
		if (boss->GetHP() <= 0) {

			clearTimer--;

			if (clearTimer <= 0) {
				isSceneChange = true;
				rhythm->StopBGM();
			}
		}
	}
	else {
		if (input_.TriggerKey(DIK_T)) {

			isSceneChange = true;
			rhythm->DecisionSoundPlay();
		}


		/*if (sceneShiftFlame <= 0) {

			scene_ = Scene::Title;
			rhythm->ResetRhythm();
			player->SetPos({ 0.0f,0.0f,20.0f });
			viewProjection_.Initialize();
			UIManager.Init();
		}*/
	}

#pragma region DebugText
	//debugText.Printf(0, 100, 1.0f, 6, " HP:%d", boss->GetHP());
	////debugText.Printf(0, 200, 1.0f, 16, " Player:%f,%f,%f",
	////	playerBullet->GetWorldTransform().translation.x, playerBullet->GetWorldTransform().translation.y, playerBullet->GetWorldTransform().translation.z);
	//debugText.Printf(0, 150, 1.0f, 14, " Boss:%2f,%2f,%2f",
	//	boss->GetWorldTransform().translation.x, boss->GetWorldTransform().translation.y, boss->GetWorldTransform().translation.z);
	debugText.Printf(0, 230, 1.0f, 27, " %f,%f,%f",
		player->GetWorldTransform().matWorld.m[3][0],
		player->GetWorldTransform().matWorld.m[3][1],
		player->GetWorldTransform().matWorld.m[3][2]);
	debugText.Printf(0, 200, 1.0f, 12, "Enemy:%d", spawntime);
	debugText.Printf(0, 250, 1.0f, 12, "AimCount:%d", aimCount);
#pragma endregion
}

void GameScene::Draw() {
	if (scene_ == Scene::Title) {
		TitleDraw();
	}
	else if (scene_ == Scene::Stage) {
		StageDraw();
	}
}

void GameScene::TitleDraw() {
	skydome->Draw();
	rhythm->Draw(player->GetIsDead());

	//スプライト描画
	Sprite::PreDraw(dx12base_.GetCmdList().Get());
	UIManager.Draw(rhythm, boss->GetHP());

	//シーン切り替えの描画
	sceneChangeSprite->Draw();

	Sprite::PostDraw();

}
void GameScene::StageDraw() {
	//3D描画
	//プレイヤー描画
	player->Draw();

	rhythm->Draw(player->GetIsDead());
	particle->Draw();
	particle2->Draw();
	/*for (int i = 0; i < enemyVal; i++) {
		pManager[i].Draw();
	}*/
	//敵の描画
	for (std::unique_ptr<Enemy>& enemy : enemys1) {
		if (enemy->GetSpownFlag() == true)
		{
			enemy->Draw();
		}
	}
	for (std::unique_ptr<EnemyBullet>& bullet : bullets1) {
		if (bullet->GetIsDead() == false) {
			bullet->Draw();
		}
	}

	if (rhythm->GetSoundState().wave == 3) {
		boss->Draw();
		for (std::unique_ptr<BossBullet>& bullet : bossBullet1) {
			bullet->Draw();
		}

		for (std::unique_ptr<BossBullet>& bullet : bossBullet2) {
			bullet->Draw();
		}
	}



	//アイテム描画
	for (std::unique_ptr<Item>& item : items_) { item->Draw(); }
	skydome->Draw();

	//スプライト描画
	Sprite::PreDraw(dx12base_.GetCmdList().Get());


	crosshair->Draw();

	UIManager.Draw(rhythm, boss->GetHP());

	// デバッグテキストの描画
	debugText.DrawAll(dx12base_.GetCmdList().Get());

	//シーン切り替えの描画
	sceneChangeSprite->Draw();

	Sprite::PostDraw();

}

void GameScene::Reset() {
	viewProjection_.Initialize();
	UIManager.Init();
	clearTimer = 500;
	reilCamera->Initialize({ 0,0,-50 }, { 0,0,0 });
	rhythm->ResetRhythm();
	player->Reset();
	player->SetPos({ 0.0f, 0.0f, 20.0f });
	particle->Reset();
	particle2->Reset();
	boss->Reset();
	spawntime = 0;
	aimCount = 0;
	if (loadCount == true)
	{
		loadCount = false;
	}
	for (int i = 1; i <= enemyVal; i++) {
		for (std::unique_ptr<Enemy>& newEnemy : enemys1) {
			newEnemy->Reset();

		}
	}
	for (std::unique_ptr<EnemyBullet>& bullet : bullets1) {
		if (bullet->GetIsDead() == false) {
			bullet->SetIsDead(true);
		}
	}
}

void GameScene::Collisions() {
	for (std::unique_ptr<Enemy>& enemy : enemys1) {
		if (enemy->GetWorldTransform().translation.x - player->GetAimPos().x < 5 &&
			-5 < enemy->GetWorldTransform().translation.x - player->GetAimPos().x) {
			if (enemy->GetWorldTransform().translation.y - player->GetAimPos().y < 5 &&
				-5 < enemy->GetWorldTransform().translation.y - player->GetAimPos().y) {
				if (enemy->GetWorldTransform().translation.z - player->GetAimPos().z < 50 &&
					-100 < enemy->GetWorldTransform().translation.z - player->GetAimPos().z) {
					player->AimHit();
				}
				else
				{
					player->NotAimHit();
				}
			}
		}
	}
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

					player->OnCollision(rhythm);

				}
			}
		}
#pragma endregion

#pragma region bulletToEnemyCollisions
		for (const std::unique_ptr<PlayerBullet>& bulletA : playerBullets) {
			if (enemy->GetWorldTransform().translation.x - bulletA->GetWorldTransform().translation.x < 3 &&
				-3 < enemy->GetWorldTransform().translation.x - bulletA->GetWorldTransform().translation.x) {
				if (enemy->GetWorldTransform().translation.y - bulletA->GetWorldTransform().translation.y < 3 &&
					-3 < enemy->GetWorldTransform().translation.y - bulletA->GetWorldTransform().translation.y) {
					if (enemy->GetWorldTransform().translation.z - bulletA->GetWorldTransform().translation.z < 3 &&
						-3 < enemy->GetWorldTransform().translation.z - bulletA->GetWorldTransform().translation.z) {

						//bulletA->OnCollision();

						if (enemy->GetAimFlag() == true)
						{
							enemy->OnCollision(rhythm);
						}

						aimCount = 0;
						rhythm->ComboUp();
						UIManager.PlusScore(rhythm);
					}
				}
			}
		}


		for (const std::unique_ptr<PlayerBullet>& bulletA : playerBullets) {
			if (boss->GetWorldTransform().translation.x - bulletA->GetWorldTransform().translation.x < 40 &&
				-40 < boss->GetWorldTransform().translation.x - bulletA->GetWorldTransform().translation.x) {
				if (boss->GetWorldTransform().translation.y - bulletA->GetWorldTransform().translation.y < 200 &&
					-200 < boss->GetWorldTransform().translation.y - bulletA->GetWorldTransform().translation.y) {
					if (boss->GetWorldTransform().translation.z - bulletA->GetWorldTransform().translation.z < 20 &&
						-20 < boss->GetWorldTransform().translation.z - bulletA->GetWorldTransform().translation.z) {

						bulletA->OnCollision();
						boss->OnCollision();

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

						//bulletA->OnCollision();
						bulletB->OnCollision();

					}
				}
			}
		}
	}

	//bullet-enemyBullet
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
					player->OnCollision(rhythm);
				}
			}
		}
	}
#pragma endregion

#pragma region BossBulletaToPlayer
	//ランダム弾
	for (const std::unique_ptr<BossBullet>& bulletB : bossBullet1) {

		if (player->GetPos().x - bulletB->GetWorldTransform().translation.x < 2 &&
			-2 < player->GetPos().x - bulletB->GetWorldTransform().translation.x) {
			if (player->GetPos().y - bulletB->GetWorldTransform().translation.y < 2 &&
				-2 < player->GetPos().y - bulletB->GetWorldTransform().translation.y) {
				if (player->GetPos().z - bulletB->GetWorldTransform().translation.z < 2 &&
					-2 < player->GetPos().z - bulletB->GetWorldTransform().translation.z) {

					//bulletB->OnCollision();
					//enemy->Reset();
					player->OnCollision(rhythm);
				}
			}
		}
	}
	//移動弾
	for (const std::unique_ptr<BossBullet>& bulletB : bossBullet2) {

		if (player->GetPos().x - bulletB->GetWorldTransform().translation.x < 2 &&
			-2 < player->GetPos().x - bulletB->GetWorldTransform().translation.x) {
			if (player->GetPos().y - bulletB->GetWorldTransform().translation.y < 2 &&
				-2 < player->GetPos().y - bulletB->GetWorldTransform().translation.y) {
				if (player->GetPos().z - bulletB->GetWorldTransform().translation.z < 2 &&
					-2 < player->GetPos().z - bulletB->GetWorldTransform().translation.z) {

					//bulletB->OnCollision();
					//enemy->Reset();
					player->OnCollision(rhythm);
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
				if (enemy->GetWorldTransform().translation.z - player->GetAimPos().z < 60 &&
					-2 < enemy->GetWorldTransform().translation.z - player->GetAimPos().z) {
					player->AimHit();
					if (enemy->GetAimFlag() == false && aimCount < 5) {
						aimCount++;
						enemy->AimCheck();
					}
				}
			}
		}
	}

	const std::list < std::unique_ptr<Pattern2>>& playerAim = player->GetAim();

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

	for (std::unique_ptr<Item>& items : items_) {
		if (items->GetWorldTransform().translation.x - player->GetPos().x < 2 &&
			-2 < items->GetWorldTransform().translation.x - player->GetPos().x) {
			if (items->GetWorldTransform().translation.y - player->GetPos().y < 2 &&
				-2 < items->GetWorldTransform().translation.y - player->GetPos().y) {
				if (items->GetWorldTransform().translation.z - player->GetPos().z < 2 &&
					-2 < items->GetWorldTransform().translation.z - player->GetPos().z) {

					items->OnCollision();
					if (items->GetWeapon() == 0) {
						rhythm->SetWeapon(Weapons::Normal);
					}
					else if (items->GetWeapon() == 1) {
						rhythm->SetWeapon(Weapons::Rapid);
					}
					else if (items->GetWeapon() == 2) {
						rhythm->SetWeapon(Weapons::ThreeWay);
					}

					rhythm->ItemSoundPlay();
				}
			}
		}
	}
}

void GameScene::LoadCsv(int obstacleVal)
{
	// open file
	std::ifstream file;
	file.open(L"Resources/wave1.csv");
	std::stringstream obstaclePosList;    // string stream
	obstaclePosList << file.rdbuf();
	file.close();

	//1行分の文字列
	std::string line;

	//コマンド実行
	while (std::getline(obstaclePosList, line)) {
		//1行分の文字列をストリームに変換
		std::istringstream line_stream(line);
		//std::string first_value;
		std::string word;
		std::getline(line_stream, word, ',');
		if (word.find("//") == 0) {
			continue;
		}
		if (word.find("{") == 0) {
			std::getline(line_stream, word, ',');
			int x = (int)std::atof(word.c_str());
			std::getline(line_stream, word, ',');
			int y = (int)std::atof(word.c_str());
			std::getline(line_stream, word, ',');
			int z = (int)std::atof(word.c_str());
			std::getline(line_stream, word, ',');
			int32_t timer = (int)std::atof(word.c_str());
			std::getline(line_stream, word, ',');
			useBullet = (int)std::atof(word.c_str());
			std::getline(line_stream, word, ',');
			enemyMove = (int)std::atof(word.c_str());
			//座標
			Vector3 pos(x, y, z);
			//待ち時間
			int32_t waitTime = timer;
			obstaclePos.push_back(pos);
			spawntimer.push_back(waitTime);
			bulletNum.push_back(useBullet);
			moveNum.push_back(enemyMove);
		}
	}
	int i = 0;
	spawntime += 1;
	for (std::unique_ptr<Enemy>& newEnemy : enemys1) {
		if (spawntime == spawntimer[0]) {
			if (i < obstaclePos.size() && i < 15 && i >= 0) {
				newEnemy->Settransform(obstaclePos[i]);
				newEnemy->SetBulletNum(bulletNum[i]);
				newEnemy->SetMoveNum(moveNum[i]);
				newEnemy->SetSpeed(0, 0, 0);
				//if (newEnemy->GetSpownFlag() == false) {
				newEnemy->Spawn();
				//}
			}
			i++;
		}
		if (spawntime == spawntimer[15]) {
			if (i < obstaclePos.size() && i < 30 && i >= 15) {
				newEnemy->Settransform(obstaclePos[i]);
				newEnemy->SetBulletNum(bulletNum[i]);
				newEnemy->SetMoveNum(moveNum[i]);
				newEnemy->SetSpeed(0, 0, 0);
				//if (newEnemy->GetSpownFlag() == false) {
				newEnemy->Spawn();
				//}
			}
			i++;
		}
		if (spawntime == spawntimer[30]) {
			if (i < obstaclePos.size() && i < 45 && i >= 30) {
				newEnemy->Settransform(obstaclePos[i]);
				newEnemy->SetBulletNum(bulletNum[i]);
				newEnemy->SetMoveNum(moveNum[i]);
				newEnemy->SetSpeed(0, 0, 0);
				//if (newEnemy->GetSpownFlag() == false) {
				newEnemy->Spawn();
				//}
			}
			i++;
		}
		if (spawntime == spawntimer[45]) {
			if (i < obstaclePos.size() && i < 60 && i >= 45) {
				newEnemy->Settransform(obstaclePos[i]);
				newEnemy->SetBulletNum(bulletNum[i]);
				newEnemy->SetMoveNum(moveNum[i]);
				newEnemy->SetSpeed(0, 0, 0);
				//if (newEnemy->GetSpownFlag() == false) {
				newEnemy->Spawn();
				//}
			}
			i++;
		}
		if (spawntime == spawntimer[60]) {
			if (i < obstaclePos.size() && i < 75 && i >= 60) {
				newEnemy->Settransform(obstaclePos[i]);
				newEnemy->SetBulletNum(bulletNum[i]);
				newEnemy->SetMoveNum(moveNum[i]);
				newEnemy->SetSpeed(0, 0, 0);
				//if (newEnemy->GetSpownFlag() == false) {
				newEnemy->Spawn();
				//}
			}
			i++;
		}
		if (spawntime == spawntimer[75]) {
			if (i < obstaclePos.size() && i < 90 && i > 75) {
				newEnemy->Settransform(obstaclePos[i]);
				newEnemy->SetBulletNum(bulletNum[i]);
				newEnemy->SetMoveNum(moveNum[i]);
				newEnemy->SetSpeed(0, 0, 0);
				//if (newEnemy->GetSpownFlag() == false) {
				newEnemy->Spawn();
				//}
			}
			i++;
		}
	}
}

void GameScene::LoadCsv2(int obstacleVal)
{
	// open file
	std::ifstream file;
	file.open(L"Resources/wave2.csv");
	std::stringstream obstaclePosList;    // string stream
	obstaclePosList << file.rdbuf();
	file.close();

	//1行分の文字列
	std::string line;
	std::vector<Vector3> obstaclePos;
	std::vector<int32_t> spawntimer;
	std::vector<int32_t> bulletNum;
	std::vector<int32_t> moveNum;
	//コマンド実行
	while (std::getline(obstaclePosList, line)) {
		//1行分の文字列をストリームに変換
		std::istringstream line_stream(line);
		//std::string first_value;
		std::string word;
		std::getline(line_stream, word, ',');
		if (word.find("//") == 0) {
			continue;
		}
		if (word.find("{") == 0) {
			std::getline(line_stream, word, ',');
			int x = (int)std::atof(word.c_str());
			std::getline(line_stream, word, ',');
			int y = (int)std::atof(word.c_str());
			std::getline(line_stream, word, ',');
			int z = (int)std::atof(word.c_str());
			std::getline(line_stream, word, ',');
			int32_t timer = (int)std::atof(word.c_str());
			std::getline(line_stream, word, ',');
			useBullet = (int)std::atof(word.c_str());
			std::getline(line_stream, word, ',');
			enemyMove = (int)std::atof(word.c_str());
			//座標
			Vector3 pos(x, y, z);
			//待ち時間
			int32_t waitTime = timer;
			obstaclePos.push_back(pos);
			spawntimer.push_back(waitTime);
			bulletNum.push_back(useBullet);
			moveNum.push_back(enemyMove);
		}
	}
	if (spawntime == 10000) {
		for (int y = 1; y <= enemyVal; y++) {
			for (std::unique_ptr<Enemy>& newEnemy : enemys1) {
				newEnemy->Reset();

			}
		}
	}
	int i = 0;
	//spawntime += 1;
	for (std::unique_ptr<Enemy>& newEnemy : enemys1) {
		if (spawntime == spawntimer[0]) {
			if (i < obstaclePos.size() && i < 15 && i >= 0) {
				newEnemy->Settransform(obstaclePos[i]);
				newEnemy->SetBulletNum(bulletNum[i]);
				newEnemy->SetMoveNum(moveNum[i]);
				newEnemy->SetSpeed(0, 0, 0);
				//if (newEnemy->GetSpownFlag() == false) {
				newEnemy->Spawn();
				//}
			}
			i++;
		}
		if (spawntime == spawntimer[15]) {
			if (i < obstaclePos.size() && i < 30 && i >= 15) {
				newEnemy->Settransform(obstaclePos[i]);
				newEnemy->SetBulletNum(bulletNum[i]);
				newEnemy->SetMoveNum(moveNum[i]);
				newEnemy->SetSpeed(0, 0, 0);
				//if (newEnemy->GetSpownFlag() == false) {
				newEnemy->Spawn();
				//}
			}
			i++;
		}

		if (spawntime == spawntimer[30]) {
			if (i < obstaclePos.size() && i < 45 && i >= 30) {
				newEnemy->Settransform(obstaclePos[i]);
				newEnemy->SetBulletNum(bulletNum[i]);
				newEnemy->SetMoveNum(moveNum[i]);
				newEnemy->SetSpeed(0, 0, 0);
				//if (newEnemy->GetSpownFlag() == false) {
				newEnemy->Spawn();
				//}
			}
			i++;
		}
		if (spawntime == spawntimer[45]) {
			if (i < obstaclePos.size() && i < 60 && i >= 45) {
				newEnemy->Settransform(obstaclePos[i]);
				newEnemy->SetBulletNum(bulletNum[i]);
				newEnemy->SetMoveNum(moveNum[i]);
				newEnemy->SetSpeed(0, 0, 0);
				//if (newEnemy->GetSpownFlag() == false) {
				newEnemy->Spawn();
				//}
			}
			i++;
		}
		if (spawntime == spawntimer[60]) {
			if (i < obstaclePos.size() && i < 93 && i > 60) {
				newEnemy->Settransform(obstaclePos[i]);
				newEnemy->SetBulletNum(bulletNum[i]);
				newEnemy->SetMoveNum(moveNum[i]);
				newEnemy->SetSpeed(0, 0, 0);
				//if (newEnemy->GetSpownFlag() == false) {
				newEnemy->Spawn();
				//}
			}
			i++;
		}
		//if (spawntime == spawntimer[24]) {
		//	if (i < obstaclePos.size() && i < 32 && i > 23) {
		//		newEnemy->Settransform(obstaclePos[i]);
		//		newEnemy->SetBulletNum(bulletNum[i]);
		//		newEnemy->SetMoveNum(moveNum[i]);
		//		newEnemy->SetSpeed(0, 0, 0);
		//		if (newEnemy->GetSpownFlag() == false) {
		//			newEnemy->Spawn();
		//		}
		//	}
		//	i++;
		//}
		//if (spawntime == spawntimer[32]) {
		//	if (i < obstaclePos.size() && i < 40 && i > 31) {
		//		newEnemy->Settransform(obstaclePos[i]);
		//		newEnemy->SetBulletNum(bulletNum[i]);
		//		newEnemy->SetMoveNum(moveNum[i]);
		//		newEnemy->SetSpeed(0, 0, 0);
		//		if (newEnemy->GetSpownFlag() == false) {
		//			newEnemy->Spawn();
		//		}
		//	}
		//	i++;
		//}
		//if (spawntime == spawntimer[40]) {
		//	if (i < obstaclePos.size() && i < 45 && i > 39) {
		//		newEnemy->Settransform(obstaclePos[i]);
		//		newEnemy->SetBulletNum(bulletNum[i]);
		//		newEnemy->SetMoveNum(moveNum[i]);
		//		newEnemy->SetSpeed(0, 0, 0);
		//		if (newEnemy->GetSpownFlag() == false) {
		//			newEnemy->Spawn();
		//		}
		//	}
		//	i++;
		//}
		//if (spawntime == spawntimer[45]) {
		//	if (i < obstaclePos.size() && i < 50 && i > 44) {
		//		newEnemy->Settransform(obstaclePos[i]);
		//		newEnemy->SetBulletNum(bulletNum[i]);
		//		newEnemy->SetMoveNum(moveNum[i]);
		//		newEnemy->SetSpeed(0, 0, 0);
		//		if (newEnemy->GetSpownFlag() == false) {
		//			newEnemy->Spawn();
		//		}
		//	}
		//	i++;
		//}
	}
}