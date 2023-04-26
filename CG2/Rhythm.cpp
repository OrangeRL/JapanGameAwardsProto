#include "Rhythm.h"

Rhythm::~Rhythm() {
	SoundUnload(guideSound1);
	SoundUnload(guideSound2);
	SoundUnload(guideSound3);
	SoundUnload(shotSound);
	SoundUnload(laserSound);
	SoundUnload(explosionSound);
	SoundUnload(itemSound);
	SoundUnload(missSound);
	SoundUnload(damageSound);
	SoundUnload(knockSound);
	SoundUnload(scoreRiseSound);
	SoundUnload(countdownSound);
	SoundUnload(decisionSound);
	SoundUnload(selectSound);

	SoundUnload(titleBGM);
	SoundUnload(demoBGM);
	SoundUnload(stage1_1BGM);
	SoundUnload(stage1_2BGM);
	SoundUnload(stage1_bossBGM);
	delete soundManager_;
	for (int i = 0; i < circleNum; i++) {
		delete circle[i];
	}
}

void Rhythm::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection) {

	soundManager_ = new SoundManager();
	soundManager_->Initialize();
	soundState.measureCount = 0;

	for (int i = 0; i < circleNum; i++) {
		circle[i] = new GameObject3D();
		circle[i]->PreLoadModel("Resources/circle/circle.obj");
		circle[i]->PreLoadTexture(L"Resources/white1x1.png");
		circle[i]->SetViewProjection(viewProjection);
		circle[i]->SetMatProjection(matProjection);
		circle[i]->Initialize();

	}
	circle[0]->worldTransform.scale = { 2.0f,2.0f,1.0f };
	circle[1]->worldTransform.scale = { 5.0f,5.0f,0.0f };

	pitch = 1.0003f;
	ResetRhythm();
}

void Rhythm::ResetRhythm() {
	soundState.measureCount = 0;
	soundState.timer = 0.0f;
	soundState.wave = 1;
	soundState.isFireActive = 0;
	soundState.combo = 0;
	soundState.isPause = 0;
	soundState.weapon = Weapons::Normal;
	soundState.isFireSucces;
	soundState.isLaserActive = 0;
	soundState.judge = Judge::None;
	circle[1]->worldTransform.scale = { 5.0f,5.0f,0.0f };
	circle[1]->color.z = 1.0f;
}

void Rhythm::Update(Input* input, Vector3 pos, Vector3 rot, int isDead, int stage, Scene scene, int select) {

	if (scene == Scene::Title) {

		soundState.weapon = Weapons::Normal;

		if (soundState.measureCount == 64) {
			soundState.measureCount = 0;
		}

		//�ŏ��̉����Đ�����
		if (soundState.timer == 0 && soundState.measureCount <= 0) {
			soundManager_->SoundPlayWave(soundManager_->xAudio2.Get(), titleBGM, false, soundState.BGMVolume);
		}

		//UI�̉~�����X�ɏ���������
		shrinkSpeed = { 0.1f,0.1f,0.0f };
		circle[1]->color.z -= 0.05f;
		circle[1]->worldTransform.scale -= shrinkSpeed;

		for (int i = 0; i < circleNum; i++) {
			circle[i]->worldTransform.translation = pos;
			circle[i]->worldTransform.rotation = rot;
			circle[i]->Update();
		}

		if (select == 3) {
			//UI(�~)�̍X�V
			if (input->TriggerKey(DIK_SPACE) && soundState.isFireActive == 1) {
				colorChange = 0.0f;
			}
			colorChange += 0.05f;

			NormalShot(soundState,input);
			if (soundState.judge == Judge::Good) {
				circle[0]->color = { 1.0f,1.0f,colorChange,0.0f };
			}
			else if (soundState.judge == Judge::Miss) {
				circle[0]->color = { 1.0f,colorChange,colorChange,0.0f };
			}
		}

		//�^�C�}�[�𑝂₷
		soundState.timer++;

		//�^�C�}�[���ő�܂ł�������0�ɂ��A���߂�1�i�߂�
		if (soundState.timer >= maxTimer) {
			soundState.timer = 0;
			soundState.measureCount++;
		}
		if (soundState.timer == 0.0f || soundState.timer == 30.0f) {
			circle[1]->worldTransform.scale = { 5.0f,5.0f,0.0f };
			circle[1]->color.z = 1.0f;
		}

		if (select == 5) {
			if (titleBGM.pSourceVoice) {
				titleBGM.pSourceVoice->Stop(0);
			}
			soundState.wave = 1;
			soundState.measureCount = 0;
			soundState.timer = 0;
		}

		pitch = 1.0003f;
		if (titleBGM.pSourceVoice) {
			titleBGM.pSourceVoice->SetVolume(soundState.BGMVolume);
			titleBGM.pSourceVoice->SetFrequencyRatio(pitch);
		}

	}
	else if (scene == Scene::Stage) {

		if (stage == 1) {
			if (soundState.timer == 0 && soundState.measureCount <= 0) {
				if (soundState.wave == 1) {
					soundManager_->SoundPlayWave(soundManager_->xAudio2.Get(), stage1_1BGM, false, soundState.BGMVolume);
				}
				else if (soundState.wave == 2) {
					soundManager_->SoundPlayWave(soundManager_->xAudio2.Get(), stage1_2BGM, false, soundState.BGMVolume);
				}
			}

			if (soundState.wave == 3 && soundState.timer == 0 && soundState.measureCount % 72 == 0) {
				soundManager_->SoundPlayWave(soundManager_->xAudio2.Get(), stage1_bossBGM, false, soundState.BGMVolume);
			}
		}
		else if (stage == 2) {

		}
		else if (stage == 3) {

		}

		if (soundState.timer == 0 && soundState.measureCount == 4) {
			soundManager_->SoundPlayWave(soundManager_->xAudio2.Get(), countdownSound, false, soundState.BGMVolume);
		}


		if (isDead == 0) {

			pitch = 1.0003f;

			//����ύX
			if (input->TriggerKey(DIK_0)) {
				soundState.weapon = Weapons::Normal;
			}
			else if (input->TriggerKey(DIK_1)) {
				soundState.weapon = Weapons::Rapid;
			}
			else if (input->TriggerKey(DIK_2)) {
				soundState.weapon = Weapons::ThreeWay;
			}
			else if (input->TriggerKey(DIK_3)) {
				soundState.weapon = Weapons::Explosion;
			}
			else if (input->TriggerKey(DIK_4)) {
				soundState.weapon = Weapons::Laser;
			}

			if (soundState.isPause == false) {
				if (soundState.measureCount >= 8 &&
					((soundState.measureCount < 72 && soundState.wave == 1) ||
						(soundState.measureCount < 80 && soundState.wave == 2) ||
						soundState.wave == 3)) {

					//�^�C�}�[����������烌�[�U�[���ˉ\��
					if (soundState.timer == 30.0f) {
						if (soundState.isLaserActive == 0) {
							soundState.isLaserActive = 1;
						}
						else {
							soundState.isLaserActive = 0;
						}
					}

					soundState.isFireSucces = 0;

					//�ʏ�e�̃��Y��
					if (soundState.weapon == Weapons::Normal) {
						NormalShot(soundState, input);
						shrinkSpeed = { 0.1f,0.1f,0.0f };
						circle[1]->color.z -= 0.05f;
					}
					//���˒e�̃��Y��
					else if (soundState.weapon == Weapons::Rapid) {
						RapidShot(soundState, input);
						shrinkSpeed = { 0.2f,0.2f,0.0f };
						circle[1]->color.z -= 0.1f;
					}
					//3way�e�̃��Y��
					else if (soundState.weapon == Weapons::ThreeWay) {
						ThreeWayShot(soundState, input);
						shrinkSpeed = { 0.15f,0.15f,0.0f };
						circle[1]->color.z -= 0.075f;
					}
					//�����e�̃��Y��
					else if (soundState.weapon == Weapons::Explosion) {
						shrinkSpeed = { 0.05f,0.05f,0.0f };
						circle[1]->color.z -= 0.025f;
						ExplosionShot(soundState, input);
					}
					//���[�U�[�e�̃��Y��
					else if (soundState.weapon == Weapons::Laser) {
						shrinkSpeed = { 0.0f,0.0f,0.0f };
						circle[1]->color.z -= 0.025f;
						LaserShot(soundState, input);
					}

					//UI�̉~�����X�ɏ���������
					circle[1]->worldTransform.scale -= shrinkSpeed;

				}

				//�^�C�}�[�𑝂₷
				soundState.timer++;

				//�^�C�}�[���ő�܂ł�������0�ɂ��A���߂�1�i�߂�
				if (soundState.timer >= maxTimer) {
					soundState.timer = 0;
					soundState.measureCount++;
				}

				//���߂����܂ł������烊�Z�b�g
				if ((soundState.wave == 1 && soundState.measureCount >= 80) ||
					(soundState.wave == 2 && soundState.measureCount >= 88)) {
					soundState.measureCount = 0;
					soundState.wave++;
					soundState.combo = 0;
				}
				/*else if (soundState.wave == 2 && soundState.measureCount >= 88) {
					soundState.measureCount = 0;
					soundState.wave++;
				}*/
			}

			//�ꎞ��~
			if (input->TriggerKey(DIK_P) && soundState.measureCount > 7) {

				if (soundState.isPause == 0) {
					soundState.isPause = 1;

					if (countdownSound.pSourceVoice) {
						countdownSound.pSourceVoice->Stop(0);
					}

					if (soundState.wave == 1) {
						stage1_1BGM.pSourceVoice->Stop(0);
					}
					else if (soundState.wave == 2) {
						stage1_2BGM.pSourceVoice->Stop(0);
					}
					else if (soundState.wave == 3) {
						stage1_bossBGM.pSourceVoice->Stop(0);
					}
				}
				else {
					soundState.isPause = 0;

					if (countdownSound.pSourceVoice) {
						countdownSound.pSourceVoice->Start();
					}

					if (soundState.wave == 1) {
						stage1_1BGM.pSourceVoice->Start();
					}
					else if (soundState.wave == 2) {
						stage1_2BGM.pSourceVoice->Start();
					}
					else if (soundState.wave == 3) {
						stage1_bossBGM.pSourceVoice->Start();
					}

				}
			}

			//UI(�~)�̍X�V
			if (input->TriggerKey(DIK_SPACE)) {
				colorChange = 0.0f;
			}

			colorChange += 0.05f;

			if (soundState.judge == Judge::Good) {
				circle[0]->color = { 1.0f,1.0f,colorChange,0.0f };
			}
			else if (soundState.judge == Judge::Miss) {
				circle[0]->color = { 1.0f,colorChange,colorChange,0.0f };
			}

			for (int i = 0; i < circleNum; i++) {
				circle[i]->worldTransform.translation = pos;
				circle[i]->worldTransform.rotation = rot;
				circle[i]->Update();
			}
		}
		else {
			//���S���A�s�b�`��i�X������
			pitch -= 0.01;
		}

		if (soundState.wave == 1) {
			if (stage1_1BGM.pSourceVoice) {
				stage1_1BGM.pSourceVoice->SetFrequencyRatio(pitch);
				stage1_1BGM.pSourceVoice->SetVolume(soundState.BGMVolume);
			}
		}
		else if (soundState.wave == 2) {
			if (stage1_2BGM.pSourceVoice) {
				stage1_2BGM.pSourceVoice->SetFrequencyRatio(pitch);
				stage1_2BGM.pSourceVoice->SetVolume(soundState.BGMVolume);
			}
		}
		else if (soundState.wave == 3) {
			if (stage1_bossBGM.pSourceVoice) {
				stage1_bossBGM.pSourceVoice->SetFrequencyRatio(pitch);
				stage1_bossBGM.pSourceVoice->SetVolume(soundState.BGMVolume);
			}
		}
	}
}

void Rhythm::Draw(int isDead) {
	if (isDead == 0) {
		for (int i = 0; i < circleNum; i++) {
			circle[i]->Draw();
		}
	}
}

void Rhythm::SoundPlayWave(SoundData soundData, float volume) {
	soundManager_->StopWave(soundData);
	soundManager_->SoundPlayWave(soundManager_->xAudio2.Get(), soundData, false, volume);
}

void Rhythm::SoundUnload(SoundData soundData) {
	soundManager_->StopWave(soundData);
	soundManager_->SoundUnload(soundData);
}

//�ʏ�e
void Rhythm::NormalShot(SoundState s, Input* input) {
	shotTiming1 = 30.0f;	//�e���˃^�C�~���O1
	reloadTiming = 15.0f;	//�e�����U�����^�C�~���O

	//���̃^�C�~���O�ŉ���炷
	if (s.timer == 0.0f || s.timer == shotTiming1) {
		circle[1]->worldTransform.scale = { 5.0f,5.0f,0.0f };
		circle[1]->color.z = 1.0f;
		SoundPlayWave(guideSound2, s.guideSEVolume);
	}
	//���̃^�C�~���O�Œe�𑕓U����
	if (s.timer == reloadTiming || s.timer == shotTiming1 + reloadTiming) {
		soundState.isFireActive = 1;
	}

	//�e�����ˉ\�ȂƂ�
	if (soundState.isFireActive == 1) {
		if (input->TriggerKey(DIK_SPACE)) {
			//���x�����^�C�~���O�ŃL�[�������Ɣ���
			if (((s.timer <= judgeBreadth + s.offset) ||
				(s.timer >= shotTiming1 - judgeBreadth + s.offset && s.timer <= shotTiming1 + judgeBreadth + s.offset) ||
				(s.timer >= maxTimer - judgeBreadth + s.offset))) {

				soundState.isFireSucces = 1;
				soundState.judge = Judge::Good;
				soundState.combo++;
				SoundPlayWave(shotSound, s.normalSEVolume);
			}
			//����ȊO�̃^�C�~���O�͎��s
			else {
				soundState.judge = Judge::Miss;
				soundState.combo = 0;
				SoundPlayWave(missSound, s.normalSEVolume);
			}

			soundState.isFireActive = 0;
		}
	}
}

//���˒e
void Rhythm::RapidShot(SoundState s, Input* input) {
	shotTiming1 = 15.0f;	//�e���˃^�C�~���O1
	shotTiming2 = 30.0f;	//�e���˃^�C�~���O2
	shotTiming3 = 45.0f;	//�e���˃^�C�~���O3
	reloadTiming = 7.0f;	//�e�����U�����^�C�~���O

	//���̃^�C�~���O�ŉ���炷
	if (s.timer == 0.0f || s.timer == shotTiming1 || s.timer == shotTiming2 || s.timer == shotTiming3) {
		SoundPlayWave(guideSound2, s.guideSEVolume);
		circle[1]->worldTransform.scale = { 5.0f,5.0f,0.0f };
		circle[1]->color.z = 1.0f;
	}
	//���̃^�C�~���O�Œe�𑕓U����
	if (s.timer == reloadTiming || s.timer == shotTiming1 + reloadTiming ||
		s.timer == shotTiming2 + reloadTiming || s.timer == shotTiming3 + reloadTiming) {
		soundState.isFireActive = 1;
	}

	//�e�����ˉ\�ȂƂ�
	if (soundState.isFireActive == 1) {
		if (input->TriggerKey(DIK_SPACE)) {
			//���x�����^�C�~���O�ŃL�[�������Ɣ���
			if (((s.timer <= judgeBreadth + s.offset) ||
				(s.timer >= shotTiming1 - judgeBreadth + s.offset && s.timer <= shotTiming1 + judgeBreadth + s.offset) ||
				(s.timer >= shotTiming2 - judgeBreadth + s.offset && s.timer <= shotTiming2 + judgeBreadth + s.offset) ||
				(s.timer >= shotTiming3 - judgeBreadth + s.offset && s.timer <= shotTiming3 + judgeBreadth + s.offset) ||
				(s.timer >= maxTimer - judgeBreadth + s.offset))) {

				soundState.isFireSucces = 1;
				soundState.judge = Judge::Good;
				SoundPlayWave(shotSound, s.normalSEVolume);
			}
			//����ȊO�̃^�C�~���O�͎��s
			else {
				soundState.judge = Judge::Miss;
				SoundPlayWave(missSound, s.normalSEVolume);
			}

			soundState.isFireActive = 0;
		}
	}
}

//3way�e
void Rhythm::ThreeWayShot(SoundState s, Input* input) {
	shotTiming1 = 20.0f;	//�e���˃^�C�~���O1
	shotTiming2 = 40.0f;	//�e���˃^�C�~���O2
	reloadTiming = 10.0f;	//�e�����U�����^�C�~���O

	//���̃^�C�~���O�ŉ���炷
	if (s.timer == 0.0f || s.timer == shotTiming1 || s.timer == shotTiming2) {
		circle[1]->worldTransform.scale = { 5.0f,5.0f,0.0f };
		circle[1]->color.z = 1.0f;
		SoundPlayWave(guideSound2, s.guideSEVolume);

	}
	//���̃^�C�~���O�Œe�𑕓U����
	if (s.timer == reloadTiming || s.timer == shotTiming1 + reloadTiming || s.timer == shotTiming2 + reloadTiming) {
		soundState.isFireActive = 1;
	}

	//�e�����ˉ\�ȂƂ�
	if (soundState.isFireActive == 1) {
		if (input->TriggerKey(DIK_SPACE)) {
			//���x�����^�C�~���O�ŃL�[�������Ɣ���
			if (((s.timer <= judgeBreadth + s.offset) ||
				(s.timer >= shotTiming1 - judgeBreadth + s.offset && s.timer <= shotTiming1 + judgeBreadth + s.offset) ||
				(s.timer >= shotTiming2 - judgeBreadth + s.offset && s.timer <= shotTiming2 + judgeBreadth + s.offset) ||
				(s.timer >= maxTimer - judgeBreadth + s.offset))) {

				soundState.isFireSucces = 1;
				soundState.judge = Judge::Good;
				SoundPlayWave(shotSound, s.normalSEVolume);
			}
			//����ȊO�̃^�C�~���O�͎��s
			else {
				soundState.judge = Judge::Miss;
				SoundPlayWave(missSound, s.normalSEVolume);
			}

			soundState.isFireActive = 0;

		}
	}
}

//�����e
void Rhythm::ExplosionShot(SoundState s, Input* input) {
	shotTiming1 = 30.0f;	//�e���˃^�C�~���O1(���j�^�C�~���O)
	reloadTiming = 45.0f;	//�e�����U�����^�C�~���O

	//���̃^�C�~���O�ŉ���炷
	if (s.timer == 0.0f) {
		circle[1]->worldTransform.scale = { 5.0f,5.0f,0.0f };
		circle[1]->color.z = 1.0f;
		SoundPlayWave(guideSound2, s.guideSEVolume);
	}
	//���̃^�C�~���O�Œe�𑕓U����
	if (s.timer == reloadTiming) {
		isSuccess = 0;
		soundState.isFireActive = 1;
	}

	//�e�����ˉ\�ȂƂ�
	if (soundState.isFireActive == 1) {
		if (input->TriggerKey(DIK_SPACE)) {
			//���x�����^�C�~���O�ŃL�[�������Ɣ���
			if (((s.timer <= judgeBreadth + s.offset) ||
				(s.timer >= maxTimer - judgeBreadth + s.offset))) {

				soundState.isFireSucces = 1;
				isSuccess = 1;
				soundState.judge = Judge::Good;
				SoundPlayWave(shotSound, s.normalSEVolume);
			}
			//����ȊO�̃^�C�~���O�͎��s
			else {
				soundState.judge = Judge::Miss;
				SoundPlayWave(missSound, s.normalSEVolume);
				isSuccess = 0;
			}

			soundState.isFireActive = 0;
		}
	}
	//���������甚�j����炷
	if (isSuccess == 1 && s.timer == shotTiming1) {
		SoundPlayWave(explosionSound, s.normalSEVolume);
	}
}

//���[�U�[�e
void Rhythm::LaserShot(SoundState s, Input* input) {
	reloadTiming = 0.0f;	//�e�����U�����^�C�~���O}

	//���̃^�C�~���O�ŉ���炷
	if (s.timer == 0.0f && s.isLaserActive == 1) {
		SoundPlayWave(guideSound3, s.guideSEVolume);
	}
	//���̃^�C�~���O�Œe�𑕓U����
	if (s.timer == reloadTiming && s.isLaserActive == 0) {
		circle[1]->worldTransform.scale = { 5.0f,5.0f,0.0f };
		circle[1]->color.z = 1.0f;
		soundState.isFireActive = 1;
	}

	//�e�����ˉ\�ȂƂ�
	if (soundState.isFireActive == 1) {
		if (input->TriggerKey(DIK_SPACE)) {
			//���x�����^�C�~���O�ŃL�[�������Ɣ���
			if (((s.timer <= judgeBreadth + s.offset && s.isLaserActive == 1) ||
				(s.timer >= maxTimer - judgeBreadth + s.offset && s.isLaserActive == 1))) {

				soundState.isFireSucces = 1;
				soundState.judge = Judge::Good;
				soundManager_->SoundPlayWave(soundManager_->xAudio2.Get(), laserSound, false, s.normalSEVolume);

			}
			//����ȊO�̃^�C�~���O�͎��s
			else {
				soundState.judge = Judge::Miss;
				SoundPlayWave(missSound, s.normalSEVolume);
			}

			soundState.isFireActive = 0;
		}
		shrinkSpeed = { 0.05f,0.05f,0.0f };

	}

	if (circle[1]->worldTransform.scale.x <= 0) {
		shrinkSpeed = { 0.0f,0.0f,0.0f };
	}
	//�{�^���𗣂��Ǝ~�܂�
	if (input->PushKey(DIK_SPACE) == 0 && laserSound.pSourceVoice != nullptr) {
		laserSound.pSourceVoice->Stop(0);
	}

}
