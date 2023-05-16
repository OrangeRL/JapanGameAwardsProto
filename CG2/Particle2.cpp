#include "Particle2.h"
#include <random>

//�����V�[�h������
std::random_device seed_gen3;
//�����Z���k�E�c�C�X�^�[�̗����G���W��
std::mt19937_64 engine3(seed_gen3());
//�����͈͂̎w��
std::uniform_real_distribution<float> distRot3(MathFunc::Utility::Deg2Rad(-60), MathFunc::Utility::Deg2Rad(60));

Particle2::Particle2() {

}

Particle2::~Particle2() {
	for (int i = 0; i < particleValue; i++) {
		delete gameObject[i];
	}
}

void Particle2::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection, const wchar_t* textureFileName,Vector3 position) {
	soundManager_.Initialize();
	for (int i = 0; i < particleValue; i++) {

		gameObject[i] = new GameObject3D;
		gameObject[i]->SetViewProjection(viewProjection);
		gameObject[i]->SetMatProjection(matProjection);
		gameObject[i]->PreLoadTexture(textureFileName);
		gameObject[i]->Initialize();

		isDead[i] = true;

		Reset(position);

	}

}

void Particle2::Update(Vector3 initPos) {
	for (int i = 0; i < particleValue; i++) {
		if (particleOff[i] == true) {

			gameObject[i]->worldTransform.translation = initPos + velocity[i];

			velocity[i] = {
				speed * cosf(1.8) ,
				0 ,
				speed * -sinf(1.8)
			};
			gameObject[i]->worldTransform.rotation = { distRot3(engine3) , distRot3(engine3) , distRot3(engine3) };

			MathFunc::Affine::SetMatRotation(gameObject[i]->worldTransform.matWorld, gameObject[i]->worldTransform.rotation);

			velocity[i] = MathFunc::Utility::MulVector3AndMatrix4(velocity[i], gameObject[i]->worldTransform.matWorld);

			particleOff[i] = false;
			isDead[i] = false;
			timer[i] = 60;


			spawnPos[i] = initPos + velocity[i] * 40;
			if (i > 20)
			{
				spawnPos[i] = initPos + velocity[i] * 30;
			}

			enemyPos[i] = initPos;
			gameObject[i]->worldTransform.translation = spawnPos[i];
			//�{�X�Ǝ��@�̍����x�N�g�������߂�
			velocity[i] = enemyPos[i] - spawnPos[i];

			//�x�N�g���̐��K��
			velocity[i].nomalize();
			//�x�N�g���̒����𑬂��ɍ��킹��
			velocity[i].x *= speed;
			velocity[i].y *= speed;
			velocity[i].z *= speed;

		}
		else {
			//�{�X�Ǝ��@�̍����x�N�g�������߂�
			velocity[i] = enemyPos[i] - spawnPos[i];
			//velocity[i] = { 0.0f,0.0f,5.0f };

			//�x�N�g���̐��K��
			velocity[i].nomalize();
			//�x�N�g���̒����𑬂��ɍ��킹��
			velocity[i].x *= speed;
			velocity[i].y *= speed;
			velocity[i].z *= speed;
			//���˃t���O��true�Ȃ炻�̎��_�ł̎��@�̍��W�Ɍ������Ĉړ�����
			gameObject[i]->worldTransform.translation += velocity[i];

			gameObject[i]->worldTransform.scale = { 1.5f * timer[i] / 80 , 1.5f * timer[i] / 80 , 1.5f * timer[i] / 80 };
			gameObject[i]->worldTransform.rotation = { 0.5f * timer[i] / 60, 0.5f * timer[i] / 60, 0.5f * timer[i] / 60 };

			speed2 = -0.2f;
			posA = initPos + velocity[i] * 40;
			posB = initPos;
			posC = posA - posB;
			posC.nomalize();
			posC *= speed2;

			gameObject[i]->worldTransform.translation -= posC;

			gameObject[i]->Update();
			timer[i]--;
			if (timer[i] < 20) {
				isDead[i] = true;

			}
		}
	}
}



void Particle2::Draw() {

	for (int i = 0; i < particleValue; i++) {
		if (timer[i] > 0 && particleOff[i] == false) {

			gameObject[i]->Draw();

		}
	}

}

void Particle2::Reset(Vector3 position) {
	for (int i = 0; i < particleValue; i++) {
		isDead[i] = false;
		timer[i] = 60;
		particleOff[i] = true;
		isPlayingBGM = false;
		speed2 = -0.2f;
		velocity[i] = {
				speed * cosf(1.8) ,
				0 ,
				speed * -sinf(1.8)
		};
		spawnPos[i] = position;
	}
}

int Particle2::GetIsDead() {
	return isDead[0];
}