#include "Rhythm.h"

Rhythm::~Rhythm() {
	SoundUnload(guideSound1);
	SoundUnload(guideSound2);
	SoundUnload(guideSound3);
	SoundUnload(shotSound);
	SoundUnload(laserSound);
	SoundUnload(explosionSound);
	SoundUnload(missSound);
	SoundUnload(mainBGM);
	delete soundManager_;
}

void Rhythm::Initialize() {
	soundManager_ = new SoundManager();
	soundManager_->Initialize();
}

void Rhythm::Update(Input* input) {

	//武器変更
	if (input->TriggerKey(DIK_0)) {
		weapon = Weapons::Normal;
	}
	else if (input->TriggerKey(DIK_1)) {
		weapon = Weapons::Rapid;
	}
	else if (input->TriggerKey(DIK_2)) {
		weapon = Weapons::Explosion;
	}
	else if (input->TriggerKey(DIK_3)) {
		weapon = Weapons::ThreeWay;
	}
	else if (input->TriggerKey(DIK_4)) {
		weapon = Weapons::Laser;
	}

	if (isPause == false) {
		//オフセット設定
		if (input->TriggerKey(DIK_A) && offset > -5.0f) {
			offset--;
		}
		else if (input->TriggerKey(DIK_D) && offset < 5.0f) {
			offset++;
		}

		//タイマーが一周したらレーザー発射可能に
		if (timer == 30.0f) {
			if (isLaserActive == 0) {
				isLaserActive = 1;
			}
			else {
				isLaserActive = 0;
			}
		}

		//通常弾のリズム
		if (weapon == Weapons::Normal) {
			shotTiming1 = 30.0f;	//弾発射タイミング1
			reloadTiming = 15.0f;	//弾が装填されるタイミング

			//一定のタイミングで音を鳴らす
			if (timer == 0.0f || timer == shotTiming1) {
				SoundPlayWave(guideSound2, guideSEVolume);
			}
			//一定のタイミングで弾を装填する
			if (timer == reloadTiming || timer == shotTiming1 + reloadTiming) {
				isActive = 1;
			}

			//弾が発射可能なとき
			if (isActive == 1) {
				if (input->TriggerKey(DIK_SPACE)) {
					//丁度いいタイミングでキーを押すと発射
					if (((timer <= judgeBreadth + offset) ||
						(timer >= shotTiming1 - judgeBreadth + offset && timer <= shotTiming1 + judgeBreadth + offset) ||
						(timer >= maxTimer - judgeBreadth + offset))) {
						SoundPlayWave(shotSound, normalSEVolume);
					}
					//それ以外のタイミングは失敗
					else {
						SoundPlayWave(missSound, normalSEVolume);
					}

					isActive = 0;
				}
			}
		}
		//速射弾のリズム
		else if (weapon == Weapons::Rapid) {
			shotTiming1 = 15.0f;	//弾発射タイミング1
			shotTiming2 = 30.0f;	//弾発射タイミング2
			shotTiming3 = 45.0f;	//弾発射タイミング3
			reloadTiming = 7.0f;	//弾が装填されるタイミング

			//一定のタイミングで音を鳴らす
			if (timer == 0.0f || timer == shotTiming1 || timer == shotTiming2 || timer == shotTiming3) {
				SoundPlayWave(guideSound2, guideSEVolume);
			}
			//一定のタイミングで弾を装填する
			if (timer == reloadTiming || timer == shotTiming1 + reloadTiming ||
				timer == shotTiming2 + reloadTiming || timer == shotTiming3 + reloadTiming) {
				isActive = 1;
			}

			//弾が発射可能なとき
			if (isActive == 1) {
				if (input->TriggerKey(DIK_SPACE)) {
					//丁度いいタイミングでキーを押すと発射
					if (((timer <= judgeBreadth + offset) ||
						(timer >= shotTiming1 - judgeBreadth + offset && timer <= shotTiming1 + judgeBreadth + offset) ||
						(timer >= shotTiming2 - judgeBreadth + offset && timer <= shotTiming2 + judgeBreadth + offset) ||
						(timer >= shotTiming3 - judgeBreadth + offset && timer <= shotTiming3 + judgeBreadth + offset) ||
						(timer >= maxTimer - judgeBreadth + offset))) {
						SoundPlayWave(shotSound, normalSEVolume);
					}
					//それ以外のタイミングは失敗
					else {
						SoundPlayWave(missSound, normalSEVolume);
					}

					isActive = 0;
				}
			}
		}

		//爆裂弾のリズム
		else if (weapon == Weapons::Explosion) {
			shotTiming1 = 30.0f;	//弾発射タイミング1(爆破タイミング)
			reloadTiming = 45.0f;	//弾が装填されるタイミング

			//一定のタイミングで音を鳴らす
			if (timer == 0.0f) {
				SoundPlayWave(guideSound2, guideSEVolume);
			}
			//一定のタイミングで弾を装填する
			if (timer == reloadTiming) {
				isSuccess = 0;
				isActive = 1;
			}

			//弾が発射可能なとき
			if (isActive == 1) {
				if (input->TriggerKey(DIK_SPACE)) {
					//丁度いいタイミングでキーを押すと発射
					if (((timer <= judgeBreadth + offset) ||
						(timer >= maxTimer - judgeBreadth + offset))) {
						SoundPlayWave(shotSound, normalSEVolume);
						isSuccess = 1;
					}
					//それ以外のタイミングは失敗
					else {
						SoundPlayWave(missSound, normalSEVolume);
						isSuccess = 0;
					}

					isActive = 0;
				}
			}
			//成功したら爆破音を鳴らす
			if (isSuccess == 1 && timer == shotTiming1) {
				SoundPlayWave(explosionSound, normalSEVolume);
			}
		}

		//3way弾のリズム
		else if (weapon == Weapons::ThreeWay) {
			shotTiming1 = 20.0f;	//弾発射タイミング1
			shotTiming2 = 40.0f;	//弾発射タイミング2
			reloadTiming = 10.0f;	//弾が装填されるタイミング

			//一定のタイミングで音を鳴らす
			if (timer == 0.0f || timer == shotTiming1 || timer == shotTiming2) {
				SoundPlayWave(guideSound2, guideSEVolume);

			}
			//一定のタイミングで弾を装填する
			if (timer == reloadTiming || timer == shotTiming1 + reloadTiming || timer == shotTiming2 + reloadTiming) {
				isActive = 1;
			}

			//弾が発射可能なとき
			if (isActive == 1) {
				if (input->TriggerKey(DIK_SPACE)) {
					//丁度いいタイミングでキーを押すと発射
					if (((timer <= judgeBreadth + offset) ||
						(timer >= shotTiming1 - judgeBreadth + offset && timer <= shotTiming1 + judgeBreadth + offset) ||
						(timer >= shotTiming2 - judgeBreadth + offset && timer <= shotTiming2 + judgeBreadth + offset) ||
						(timer >= maxTimer - judgeBreadth + offset))) {
						SoundPlayWave(shotSound, normalSEVolume);
					}
					//それ以外のタイミングは失敗
					else {
						SoundPlayWave(missSound, normalSEVolume);
					}

					isActive = 0;

				}
			}
		}

		//レーザー弾のリズム
		else if (weapon == Weapons::Laser) {
			reloadTiming = 0.0f;	//弾が装填されるタイミング

			//一定のタイミングで音を鳴らす
			if (timer == 0.0f && isLaserActive == 1) {
				SoundPlayWave(guideSound3, guideSEVolume);
			}
			//一定のタイミングで弾を装填する
			if (timer == reloadTiming && isLaserActive == 0) {
				isActive = 1;
			}

			//弾が発射可能なとき
			if (isActive == 1) {
				if (input->TriggerKey(DIK_SPACE)) {
					//丁度いいタイミングでキーを押すと発射
					if (((timer <= judgeBreadth + offset && isLaserActive == 1) ||
						(timer >= maxTimer - judgeBreadth + offset && isLaserActive == 1))) {
						soundManager_->SoundPlayWave(soundManager_->xAudio2.Get(), laserSound, false, normalSEVolume);

					}
					//それ以外のタイミングは失敗
					else {
						SoundPlayWave(missSound, normalSEVolume);
					}

					isActive = 0;
				}
			}

			//ボタンを離すと止まる
			if (input->PushKey(DIK_SPACE) == 0 && laserSound.pSourceVoice != nullptr) {
				laserSound.pSourceVoice->Stop(0);
			}
		}

		//曲が一周したら最初から再生(仮)
		if (measureCount == 0 && timer == 0) {
			//SoundPlayWave(mainBGM);
			soundManager_->SoundPlayWave(soundManager_->xAudio2.Get(), mainBGM, false, BGMVolume);
		}

		//目安として2小節目で大きめの音を鳴らす
		if (timer == maxTimer / 2) {
			SoundPlayWave(guideSound1, BGMVolume);
		}

		//タイマーを増やす
		timer++;

		//タイマーが最大までいったら0にし、小節を1進める
		if (timer >= maxTimer) {
			timer = 0;
			measureCount++;
		}

		//小節が一定までいったら曲を最初からにする(仮)
		if (measureCount >= 16) {
			measureCount = 0;
		}
	}

	//一時停止
	if (input->TriggerKey(DIK_P)) {

		if (isPause == 0) {
			isPause = 1;
			mainBGM.pSourceVoice->Stop(0);
		}
		else {
			isPause = 0;
			mainBGM.pSourceVoice->Start();

		}
	}

	//音量調節
	//BGM
	if (input->PushKey(DIK_UP) && BGMVolume < 2.0f) {
		BGMVolume += 0.01;
	}
	if (input->PushKey(DIK_DOWN) && BGMVolume >= 0.0f) {
		BGMVolume -= 0.01;
		if (BGMVolume <= 0.0f) {
			BGMVolume = 0;
		}
	}
	//SE
	if (input->PushKey(DIK_RIGHT) && guideSEVolume < 2.0f) {
		guideSEVolume += 0.01;
	}
	if (input->PushKey(DIK_LEFT) && guideSEVolume >= 0.0f) {
		guideSEVolume -= 0.01;
		if (guideSEVolume <= 0.0f) {
			guideSEVolume = 0;
		}
	}

	mainBGM.pSourceVoice->SetVolume(BGMVolume);
}

void Rhythm::SoundPlayWave(SoundData soundData, float volume) {
	soundManager_->StopWave(soundData);
	soundManager_->SoundPlayWave(soundManager_->xAudio2.Get(), soundData, false, volume);
}

void Rhythm::SoundUnload(SoundData soundData) {
	soundManager_->StopWave(soundData);
	soundManager_->SoundUnload(soundData);
}