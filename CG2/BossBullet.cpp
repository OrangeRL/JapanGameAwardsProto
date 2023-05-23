#include "BossBullet.h"
#include <random>

void BossBullet::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection, Vector3 player, Vector3 enemy)
{
	gameObject = new GameObject3D();
	gameObject->PreLoadTexture(L"Resources/white1x1.png");
	gameObject->SetViewProjection(viewProjection);
	gameObject->SetMatProjection(matProjection);
	gameObject->Initialize();

	Aim(player, enemy);
}

void BossBullet::Update(BossPhase phase, Vector3 player)
{
	if (phase == attack) {
		gameObject->worldTransform.scale = { 1,1,1 };
		gameObject->worldTransform.translation -= posC;
	}
	else if (phase == attack2) {
		if (isSelect = false)
		{
			isSelect = true;
		}
		if (selectPos == 0)
		{
			gameObject->worldTransform.scale = { 15,1,1 };
			gameObject->worldTransform.translation = { 0,9,player.z };
		}
		else if (selectPos == 1)
		{
			gameObject->worldTransform.scale = { 15,1,1 };
			gameObject->worldTransform.translation = { 0,-9,player.z };
		}
		else
		{
			gameObject->worldTransform.scale = { 15,1,1 };
			gameObject->worldTransform.translation = { 0,0,player.z };
		}
	}

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

void BossBullet::Aim(Vector3 player, Vector3 enemy)
{
	posA = player;
	posB = enemy;
	posC = posA - posB;
	posC.nomalize();
	posC *= -1.5f;
}