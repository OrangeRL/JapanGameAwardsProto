#include "Enemy.h"

Enemy::Enemy() {

}

Enemy::~Enemy() {
	delete gameObject;
}

void Enemy::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection, const wchar_t* textureFileName) {

	gameObject = new GameObject3D();
	gameObject->PreLoadTexture(textureFileName);
	gameObject->SetViewProjection(viewProjection);
	gameObject->SetMatProjection(matProjection);
	gameObject->Initialize();

	gameObject->worldTransform.translation = { 0 , 0 , 75 };
	gameObject->worldTransform.scale = { 2 , 2 , 2};

	gameObject->Update();
}

void Enemy::Update() {

	if (gameObject->worldTransform.translation.y >= 60 || gameObject->worldTransform.translation.y <= -60)
	{
		moveSpeed = -moveSpeed;
	}
	gameObject->worldTransform.translation += moveSpeed;
	gameObject->Update();

}

void Enemy::Draw() {
	gameObject->Draw();
}

WorldTransform Enemy::GetWorldTransform() {
	return gameObject->worldTransform;
}

WorldTransform Enemy::Settransform(float x,float y,float z)
{
	this->gameObject->worldTransform.translation.x = x;
	this->gameObject->worldTransform.translation.y = y;
	this->gameObject->worldTransform.translation.z = z;

	return gameObject->worldTransform;
}

Vector3 Enemy::SetSpeed(float x, float y, float z)
{
	this->moveSpeed.x = x;
	this->moveSpeed.y = y;
	this->moveSpeed.z = z;

	return moveSpeed;
}
