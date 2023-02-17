#include "GameScene.h"
#include <cassert>


GameScene::GameScene() {

}

GameScene::~GameScene() {
	
	soundManager_.SoundUnload(soundData1);
	soundManager_.SoundUnload(selectSound);
}

void GameScene::Initialize(WinApp* winApp) {

	//透視投影変換行列の計算
	matProjection_ = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0) ,
		(float)winApp->window_width / winApp->window_height ,
		0.1f , 1000.0f
	);

	viewProjection_.Initialize();
	viewProjection_.eye = {0 ,100 , -100};

	enemy = new Enemy();
	enemy->Initialize(&viewProjection_, &matProjection_);
}

void GameScene::Update() {
	
	viewProjection_.UpdateView();
	//シーン管理
	enemy->Update();
}

void GameScene::Draw() {
	//3D描画
	enemy->Draw();

	//スプライト描画
	Sprite::PreDraw(dx12base_.GetCmdList().Get());


	Sprite::PostDraw();

}

void GameScene::Reset() {


}