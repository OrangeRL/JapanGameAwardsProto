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

void Enemy::Update() {

	//if (ct <= 0) {
	//	moveSpeed = g * (timer / 50);

	//	gameObject->worldTransform.translation.z -= moveSpeed;

	//	timer++;
		if (gameObject->worldTransform.translation.z < -50) {
			timer = 0;
			gameObject->worldTransform.translation.z = 100;
			ct = 50;
		}
	//}
	//else {
	//	ct--;
	//}

	gameObject->Update();

}

void Enemy::Draw() {
	gameObject->Draw();
}

WorldTransform Enemy::GetWorldTransform() {
	return gameObject->worldTransform;
}