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

void PlayerBullet::OnCollision() {
	isDead_ = true;
}

void PlayerBullet::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection, Vector3 playerPos, Vector3 bossPos) {

	gameObject = new GameObject3D();
	//gameObject->PreLoadModel("Resources/star/star.obj");
	gameObject->PreLoadTexture(L"Resources/red.png");
	gameObject->SetViewProjection(viewProjection);
	gameObject->SetMatProjection(matProjection);
	gameObject->Initialize();
	gameObject->worldTransform.scale = Vector3{ 1,1,1 };
	gameObject->Update();
	isDead_ = false;

	newPlayerPos = playerPos;
	newEnemyPos = bossPos;
}

void PlayerBullet::Update(Vector3 playerPos, Vector3 bossPos) {
	const float rotationSpeed = MathFunc::Utility::Deg2Rad(60.0f);

	Vector3 rotation = { 0 , 0 , 0 };
	/*newPlayerPos = playerPos;*/
	rotation.y = rotationSpeed;
	rotation.x = rotationSpeed;
	rotation.z = rotationSpeed;
	angle = gameObject->worldTransform.rotation;
	gameObject->worldTransform.rotation += rotation;
	Attack(playerPos, bossPos);
	
	if (isShot) {
		gameObject->Update();
	}

}
void PlayerBullet::Attack(Vector3 playerPos, Vector3 bossPos) {
	const float rotationSpeed = MathFunc::Utility::Deg2Rad(60.0f);

	Vector3 rotation = { 0 , 0 , 0 };

	rotation.y = rotationSpeed;
	rotation.x = rotationSpeed;
	rotation.z = rotationSpeed;

	gameObject->worldTransform.rotation += rotation;
	if (!isShot) {
		//その番号の弾の発射フラグがfalseならtrueにする
		isShot = true;
		isDead_ = false;
		//それと同時に同番号のオブジェクトの座標をボスの座標に持っていく
		gameObject->worldTransform.translation = newEnemyPos;
		//ボスと自機の差分ベクトルを求める
		velocity = newPlayerPos - newEnemyPos;
		//ベクトルの正規化
		velocity.nomalize();
		//ベクトルの長さを速さに合わせる
		velocity.x *= speed;
		velocity.y *= speed;
		velocity.z *= speed;
	}

	if (isShot) {
		//ボスと自機の差分ベクトルを求める
		velocity = newPlayerPos - newEnemyPos;
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
			gameObject->worldTransform.translation.z < -canMoveArea - 200 ||
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

void PlayerBullet::Draw() {

	if (isShot) {
		gameObject->Draw();

	}
}



int PlayerBullet::GetIsDead() {
	return isDead_;
}


WorldTransform PlayerBullet::GetWorldTransform() {
	return gameObject->worldTransform;
}

Vector3 PlayerBullet::GetAngle() {
	return angle;
}