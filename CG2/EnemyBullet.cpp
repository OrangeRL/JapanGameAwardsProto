#include "EnemyBullet.h"

void EnemyBullet::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection, const wchar_t* textureFileName,Vector3 player, Vector3 enemy)
{
	Bullet1(player, enemy);

	gameObject = new GameObject3D();
	gameObject->PreLoadTexture(textureFileName);
	gameObject->SetViewProjection(viewProjection);
	gameObject->SetMatProjection(matProjection);
	gameObject->Initialize();

}

void EnemyBullet::Update()
{
	//プレイヤーを狙う
	if (bulletNum == 0){
		gameObject->worldTransform.translation -= posC;
	}
	//正面にカーテン形成
	else if (bulletNum == 1) {
		gameObject->worldTransform.translation.z -= 0.1f;
	}

	if (--deleteTimer_ <= 0) {
		isDelete_ = true;
	}

	gameObject->Update();
	
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

int EnemyBullet::SetBullet(int bulletNum)
{
	this->bulletNum = bulletNum;

	return this->bulletNum;
}

void EnemyBullet::Bullet1(Vector3 player, Vector3 enemy)
{
	const float speed = -0.3f;
	posA = player;
	posB = enemy;
	posC = posA - posB;
	posC.nomalize();
	posC *= speed;
}
