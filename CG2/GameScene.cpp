#include "GameScene.h"
#include <cassert>


GameScene::GameScene() {

}

GameScene::~GameScene() {
	
	soundManager_.StopWave(soundData1);
	soundManager_.StopWave(soundData2);
	soundManager_.StopWave(soundData3);
	soundManager_.SoundUnload(soundData1);
	soundManager_.SoundUnload(soundData2);
	soundManager_.SoundUnload(soundData3);
}

void GameScene::Initialize(WinApp* winApp) {

	//透視投影変換行列の計算
	matProjection_ = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0) ,
		(float)winApp->window_width / winApp->window_height ,
		0.1f , 1000.0f
	);

	viewProjection_.Initialize();
	viewProjection_.eye = {0 , 100 , -100};

	enemy = new Enemy();
	enemy->Initialize(&viewProjection_, &matProjection_);

	soundManager_.Initialize();
	soundManager_.SoundPlayWave(soundManager_.xAudio2.Get(), mainBGM, false, 1.0f);
}

void GameScene::Update() {
	if (measure == 0 && timer == 0) {
		soundManager_.StopWave(mainBGM);
		soundManager_.SoundPlayWave(soundManager_.xAudio2.Get(), mainBGM, false, 1.0f);
	}

	timer++;

	if (timer >= 60.0f) {
		timer = 0;
		measure++;
	}

	if (measure >= 16) {
		measure = 0;
	}

	if (timer == 0) {
		soundManager_.StopWave(soundData1);
		soundManager_.SoundPlayWave(soundManager_.xAudio2.Get(), soundData1, false, 1.0f);
	
	}

	if (input_.TriggerKey(DIK_0)) {
		weapon = 0;
	}
	else if (input_.TriggerKey(DIK_1)) {
		weapon = 1;
	}
	else if (input_.TriggerKey(DIK_2)) {
		weapon = 2;
	}

	if (input_.TriggerKey(DIK_A)) {
		soundManager_.StopWave(mainBGM);
		soundManager_.SoundPlayWave(soundManager_.xAudio2.Get(), mainBGM, false, 1.0f);

	}

	if (weapon == 0) {
		if (timer == 0.0f || timer == 30.0f) {
			soundManager_.StopWave(soundData2);
			soundManager_.SoundPlayWave(soundManager_.xAudio2.Get(), soundData2, false, 1.0f);
		}
		if (timer == 15.0f || timer == 45.0f) {
			isActive = 1;
		}

		if (isActive == 1) {
			if (input_.TriggerKey(DIK_SPACE)) {
				if (((timer <= 4.0f) || (timer >= 26.0f && timer <= 34.0f) || (timer >= 56.0f))) {
					soundManager_.StopWave(soundData3);
					soundManager_.SoundPlayWave(soundManager_.xAudio2.Get(), soundData3, false, 1.0f);
				}
				else {
					soundManager_.StopWave(soundData4);
					soundManager_.SoundPlayWave(soundManager_.xAudio2.Get(), soundData4, false, 1.0f);
				}

				isActive = 0;
				//timer -= 1.0f;
			}
		}
	}

	else if (weapon == 1) {
		if (timer == 0.0f || timer == 15.0f || timer == 30.0f || timer == 45.0f) {
			soundManager_.StopWave(soundData2);
			soundManager_.SoundPlayWave(soundManager_.xAudio2.Get(), soundData2, false, 1.0f);

		}
		if (timer == 7.0f || timer == 22.0f || timer == 37.0f || timer == 52.0f) {
			isActive = 1;
		}

		if (isActive == 1) {
			if (input_.TriggerKey(DIK_SPACE)) {

				if (((timer <= 4.0f) || (timer >= 11.0f && timer <= 19.0f) || (timer >= 26.0f && timer <= 34.0f) || (timer >= 41.0f && timer <= 49.0f) || (timer >= 56.0f))) {
					soundManager_.StopWave(soundData3);
					soundManager_.SoundPlayWave(soundManager_.xAudio2.Get(), soundData3, false, 1.0f);
				}
				else {
					soundManager_.StopWave(soundData4);
					soundManager_.SoundPlayWave(soundManager_.xAudio2.Get(), soundData4, false, 1.0f);
				}

				isActive = 0;
				//timer += 1;
			}
		}
	}

	else if (weapon == 2) {
		if (timer == 0.0f || timer == 20.0f || timer == 40.0f) {
			soundManager_.StopWave(soundData2);
			soundManager_.SoundPlayWave(soundManager_.xAudio2.Get(), soundData2, false, 1.0f);

		}
		if (timer == 10.0f || timer == 30.0f || timer == 50.0f) {
			isActive = 1;
		}

		if (isActive == 1) {
			if (input_.TriggerKey(DIK_SPACE)) {
				if (((timer <= 4.0f) || (timer >= 16.0f && timer <= 24.0f) || (timer >= 36.0f && timer <= 44.0f) || (timer >= 56.0f))) {
					soundManager_.StopWave(soundData3);
					soundManager_.SoundPlayWave(soundManager_.xAudio2.Get(), soundData3, false, 1.0f);
				}
				else {
					soundManager_.StopWave(soundData4);
					soundManager_.SoundPlayWave(soundManager_.xAudio2.Get(), soundData4, false, 1.0f);
				}

				isActive = 0;

			}
		}
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


	Sprite::PostDraw();

}

void GameScene::Reset() {


}