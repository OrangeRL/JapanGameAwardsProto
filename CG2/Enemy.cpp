#include "Enemy.h"

Enemy::Enemy() {

}

Enemy::~Enemy() {
	delete gameObject;
}

void Enemy::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection, const wchar_t* textureFileName) {

	gameObject = new GameObject3D();
	gameObject->PreLoadTexture(textureFileName);
	gameObject->SetViewProjection(viewProjection);
	gameObject->SetMatProjection(matProjection);
	gameObject->Initialize();


	gameObject->worldTransform.translation = { 0 , 0 , 100 };
	gameObject->worldTransform.scale = { 2 , 2 , 2 };

	
}


void Enemy::Update(ViewProjection* viewProjection, XMMATRIX* matProjection, const wchar_t* textureFileName, int bulletNum) {
	if (isAttack == false) {
		attackSpeed -= 0.5f;
		if (attackSpeed <= 0.0f) {
			//弾を生成
			std::unique_ptr<EnemyBullet> bullet = std::make_unique<EnemyBullet>();
			//初期化
			bullet->Initialize(viewProjection, matProjection, textureFileName);
			bullet->SetTransform(gameObject->worldTransform.translation);
			//使う弾の設定
			bullet->SetBullet(bulletNum);
			bullets.push_back(std::move(bullet));
			isAttack = true;
			attackSpeed = 100.0f;
		}
	}
	else
	{
		for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
			bullet->Update(player_->GetWorldTransform());
			//弾が画面外に言ったらEnemyの座標に戻す(いずれ削除に変更すること)
			if (bullet->GetWorldTransform().translation.z <= -15.0f) {
				bullet->SetTransform(gameObject->worldTransform.translation);
			}
		}
	}


	gameObject->worldTransform.translation.y += moveSpeed;
	if (gameObject->worldTransform.translation.y >= 70 || gameObject->worldTransform.translation.y <= -70)
	{
		moveSpeed = -moveSpeed;
	}
	gameObject->Update();

}

void Enemy::Draw() {
	gameObject->Draw();
	for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
		bullet->Draw();
	}
}

void Enemy::Reset() {
	gameObject->worldTransform.translation = { 0 , 0 , 100 };
}

WorldTransform Enemy::GetWorldTransform() {
	return gameObject->worldTransform;
}

WorldTransform Enemy::Settransform(float x,float y,float z)
{
	this->gameObject->worldTransform.translation.x = x;
	this->gameObject->worldTransform.translation.y = y;
	this->gameObject->worldTransform.translation.z = z;

	return gameObject->worldTransform;
}

float Enemy::SetSpeed(float speed)
{
	this->moveSpeed = speed;
	return moveSpeed;
}
