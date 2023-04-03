#include "Pattern2.h"

Pattern2::Pattern2() {
	//初期化以前の設定

	gameObject = nullptr;
	isShot = false;
	isDead_ = false;
}

Pattern2::~Pattern2() {
	delete gameObject;
}

void Pattern2::OnCollision() {
	isDead_ = true;
}

void Pattern2::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection) 
{
	gameObject = new GameObject3D();
	//gameObject->PreLoadModel("Resources/star/star.obj");
	gameObject->PreLoadTexture(L"Resources/red.png");
	gameObject->SetViewProjection(viewProjection);
	gameObject->SetMatProjection(matProjection);
	gameObject->Initialize();
	gameObject->worldTransform.scale = Vector3{ 0.3,0.3,1 };
	gameObject->Update();
	isDead_ = false;

}

void Pattern2::Update(Vector3 enemyPos, Vector3 playerPos) {

	Attack(enemyPos, playerPos);

	if (isShot) 
	{
		gameObject->Update();
	}
	
}
void Pattern2::Attack(Vector3 playerPos, Vector3 bossPos) {
	if (!isShot) 
	{
		//その番号の弾の発射フラグがfalseならtrueにする
		isShot = true;
		isDead_ = false;
		//それと同時に同番号のオブジェクトの座標をボスの座標に持っていく
		gameObject->worldTransform.translation = bossPos;
		//ボスと自機の差分ベクトルを求める
		velocity = { 0.0f,0.0f,0.0f };
	}

	if (isShot)
	{
		//ボスと自機の差分ベクトルを求める
		velocity = { 0.0f,0.0f,1.0f };

		//発射フラグがtrueならその時点での自機の座標に向かって移動する
		gameObject->worldTransform.translation.z += velocity.z;
		gameObject->worldTransform.translation.x = bossPos.x;
		gameObject->worldTransform.translation.y = bossPos.y;

		if (gameObject->worldTransform.translation.x < -canMoveArea ||
			gameObject->worldTransform.translation.x > canMoveArea ||
			gameObject->worldTransform.translation.y < -canMoveArea ||
			gameObject->worldTransform.translation.y > canMoveArea ||
			gameObject->worldTransform.translation.z < -canMoveArea ||
			gameObject->worldTransform.translation.z > playerPos.z + playerPos.z + 500)
		{
			//一定の範囲外で消滅
			isShot = false;
			isDead_ = true;

		}
	}
}

void Pattern2::Draw() 
{
	if (isShot) 
	{
		gameObject->Draw();
	}
}

void Pattern2::AttackPress()
{
	isShot = true;
}

int Pattern2::GetIsDead() {
	return isDead_;
}

WorldTransform Pattern2::GetWorldTransform() {
	return gameObject->worldTransform;
}

Vector3 Pattern2::GetAngle() {
	return angle;
}