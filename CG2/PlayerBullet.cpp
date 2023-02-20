#include "PlayerBullet.h"

PlayerBullet::PlayerBullet() {
	//初期化以前の設定

	gameObject = nullptr;
	isShot = false;
	isDead_ = false;

}

PlayerBullet::~PlayerBullet() {
	delete gameObject;
}

void PlayerBullet::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection) {

	gameObject = new GameObject3D();
	gameObject->PreLoadModel("Resources/tofu/tofu.obj");
	gameObject->SetViewProjection(viewProjection);
	gameObject->SetMatProjection(matProjection);
	gameObject->Initialize();
	gameObject->worldTransform.scale = Vector3{ 1,1,1 };
	gameObject->Update();

}

void PlayerBullet::Update(Vector3 playerPos, Vector3 bossPos) {
	Attack(playerPos, bossPos);

	if (isShot) {
		gameObject->Update();
	}

}

void PlayerBullet::Draw() {

	if (isShot) {
		gameObject->Draw();

	}
}

void PlayerBullet::Attack(Vector3 playerPos, Vector3 bossPos) {

	if (!isShot) {
		//その番号の弾の発射フラグがfalseならtrueにする
		isShot = true;
		isDead_ = false;
		//それと同時に同番号のオブジェクトの座標をボスの座標に持っていく
		gameObject->worldTransform.translation = bossPos;
		//ボスと自機の差分ベクトルを求める
		velocity = playerPos - bossPos;
		//ベクトルの正規化
		velocity.nomalize();
		//ベクトルの長さを速さに合わせる
		velocity.x *= speed;
		velocity.y *= speed;
		velocity.z *= speed;
	}

	if (isShot) {
		//ボスと自機の差分ベクトルを求める
		velocity = playerPos - bossPos;
		//ベクトルの正規化
		velocity.nomalize();
		//ベクトルの長さを速さに合わせる
		velocity.x *= speed;
		velocity.y *= speed;
		velocity.z *= speed;
		//発射フラグがtrueならその時点での自機の座標に向かって移動する
		gameObject->worldTransform.translation -= velocity;
		if (gameObject->worldTransform.translation.x < -canMoveArea ||
			gameObject->worldTransform.translation.x > canMoveArea ||
			gameObject->worldTransform.translation.y < -canMoveArea ||
			gameObject->worldTransform.translation.y > canMoveArea ||
			gameObject->worldTransform.translation.z < -canMoveArea ||
			gameObject->worldTransform.translation.z > canMoveArea) {
			//一定の範囲外で消滅
			isShot = false;
			isDead_ = true;

		}
	}
	if (bulletCount < 10) {
		bulletCount++;
	}
}