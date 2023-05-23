#include "BossBullet.h"

void BossBullet::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection, Vector3 player, Vector3 enemy)
{
	gameObject = new GameObject3D();
	gameObject->PreLoadTexture(L"Resources/white1x1.png");
	gameObject->SetViewProjection(viewProjection);
	gameObject->SetMatProjection(matProjection);
	gameObject->Initialize();

	for (int i = 0; i < 4; i++)
	{
		gameObject2[i] = new GameObject3D();
		gameObject2[i]->PreLoadTexture(L"Resources/white1x1.png");
		gameObject2[i]->SetViewProjection(viewProjection);
		gameObject2[i]->SetMatProjection(matProjection);
		gameObject2[i]->Initialize();
	}

	Aim(player, enemy);
}

void BossBullet::Update(BossPhase phase, Vector3 player)
{
	if (phase == attack) {
		gameObject->worldTransform.translation -= posC;
	}
	else if (phase == attack2) {
		gameObject->worldTransform.translation -= posC;
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
