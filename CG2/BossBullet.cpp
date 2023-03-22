#include "BossBullet.h"

void BossBullet::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection)
{
	gameObject = new GameObject3D();
	gameObject->PreLoadTexture(L"Resources/white1x1.png");
	gameObject->SetViewProjection(viewProjection);
	gameObject->SetMatProjection(matProjection);
	gameObject->Initialize();
	speed = { 0,0,0.5f };
}

void BossBullet::Update()
{
	gameObject->worldTransform.translation.z -= 0.5f;

	gameObject->Update();
}

void BossBullet::Draw()
{
	gameObject->Draw();
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
