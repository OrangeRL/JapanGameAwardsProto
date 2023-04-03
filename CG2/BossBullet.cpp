#include "BossBullet.h"

void BossBullet::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection, Vector3 player, Vector3 enemy)
{
	gameObject = new GameObject3D();
	gameObject->PreLoadTexture(L"Resources/white1x1.png");
	gameObject->SetViewProjection(viewProjection);
	gameObject->SetMatProjection(matProjection);
	gameObject->Initialize();

	Aim(player, enemy);
}

void BossBullet::Update()
{
	gameObject->worldTransform.translation -= posC;

	if (--deleteTimer_ <= 0) {
		isDelete_ = true;
	}

	gameObject->Update();
}

void BossBullet::Draw()
{
	gameObject->Draw();
}

WorldTransform BossBullet::GetWorldTransform()
{
	return gameObject->worldTransform;
}

//WorldTransform BossBullet::GetWorldTransform()
//{
//	return gameObject->worldTransform;
//}

Vector3 BossBullet::SetTransform(Vector3 transform)
{
	this->gameObject->worldTransform.translation = transform;
	return this->gameObject->worldTransform.translation;
}

Vector3 BossBullet::SetScale(Vector3 scale)
{
	this->gameObject->worldTransform.scale = scale;
	return this->gameObject->worldTransform.scale;
}

int BossBullet::SetBullet(int bulletNum)
{
	this->bulletNum = bulletNum;

	return this->bulletNum;
}

void BossBullet::Aim(Vector3 player, Vector3 enemy)
{
	posA = player;
	posB = enemy;
	posC = posA - posB;
	posC.nomalize();
	posC *= -0.5f;
}
