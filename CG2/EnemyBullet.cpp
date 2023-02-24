#include "EnemyBullet.h"

void EnemyBullet::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection, const wchar_t* textureFileName)
{
	gameObject = new GameObject3D();
	gameObject->PreLoadTexture(textureFileName);
	gameObject->SetViewProjection(viewProjection);
	gameObject->SetMatProjection(matProjection);
	gameObject->Initialize();

}

void EnemyBullet::Update(WorldTransform enemy)
{

	gameObject->Update();
}

void EnemyBullet::Draw()
{
	gameObject->Draw();
}

Vector3 EnemyBullet::SetTransform(Vector3 transform)
{
	this->gameObject->worldTransform.translation = transform;

	return gameObject->worldTransform.translation;
}
