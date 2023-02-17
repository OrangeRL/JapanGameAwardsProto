#include "PlayerBullet.h"

PlayerBullet::PlayerBullet() {
	//初期化以前の設定
	for (int i = 0; i < bulletNum; i++) {
		gameObject[i] = nullptr;
		isShot[i] = false;
	}
}

PlayerBullet::~PlayerBullet() {
	delete gameObject;
}

void PlayerBullet::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection) {
	for (int i = 0; i < bulletNum; i++) {
		gameObject[i] = new GameObject3D();
		gameObject[i]->PreLoadModel("Resources/tofu/tofu.obj");
		gameObject[i]->SetViewProjection(viewProjection);
		gameObject[i]->SetMatProjection(matProjection);
		gameObject[i]->Initialize();
		gameObject[i]->worldTransform.scale = Vector3{ 1,1,1 };
		gameObject[i]->Update();
	}
}

void PlayerBullet::Update(Vector3 playerPos, Vector3 bossPos) {
	Attack(playerPos, bossPos);
	for (int i = 0; i < bulletNum; i++) {
		if (isShot[i]) {
			gameObject[i]->Update();
		}
	}
}

void PlayerBullet::Draw() {
	for (int i = 0; i < bulletNum; i++) {
		if (isShot[i]) {
			gameObject[i]->Draw();
		}
	}
}

void PlayerBullet::Attack(Vector3 playerPos, Vector3 bossPos) {
	if (bulletCount > bulletNum) {
		//カウントが球数の最大値をオーバーしていたらリセット
		bulletCount = 0;
	}
	if (!isShot[bulletCount]) {
		//その番号の弾の発射フラグがfalseならtrueにする
		isShot[bulletCount] = true;
		//それと同時に同番号のオブジェクトの座標をボスの座標に持っていく
		gameObject[bulletCount]->worldTransform.translation = bossPos;
		//ボスと自機の差分ベクトルを求める
		velocity[bulletCount] = playerPos - bossPos;
		//ベクトルの正規化
		velocity[bulletCount].nomalize();
		//ベクトルの長さを速さに合わせる
		velocity[bulletCount].x *= speed;
		velocity[bulletCount].y *= speed;
		velocity[bulletCount].z *= speed;
	}
	for (int i = 0; i < bulletNum; i++) {
		if (isShot[i]) {
			//ボスと自機の差分ベクトルを求める
			velocity[i] = playerPos - bossPos;
			//ベクトルの正規化
			velocity[i].nomalize();
			//ベクトルの長さを速さに合わせる
			velocity[i].x *= speed;
			velocity[i].y *= speed;
			velocity[i].z *= speed;
			//発射フラグがtrueならその時点での自機の座標に向かって移動する
			gameObject[i]->worldTransform.translation -= velocity[i];
			if (gameObject[i]->worldTransform.translation.x < -canMoveArea ||
				gameObject[i]->worldTransform.translation.x > canMoveArea ||
				gameObject[i]->worldTransform.translation.y < -canMoveArea ||
				gameObject[i]->worldTransform.translation.y > canMoveArea ||
				gameObject[i]->worldTransform.translation.z < -canMoveArea ||
				gameObject[i]->worldTransform.translation.z > canMoveArea) {
				//一定の範囲外で消滅
				isShot[i] = false;
			}
		}
	}
	bulletCount++;
}