#include "PlayerBullet.h"

PlayerBullet::PlayerBullet() {
	//初期化以前の設定

	gameObject = nullptr;
	isShot = false;
	isDead_ = false;
}

PlayerBullet::~PlayerBullet() {
	delete gameObject;
}

void PlayerBullet::OnCollision() {
	isDead_ = true;
}

void PlayerBullet::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection, Vector3 playerPos, Vector3 bossPos, Weapons weapon)
{
	gameObject = new GameObject3D();
	//gameObject->PreLoadModel("Resources/star/star.obj");
	//gameObject->PreLoadTexture(L"Resources/red.png");
	gameObject->SetViewProjection(viewProjection);
	gameObject->SetMatProjection(matProjection);
	gameObject->Initialize();
	gameObject->worldTransform.scale = Vector3{ 2,2,2 };
	gameObject->Update();
	isDead_ = false;
	newPlayerPos = playerPos;
	newEnemyPos = bossPos;
	gameObject->worldTransform.translation = bossPos;
	this->weapon = weapon;

	if (weapon == Weapons::ThreeWay) {
		for (int i = 0; i < 2; i++) {
			gameObjectSub[i] = new GameObject3D();
			//gameObject->PreLoadModel("Resources/star/star.obj");
			gameObjectSub[i]->PreLoadTexture(L"Resources/red.png");
			gameObjectSub[i]->SetViewProjection(viewProjection);
			gameObjectSub[i]->SetMatProjection(matProjection);
			gameObjectSub[i]->Initialize();
			gameObjectSub[i]->worldTransform.translation = bossPos;
		}
	}

}

void PlayerBullet::Update(Vector3 vec, float shotAngle) {
	const float rotationSpeed = MathFunc::Utility::Deg2Rad(0.1f);

	if (weapon == Weapons::Normal) {
		gameObject->worldTransform.scale = { 2.0f,2.0f,2.0f };
		speed = -5.0f;
	}
	else if (weapon == Weapons::Rapid) {
		gameObject->worldTransform.scale = { 1.0f,1.0f,1.0f };
		speed = -10.0f;
	}
	else if(weapon == Weapons::ThreeWay) {
		gameObject->worldTransform.scale = { 1.0f,1.0f,1.0f };
		speed = -5.0f;
		for (int i = 0; i < 2; i++) {
			gameObjectSub[i]->worldTransform.scale = {1.0f,1.0f,1.0f};
		}
	}
	else {

	}

	Vector3 rotation = { 0 , 0 , 0 };
	//newPlayerPos = playerPos;*/
	rotation.y = rotationSpeed;
	rotation.x = rotationSpeed;
	rotation.z = rotationSpeed;
	angle = gameObject->worldTransform.rotation;
	gameObject->worldTransform.rotation += rotation;

	Attack(newPlayerPos, newEnemyPos, vec,shotAngle);
	if (isShot) 
	{
		gameObject->Update();
		if (weapon == Weapons::ThreeWay) {
			for (int i = 0; i < 2; i++) {
				gameObjectSub[i]->Update();
			}
		}
	}

	deathTimer_--;
	if (deathTimer_ <= 0) {
		isDead_ = true;
	}
	
}

void PlayerBullet::Attack(Vector3 playerPos, Vector3 bossPos, Vector3 vec, float shotAngle) {
	const float rotationSpeed = MathFunc::Utility::Deg2Rad(60.0f);

	Vector3 rotation = { 0 , 0 , 0 };

	//rotation.y = rotationSpeed;
	//rotation.x = rotationSpeed;
	//rotation.z = rotationSpeed;
	
	//gameObject->worldTransform.rotation += rotation;
	if (!isShot) 
	{
		//その番号の弾の発射フラグがfalseならtrueにする
		isShot = true;
		isDead_ = false;
		//それと同時に同番号のオブジェクトの座標をボスの座標に持っていく
		newPlayerPos = playerPos;
		newEnemyPos = bossPos;
		//gameObject->worldTransform.translation = bossPos;
		//ボスと自機の差分ベクトルを求める
		//velocity = newPlayerPos - newEnemyPos;
		velocity = { sin(vec.y + shotAngle),-vec.x,cos(vec.y + shotAngle)};
		//vec1 = { sin(vec.y + shotAngle) * speed,-vec.x * speed,cos(vec.y + shotAngle) * speed };
		//vec2 = { sin(vec.y - shotAngle) * speed,-vec.x * speed,cos(vec.y - shotAngle) * speed };
		//ベクトルの正規化
		//velocity.nomalize();
		//ベクトルの長さを速さに合わせる
		velocity.x *= speed;
		velocity.y *= speed;
		velocity.z *= speed;
	}

	if (isShot) 
	{
		//ボスと自機の差分ベクトルを求める
		//velocity = newPlayerPos - newEnemyPos;
		//velocity = { wt.rotation.x,wt.rotation.y,wt.rotation.z };
		
		//ベクトルの正規化
		//velocity.nomalize();
		//ベクトルの長さを速さに合わせる
		/*velocity.x *= speed;
		velocity.y *= speed;
		velocity.z *= speed;*/
		//発射フラグがtrueならその時点での自機の座標に向かって移動する
		gameObject->worldTransform.translation -= velocity;
		if (weapon == Weapons::ThreeWay) {

			gameObjectSub[0]->worldTransform.translation -= vec1;
			gameObjectSub[1]->worldTransform.translation -= vec2;
			
		}

		if (gameObject->worldTransform.translation.x < -canMoveArea ||
			gameObject->worldTransform.translation.x > canMoveArea ||
			gameObject->worldTransform.translation.y < -canMoveArea ||
			gameObject->worldTransform.translation.y > canMoveArea ||
			gameObject->worldTransform.translation.z < -canMoveArea - 00 ||
			gameObject->worldTransform.translation.z > canMoveArea + playerPos.z + 400) 
		{
			//一定の範囲外で消滅
			//isShot = false;
			//isDead_ = true;
      
		}
	}
}

void PlayerBullet::Draw() 
{
	if (isShot) 
	{
		gameObject->Draw();
		for (int i = 0; i < 2; i++) {
			if (weapon == Weapons::ThreeWay) {
				//gameObjectSub[i]->Draw();
			}
		}
	}
}

void PlayerBullet::AttackPress()
{
	isShot = true;
}

int PlayerBullet::GetIsDead() {
	return isDead_;
}

WorldTransform PlayerBullet::GetWorldTransform() {
	return gameObject->worldTransform;
}

Vector3 PlayerBullet::GetAngle() {
	return angle;
}