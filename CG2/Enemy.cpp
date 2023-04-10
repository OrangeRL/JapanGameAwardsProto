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
	gameObject->PreLoadModel("Resources/enemy/enemy.obj");
	gameObject->SetViewProjection(viewProjection);
	gameObject->SetMatProjection(matProjection);
	gameObject->Initialize();

	gameObject->worldTransform.scale = { 2 , 2 , 2 };
	gameObject->worldTransform.translation = { 0 , 0 , 100 };
  
	pManager.Initialize(viewProjection, matProjection, L"Resources/purple1x1.png");
	//spManager.Initialize(viewProjection, matProjection);

}

//Num が 1の奴は移動のみ
//Num が 0は固定砲台
void Enemy::Update(ViewProjection* viewProjection, XMMATRIX* matProjection, int enemyNum) {
	pManager.Update(gameObject->worldTransform.translation);
	if (pManager.GetIsDead() == false) {
		pManager.Update(gameObject->worldTransform.translation);
	}
	//spManager.Update(viewProjection, matProjection,gameObject->worldTransform.translation);

	attackSpeed -= 0.5f;
	phaseTimer--;

	switch (phase)
	{
	case Phase::spown:
		if (phaseTimer <= 0.0f) {
			phase = Phase::normal;
			phaseTimer = 400.0f;
		}
		break;
	case Phase::normal:	//通常
		gameObject->worldTransform.translation += moveSpeed;
		if (phaseTimer <= 0.0f) {
			phase = Phase::move;
			phaseTimer = 200.0f;
		}
		break;
	case Phase::move:	//行動
		if (phaseTimer <= 0.0f) {
			phase = Phase::leave;
			phaseTimer = 300.0f;
		}
		break;
	case Phase::leave:	//離脱
		Leave({ 0.3f,0,0 }, { -0.3f,0,0 },enemyNum);
	}
	gameObject->Update();
}

void Enemy::Draw() {
	if (pManager.GetIsDead() == true) {
		gameObject->Draw();
	}
	pManager.Draw();
	//spManager.Draw();
	gameObject->Draw();
}

void Enemy::Reset() {
	gameObject->worldTransform.translation = { 0 , 0 , 100 };
}
//反復
void Enemy::Repetition()
{
}
//離脱
void Enemy::Leave(Vector3 leaveSpeedt,Vector3 leaveSpeedf, int enemyNum)
{
	if (enemyNum == 0) {	//固定砲台

	}

	if (enemyNum == 1) {	//ゲート
		if (gameObject->worldTransform.translation.x >= 1) {
			gameObject->worldTransform.translation += leaveSpeedt;
		}
		if (gameObject->worldTransform.translation.x <= -1) {
			gameObject->worldTransform.translation += leaveSpeedf;
		}
	}

	if (enemyNum == 2) {	//直線レーザータレット
		gameObject->worldTransform.rotation.y += 0.3f;
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

void Enemy::OnCollision(Rhythm* rhythm) {
	isDelete_ = true;
	rhythm->knockSoundPlay(1.0f);
}
