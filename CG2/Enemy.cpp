#include "Enemy.h"
#include "MathFunc.h"

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


	gameObject->worldTransform.translation = { 0 , 0 , 100 };
	gameObject->worldTransform.scale = { 2 , 2 , 2 };

	
}


void Enemy::Update(ViewProjection* viewProjection, XMMATRIX* matProjection, const wchar_t* textureFileName, int enemyNum) {

	attackSpeed -= 0.5f;
	if (enemyNum == 0) {
		gameObject->worldTransform.translation.z += 0.1f;
	}
	else if (enemyNum == 1) {
		gameObject->worldTransform.translation.y += moveSpeed;
		if (gameObject->worldTransform.translation.y >= 70 || gameObject->worldTransform.translation.y <= -70)
		{
			moveSpeed = -moveSpeed;
		}
	}
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

WorldTransform Enemy::Settransform(float x,float y,float z)
{
	this->gameObject->worldTransform.translation.x = x;
	this->gameObject->worldTransform.translation.y = y;
	this->gameObject->worldTransform.translation.z = z;

	return gameObject->worldTransform;
}

float Enemy::GetAttackSpeed(){
	return attackSpeed;
}

float Enemy::SetAttackSpeed(float speed)
{
	this->attackSpeed = speed;
	return attackSpeed;
}

float Enemy::SetSpeed(float speed)
{
	this->moveSpeed = speed;
	return moveSpeed;
}

bool Enemy::GetIsAttack()
{
	return isAttack;
}

bool Enemy::SetIsAttack(bool isAttack)
{
	this->isAttack = isAttack;
	return this->isAttack;
}
