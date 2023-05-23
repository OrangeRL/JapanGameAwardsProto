#include "EnemyBullet.h"

EnemyBullet::EnemyBullet() {
	
}

EnemyBullet::~EnemyBullet() {
	delete gameObject;
}

void EnemyBullet::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection, const wchar_t* textureFileName,Vector3 player, Vector3 enemy)
{
	//自機狙い用座標取得
	Aim(player, enemy);

	gameObject = new GameObject3D();
	gameObject->PreLoadTexture(textureFileName);
	gameObject->SetViewProjection(viewProjection);
	gameObject->SetMatProjection(matProjection);
	gameObject->Initialize();
	
}

void EnemyBullet::Update(bool enemy)
{
	//プレイヤーを狙う
	if (bulletNum == 0){
		gameObject->worldTransform.translation -= posC;
	}

	if (--deleteTimer_ <= 0 || enemy == true) {
		isDelete_ = true;
	}

	if (enemy == true || enemy == false)
	{
		gameObject->Update();
	}
}


void EnemyBullet::Draw()
{
	gameObject->Draw();
}

WorldTransform EnemyBullet::GetWorldTransform()
{
	return gameObject->worldTransform;
}

Vector3 EnemyBullet::SetTransform(Vector3 transform)
{
	this->gameObject->worldTransform.translation = transform;

	return gameObject->worldTransform.translation;
}

Vector3 EnemyBullet::SetScale(Vector3 scale)
{
	gameObject->worldTransform.scale = scale;

	return this->gameObject->worldTransform.scale;
}

int EnemyBullet::SetBullet(int bulletNum)
{
	this->bulletNum = bulletNum;

	return this->bulletNum;
}

void EnemyBullet::Aim(Vector3 player, Vector3 enemy)
{
	const float speed = -0.3f;
	posA = player;
	posB = enemy;
	posC = posA - posB;
	posC.nomalize();
	posC *= speed;
}

bool EnemyBullet::GetIsDead()
{
	return isDelete_;
}

void EnemyBullet::OnCollision()
{
	isDelete_ = true;
}

