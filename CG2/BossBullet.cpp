#include "BossBullet.h"

void BossBullet::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection,float Random)
{
	gameObject = new GameObject3D();
	gameObject->PreLoadTexture(L"Resources/white1x1.png");
	gameObject->SetViewProjection(viewProjection);
	gameObject->SetMatProjection(matProjection);
	gameObject->Initialize();
	speed = { Random,Random,Random };

}

void BossBullet::Update()
{
	gameObject->worldTransform.translation -= speed;

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
