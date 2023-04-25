#include "Boss.h"

void Boss::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection)
{
	gameObject = new GameObject3D();
	gameObject->PreLoadModel("Resources/tree/tree.obj");
	gameObject->PreLoadTexture(L"Resources/tree/tree.png");
	gameObject->SetViewProjection(viewProjection);
	gameObject->SetMatProjection(matProjection);
	gameObject->Initialize();

	gameObject->worldTransform.scale = { 3 , 3 , 3 };
	gameObject->worldTransform.rotation = { 0,90,0 };
	gameObject->worldTransform.translation = { 0 , 0 , 1200 };
}

void Boss::Update()
{
	attackSpeed -= 0.5f;
	phaseTimer -= 0.1f;
	switch (phase)
	{
	case BossPhase::spown:	// 誕生
		if (phaseTimer <= 0.0f) {
			phaseTimer = 300.0f;
			phase = BossPhase::attack;
		}
		break;
	case BossPhase::attack:	//攻撃1 ライン(枝)を張りそれを回転させる
		//-----------------------
		Attack();	//自分が回転するだけの移動
		if (phaseTimer <= 0.0f) {
			phaseTimer = 300.0f;
			phase = BossPhase::attack2;
		}
		break;
	case BossPhase::attack2:	//攻撃2 移動場所を絞らせる & ランダムショット
		// ↓攻撃処理2　
		Attack2();
		//-----------------------
		if (phaseTimer <= 0.0f) {
			phaseTimer = 100.0f;
			phase = BossPhase::defence;
		}
		break;
	case BossPhase::defence:	//回避,防御
		// ↓防御処理	ダメージ軽減効果を出す
		Defence();
		//-----------------------
		if (phaseTimer <= 0.0f) {
			phaseTimer = 300.0f;
			phase = BossPhase::attack;
		}
		break;
	}
	gameObject->Update();
}

void Boss::Draw()
{
	if (HP > 0) {
		gameObject->Draw();
	}
}

void Boss::Attack()	//ライン状に弾を展開回転させる : 自機を回転させる
{
	if (gameObject->worldTransform.translation.x != 0.0f) {
		gameObject->worldTransform.translation.x = 0.0f;
	}
}

void Boss::Attack2()	//移動場所を制限する&ランダムショット : 
{
	
	gameObject->worldTransform.translation += moveSpeed;

	if (gameObject->worldTransform.translation.x >= 5.0f|| gameObject->worldTransform.translation.x <= -5.0f) {
		moveSpeed = -moveSpeed;
	}
}

void Boss::Defence()	//ダメージを軽減させる : 
{
	if (gameObject->worldTransform.translation.x != 0.0f) {
		gameObject->worldTransform.translation.x = 0.0f;
	}
}

float Boss::Random(float minValue, float maxValue)
{
	//シード値乱数生成器
	std::random_device rnd;
	//メルセンヌ・ツイスタ方を使って乱数を作る
	std::mt19937_64 mt64(rnd());
	//範囲内の離散分布を作る
	std::uniform_real_distribution<float> genRandFloat(minValue, maxValue);
	//分布の中から生成した乱数を使って1つだけ値を取り出す
	return genRandFloat(mt64);
}

WorldTransform Boss::GetWorldTransform()
{
	return gameObject->worldTransform;
}

BossPhase Boss::GetPhase()
{
	return phase;
}

float Boss::GetAttackSpeed()
{
	return attackSpeed;
}

float Boss::SetAttackSpeed(float speed)
{
	this->attackSpeed = speed;

	return this->attackSpeed;
}

bool Boss::GetIsAttack()
{
	return isAttack;
}

bool Boss::SetIsAttack(bool isAttack)
{
	this->isAttack = isAttack;

	return this->isAttack;
}

bool Boss::GetIsDead()
{
	return isDead;
}

int Boss::GetHP()
{
	return HP;
}

void Boss::OnCollision()
{
	HP -= 1;
	if (HP <= 0) {
		isDead = true;
	}
}
