#include "EnemyBullet.h"

void EnemyBullet::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection, const wchar_t* textureFileName)
{
	gameObject = new GameObject3D();
	gameObject->PreLoadTexture(textureFileName);
	gameObject->SetViewProjection(viewProjection);
	gameObject->SetMatProjection(matProjection);
	gameObject->Initialize();

}

void EnemyBullet::Update(Vector3 player, Vector3 enemy)
{
	//プレイヤーを狙う
	if (bulletNum == 0){
		Vector3 velocity = player - enemy;
		velocity.nomalize();
		velocity.x *= -0.5f;
		velocity.y *= -0.5f;
		velocity.z *= -0.5f;
		gameObject->worldTransform.translation -= velocity;
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