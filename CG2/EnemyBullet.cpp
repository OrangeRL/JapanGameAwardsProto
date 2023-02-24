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
	//if (input.TriggerKey(DIK_SPACE))
	//{
	//	gameObject->worldTransform.translation = enemy.translation;
	//}
	gameObject->Update();
}

void EnemyBullet::Draw()
{
	gameObject->Draw();
}
