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
	player->SetEnemy(enemy);

	particle = new Particle;
	particle->Initialize(&viewProjection_, &matProjection_, player);

	particle2 = new Particle;
	particle2->Initialize(&viewProjection_, &matProjection_, player);

}

void GameScene::Update() {

	viewProjection_.UpdateView();
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

	////enemy-player
	//if (enemy->GetWorldTransform().translation.x -				player->GetWorldTransform().translation.x < 2 &&
	//	-2 < enemy->GetWorldTransform().translation.x -			player->GetWorldTransform().translation.x) {
	//	if (enemy->GetWorldTransform().translation.y -			player->GetWorldTransform().translation.y < 3 &&
	//		-3 < enemy->GetWorldTransform().translation.y -		player->GetWorldTransform().translation.y) {
	//		if (enemy->GetWorldTransform().translation.z -		player->GetWorldTransform().translation.z < 3 &&
	//			-3 < enemy->GetWorldTransform().translation.z - player->GetWorldTransform().translation.z) {
	//			enemy->Reset();
	//		}
	//	}
	//}
}

void GameScene::Draw() {
	//3D描画

	player->Draw();
	//enemy->Draw();
	particle->Draw();
	particle2->Draw();

	//スプライト描画
	Sprite::PreDraw(dx12base_.GetCmdList().Get());


	Sprite::PostDraw();

}

void GameScene::Reset() {

	player->Reset();
	particle->Reset();
	//enemy->Reset();

}