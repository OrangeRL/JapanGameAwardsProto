#include "Boss.h"

void Boss::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection)
{
	gameObject = new GameObject3D();
	gameObject->PreLoadModel("Resources/enemy/enemy.obj");
	gameObject->PreLoadTexture(L"Resources/enemy/Boss.png");
	gameObject->SetViewProjection(viewProjection);
	gameObject->SetMatProjection(matProjection);
	gameObject->Initialize();

	gameObject->worldTransform.scale = { 30 , 30 , 30 };
	gameObject->worldTransform.rotation = { 0,0,0 };
	gameObject->worldTransform.translation = { 0 , 0 , 1200 };
	HP = 25;
	sceneChange = false;
}

void Boss::Update()
{
	if (trueDead) {
		responTimer--;
		if (responTimer <= 0.0f) {
			responTimer = 10.0f;
			HP -= 1;
			trueDead = false;
		}
	}
	if (HP > 0) {
		attackSpeed -= 0.5f;
		phaseTimer -= 0.5f;
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
				phase = BossPhase::attack;
			}
			break;
		}
	}
	else
	{
		End();
	}
	gameObject->Update();
}

void Boss::Draw()
{
	gameObject->Draw();
}

void Boss::Attack()	//ライン状に弾を展開回転させる : 自機を回転させる
{
	if (gameObject->worldTransform.translation.x != 0.0f) {
		gameObject->worldTransform.translation.x = 0.0f;
	}
}

void Boss::Attack2()	//移動場所を制限する&ランダムショット : 
{
	
}

void Boss::End()	//死亡演出: 
{
	if (gameObject->worldTransform.translation.x != 0.0f) {
		gameObject->worldTransform.translation.x = 0.0f;
	}

	if (gameObject->worldTransform.scale.x != 0 && gameObject->worldTransform.scale.y != 0 && gameObject->worldTransform.scale.z != 0)
	{
		gameObject->worldTransform.rotation.y += 0.1f;
		gameObject->worldTransform.scale.z -= 0.03f;
		gameObject->worldTransform.scale.x -= 0.03f;
		gameObject->worldTransform.scale.y -= 0.03f;
	}
	if (gameObject->worldTransform.scale.z <= 0) {
		if (gameObject->worldTransform.scale.y <= 0) {
			if (gameObject->worldTransform.scale.x <= 0) {
				
				sceneChange = true;
			}
		}
	}
}

void Boss::Reset()
{
	gameObject->worldTransform.scale = { 30 , 30 , 30 };
	gameObject->worldTransform.rotation = { 0,0,0 };
	gameObject->worldTransform.translation = { 0 , 0 , 1200 };
	HP = 25;
	trueDead = false;
	isDead = false;
	attackSpeed = 200.0f;
	isAttack = false;
	sceneChange = false;
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
	trueDead = true;
}
