#include "Enemy.h"
#include "MathFunc.h"

Enemy::Enemy() {
	
}

Enemy::~Enemy() {
	delete gameObject;
}
void Enemy::Spawn() {
	spawnFlag = true;
}
void Enemy::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection, const wchar_t* textureFileName) {
	pManager.Initialize(viewProjection, matProjection, L"Resources/red1x1.png");
	gameObject = new GameObject3D();
	gameObject->PreLoadTexture(textureFileName);
	gameObject->PreLoadModel("Resources/enemy/enemy.obj");
	gameObject->SetViewProjection(viewProjection);
	gameObject->SetMatProjection(matProjection);
	gameObject->Initialize();

	gameObject->worldTransform.scale = { 2 , 2 , 2 };

	spawnFlag = false;
	pManager.Initialize(viewProjection, matProjection, L"Resources/purple1x1.png");
	//spManager.Initialize(viewProjection, matProjection);
}

//Num が 1の奴は移動のみ
//Num が 0は固定砲台
void Enemy::Update(ViewProjection* viewProjection, XMMATRIX* matProjection, int enemyNum) {
	
	//spManager.Update(viewProjection,matProjection,gameObject->worldTransform.translation,gameObject->worldTransform.scale);
	if (spawnFlag == true) 
	{
	pManager.Update(gameObject->worldTransform.translation);

	attackSpeed -= 0.5f;
		if (moveNum == 0){

		}
		else if (moveNum == 1) { //front
			gameObject->worldTransform.translation.z += moveSpeed.z;
		}
		else if (moveNum == 2) { //Down
			gameObject->worldTransform.translation.y -= moveSpeed.y;
		}
		else if (moveNum == 3) { //UP
			gameObject->worldTransform.translation.y += moveSpeed.y;
		}
	}
	gameObject->Update();
}

void Enemy::Draw() {
	if (pManager.GetIsDead() == true) {
		gameObject->Draw();
	}
	//gameObject->Draw();
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
WorldTransform Enemy::Settransform(Vector3 x)
{
	this->gameObject->worldTransform.translation = x;

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

void Enemy::SetBulletNum(int32_t bulletNum)
{
	useBullet = bulletNum;
}

void Enemy::SetMoveNum(int32_t moveNum)
{
	this->moveNum = moveNum;
}

int32_t Enemy::GetBulletNum()
{
	return useBullet;
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

int Enemy::GetSpownFlag()
{
	return spawnFlag;
}

void Enemy::OnCollision(Rhythm* rhythm) {
	isDelete_ = true;
	rhythm->knockSoundPlay(1.0f);
}
