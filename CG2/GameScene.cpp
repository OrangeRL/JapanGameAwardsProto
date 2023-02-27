#include "GameScene.h"
#include <cassert>

GameScene::GameScene() {

}

GameScene::~GameScene() {
	delete num0_;
	delete num1_;
	delete num2_;
	delete num3_;
	delete num4_;
	delete num5_;
	delete num6_;
	delete num7_;
	delete num8_;
	delete num9_;
	//delete num10_;
	delete rhythm;
	delete enemy;
}

void GameScene::Initialize(WinApp* winApp) {

	//透視投影変換行列の計算
	matProjection_ = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0) ,
		(float)winApp->window_width / winApp->window_height ,
		0.1f , 1000.0f
	);

	
	num0_ = new Sprite(0,{0,0},{64,64},{1.0f,1.0f,1.0f,1.0f},{0,0},0,0);
	num1_ = new Sprite(1,{0,0},{64,64},{1.0f,1.0f,1.0f,1.0f},{0,0},0,0);
	num2_ = new Sprite(2,{0,0},{64,64},{1.0f,1.0f,1.0f,1.0f},{0,0},0,0);
	num3_ = new Sprite(3,{0,0},{64,64},{1.0f,1.0f,1.0f,1.0f},{0,0},0,0);
	num4_ = new Sprite(4,{0,0},{64,64},{1.0f,1.0f,1.0f,1.0f},{0,0},0,0);
	num5_ = new Sprite(5,{0,0},{64,64},{1.0f,1.0f,1.0f,1.0f},{0,0},0,0);
	num6_ = new Sprite(6,{0,0},{64,64},{1.0f,1.0f,1.0f,1.0f},{0,0},0,0);
	num7_ = new Sprite(7,{0,0},{64,64},{1.0f,1.0f,1.0f,1.0f},{0,0},0,0);
	num8_ = new Sprite(8,{0,0},{64,64},{1.0f,1.0f,1.0f,1.0f},{0,0},0,0);
	num9_ = new Sprite(9,{0,0},{64,64},{1.0f,1.0f,1.0f,1.0f},{0,0},0,0);
	//num10_ = new Sprite(0,{0,0},{64,64},{1.0f,1.0f,1.0f,1.0f},{0,0},0,0);

	num0_->LoadTexture(0, L"Resources/0.png");
	num1_->LoadTexture(1, L"Resources/1.png");
	num2_->LoadTexture(2, L"Resources/2.png");
	num3_->LoadTexture(3, L"Resources/3.png");
	num4_->LoadTexture(4, L"Resources/4.png");
	num5_->LoadTexture(5, L"Resources/5.png");
	num6_->LoadTexture(6, L"Resources/6.png");
	num7_->LoadTexture(7, L"Resources/7.png");
	num8_->LoadTexture(8, L"Resources/8.png");
	num9_->LoadTexture(9, L"Resources/9.png");
	//num10_->LoadTexture(10, L"Resources/10.png");

	num0_->Initialize();
	num1_->Initialize();
	num2_->Initialize();
	num3_->Initialize();
	num4_->Initialize();
	num5_->Initialize();
	num6_->Initialize();
	num7_->Initialize();
	num8_->Initialize();
	num9_->Initialize();
	//num10_->Initialize();

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

	viewProjection_.UpdateView();
	//シーン管理
	enemy->Update();
}

void GameScene::Draw() {
	//3D描画
	enemy->Draw();

	//スプライト描画
	Sprite::PreDraw(dx12base_.GetCmdList().Get());

	if (offset == 0) {
		num0_->Draw();
	}
	else if (offset == 1) {
		num1_->Draw();
	}
	else if (offset == 2) {
		num2_->Draw();
	}
	else if (offset == 3) {
		num3_->Draw();
	}
	else if (offset == 4) {
		num4_->Draw();
	}
	else if (offset == 5) {
		num5_->Draw();
	}
	else if (offset == 6) {
		num6_->Draw();
	}
	else if (offset == 7) {
		num7_->Draw();
	}
	else if (offset == 8) {
		num8_->Draw();
	}
	else if (offset == 9) {
		num9_->Draw();
	}

	Sprite::PostDraw();

}

void GameScene::Reset() {


}