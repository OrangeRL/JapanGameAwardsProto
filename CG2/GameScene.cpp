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
}

void GameScene::Initialize(WinApp* winApp) {

	// デバッグテキスト用テクスチャ読み込み
	Sprite::LoadTexture(10, L"Resources/debugfont.png");
	// デバッグテキスト初期化
	debugText.Initialize(10);

	//透視投影変換行列の計算
	matProjection_ = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0) ,
		(float)winApp->window_width / winApp->window_height ,
		0.1f , 1000.0f
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
	viewProjection_.eye = {0 , 100 , -100};

	enemy = new Enemy();
	enemy->Initialize(&viewProjection_, &matProjection_);

	rhythm = new Rhythm();
	rhythm->Initialize();

}

void GameScene::Update() {
	rhythm->Update(&input_);

	if (input_.TriggerKey(DIK_A) && offset > 0) {
		offset--;
	}
	else if (input_.TriggerKey(DIK_D) && offset < 10) {
		offset++;
	}

	debugText.Printf(0, 100, 1.0f, 10," offset:%d",offset);
	debugText.Printf(0, 120, 1.0f, 10," Timer:%f",rhythm->GetSoundState().timer);
	debugText.Printf(0, 140, 1.0f, 15," BGMVolume:%f", rhythm->GetSoundState().BGMVolume);
	debugText.Printf(0, 160, 1.0f, 19," guideSEVolume:%f", rhythm->GetSoundState().guideSEVolume);
	viewProjection_.UpdateView();
	//シーン管理
	enemy->Update();
}

void GameScene::Draw() {
	//3D描画
	enemy->Draw();

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

void GameScene::Reset() {


}