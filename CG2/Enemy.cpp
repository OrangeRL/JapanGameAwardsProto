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
		switch (phase)
		{
		case Phase::normal:
		default:
			//移動
			phaseTimer -= 0.3f;
			gameObject->worldTransform.translation += moveSpeed;
			if (phaseTimer <= 0.0f) {
				phaseTimer = 300.0f;
				phase = Phase::move;
			}
			break;
		case Phase::move:
			//攻撃

			break;
		case Phase::leave:
			//離脱
			Vector3 leaveSpeedt = { 0.5f,0.0f,0.3f };
			Vector3 leaveSpeedf = { -0.5f,0.0f,0.3f };
			Leave(leaveSpeedt, leaveSpeedf);
			break;
		}

	}
	else if (enemyNum == 1) {
		gameObject->worldTransform.translation += moveSpeed;
		//�㉺�𔽕��ړ�
		//Repetition();

		CoolTime();

	gameObject->Update();

}

void Enemy::Draw() {
	gameObject->Draw();
}

void Enemy::Reset() {
	gameObject->worldTransform.translation = { 0 , 0 , 100 };
}
//�����֐�
void Enemy::Repetition()
{
}
//���E
void Enemy::Leave(Vector3 leaveSpeedt,Vector3 leaveSpeedf)
{
	if (gameObject->worldTransform.translation.x >= 1) {
		gameObject->worldTransform.translation += leaveSpeedt;
	}
	if (gameObject->worldTransform.translation.x <= -1) {
		gameObject->worldTransform.translation += leaveSpeedf;
	}
}
//�U���p�N�[���^�C��
void Enemy::CoolTime()
{
	if (isCoolDown) {
		coolTime--;
		if (coolTime <= 0.0f) {
			isCoolDown = false;
		}
	}
	else {
		coolTime++;
		if (coolTime >= 150.0f) {
			isCoolDown = true;
		}
	}
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

bool Enemy::GetCoolDown()
{
	return isCoolDown;
}

Vector3 Enemy::SetSpeed(float x, float y, float z)
{
	Vector3 speed = { x,y,z };
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

Phase Enemy::GetPhase()
{
	return phase;
}
