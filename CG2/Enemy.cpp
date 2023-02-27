#include "Enemy.h"

Enemy::Enemy() {

}

Enemy::~Enemy() {
	delete gameObject;
}

void Enemy::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection) {

	gameObject = new GameObject3D();
	gameObject->PreLoadTexture(L"Resources/white1x1.png");
	gameObject->SetViewProjection(viewProjection);
	gameObject->SetMatProjection(matProjection);
	gameObject->Initialize();

	gameObject->worldTransform.translation = { 0 , 0 , 100 };
	gameObject->worldTransform.scale = { 2 , 2 , 2 };

}

void Enemy::Update(Vector3 playerPos, Vector3 bossPos) {


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

	gameObject->Update();

}

void Enemy::Draw() {
	gameObject->Draw();
}

void Enemy::Reset() {
	gameObject->worldTransform.translation = { 0 , 0 , 100 };
}

WorldTransform Enemy::GetWorldTransform() {
	return gameObject->worldTransform;
}