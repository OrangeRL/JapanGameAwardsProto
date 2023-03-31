#include "Particle2.h"
#include <random>

//乱数シード生成器
std::random_device seed_gen3;
//メルセンヌ・ツイスターの乱数エンジン
std::mt19937_64 engine3(seed_gen3());
//乱数範囲の指定
std::uniform_real_distribution<float> distRot3(MathFunc::Utility::Deg2Rad(-60), MathFunc::Utility::Deg2Rad(60));

Particle2::Particle2() {

}

Particle2::~Particle2() {
	for (int i = 0; i < particleValue; i++) {
		delete gameObject[i];
	}
}

void Particle2::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection, const wchar_t* textureFileName) {
	soundManager_.Initialize();
	for (int i = 0; i < particleValue; i++) {

		gameObject[i] = new GameObject3D;
		/*if (i > 20) {
			gameObject[i]->PreLoadTexture(L"Resources/red1x1.png");
		}*/
		gameObject[i]->PreLoadTexture(textureFileName);
		gameObject[i]->SetViewProjection(viewProjection);
		gameObject[i]->SetMatProjection(matProjection);
		gameObject[i]->Initialize();

		gameObject[i]->Update();

		isDead[i] = true;

		Reset();

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
			//ボスと自機の差分ベクトルを求める
			velocity[i] = enemyPos[i] - spawnPos[i];

			//ベクトルの正規化
			velocity[i].nomalize();
			//ベクトルの長さを速さに合わせる
			velocity[i].x *= speed;
			velocity[i].y *= speed;
			velocity[i].z *= speed;

		}
		else {
			//ボスと自機の差分ベクトルを求める
			velocity[i] = enemyPos[i] - spawnPos[i];
			//velocity[i] = { 0.0f,0.0f,5.0f };

			//ベクトルの正規化
			velocity[i].nomalize();
			//ベクトルの長さを速さに合わせる
			velocity[i].x *= speed;
			velocity[i].y *= speed;
			velocity[i].z *= speed;
			//発射フラグがtrueならその時点での自機の座標に向かって移動する
			gameObject[i]->worldTransform.translation += velocity[i];

			gameObject[i]->worldTransform.scale = { 1.5f * timer[i] / 80 , 1.5f * timer[i] / 80 , 1.5f * timer[i] / 80 };
			gameObject[i]->worldTransform.rotation = { 0.5f * timer[i] / 60, 0.5f * timer[i] / 60, 0.5f * timer[i] / 60 };

			float speed = -0.2f;
			posA = initPos + velocity[i] * 40;
			posB = initPos;
			posC = posA - posB;
			posC.nomalize();
			posC *= speed;

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

void Particle2::Reset() {
	for (int i = 0; i < particleValue; i++) {

		particleOff[i] = true;
		isPlayingBGM = false;
	}
}

int Particle2::GetIsDead() {
	return isDead[0];
}