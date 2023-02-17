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

	gameObject->worldTransform.translation = { 0 , 0 , 75 };
	gameObject->worldTransform.scale = { 2 , 2 , 2};

}

void Enemy::Update() {

	
	gameObject->Update();

}

void Enemy::Draw() {
	gameObject->Draw();
}

WorldTransform Enemy::GetWorldTransform() {
	return gameObject->worldTransform;
}