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
	gameObject->PreLoadModel("Resources/star/star.obj");
	gameObject->SetViewProjection(viewProjection);
	gameObject->SetMatProjection(matProjection);
	gameObject->Initialize();

	gameObject->worldTransform.translation = { 0 , 0 , 100 };
	gameObject->worldTransform.scale = { 2 , 2 , 2 };
	
	pManager.Initialize(viewProjection, matProjection, L"Resources/purple1x1.png");
	//spManager.Initialize(viewProjection, matProjection);
}

void Enemy::Update(ViewProjection* viewProjection, XMMATRIX* matProjection, int enemyNum) {
	pManager.Update(gameObject->worldTransform.translation);
	if (pManager.GetIsDead() == false) {
		pManager.Update(gameObject->worldTransform.translation);
	}
	//spManager.Update(viewProjection, matProjection,gameObject->worldTransform.translation);

	attackSpeed -= 0.5f;
	if (enemyNum == 0) {
	}
	else if (enemyNum == 1) {
		phaseTimer--;
		switch (phase)
		{
		case Phase::normal:
			gameObject->worldTransform.translation += moveSpeed;
			if (phaseTimer <= 0.0f) {
				phase = Phase::move;
				phaseTimer = 300.0f;
			}
			break;
		case Phase::move:
			if (phaseTimer <= 0.0f) {
				phase = Phase::leave;
				phaseTimer = 300.0f;
			}
			break;
			break;
		case Phase::leave:
			Leave({ 0.3f,0,0 }, { -0.3f,0,0 });
		}

	}
	gameObject->Update();
}

void Enemy::Draw() {
	if (pManager.GetIsDead() == true) {
		gameObject->Draw();
	}
	pManager.Draw();
	//spManager.Draw();
}

void Enemy::Reset() {
	gameObject->worldTransform.translation = { 0 , 0 , 100 };
}
//反復
void Enemy::Repetition()
{
}
//離脱
void Enemy::Leave(Vector3 leaveSpeedt,Vector3 leaveSpeedf)
{
	if (gameObject->worldTransform.translation.x >= 1) {
		gameObject->worldTransform.translation += leaveSpeedt;
	}
	if (gameObject->worldTransform.translation.x <= -1) {
		gameObject->worldTransform.translation += leaveSpeedf;
	}

	if (--deleteTimer_ <= 0) {
		isDelete_ = true;
	}

}
//弾のクールタイム
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

void Enemy::OnCollision() {
	isDelete_ = true;
}