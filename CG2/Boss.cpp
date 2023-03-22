#include "Boss.h"

void Boss::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection)
{
	gameObject = new GameObject3D();
	gameObject->PreLoadTexture(L"Resources/red.png");
	gameObject->SetViewProjection(viewProjection);
	gameObject->SetMatProjection(matProjection);
	gameObject->Initialize();


	gameObject->worldTransform.translation = { 0 , 0 , 600 };
	gameObject->worldTransform.scale = { 10 , 15 , 10 };
}

void Boss::Update()
{
	phaseTimer--;
	switch (phase)
	{
	case BossPhase::spown:	// 誕生
		if (phaseTimer <= 0.0f) {
			phase = BossPhase::attack;
			isDead = false;
			phaseTimer = 400.0f;
		}
		break;
	case BossPhase::attack:	//攻撃1 ライン(枝)を張りそれを回転させる
		// ↓攻撃処理
		Attack();
		//-----------------------
		if (phaseTimer <= 0.0f) {
			phase = BossPhase::attack2;
			phaseTimer = 400.0f;
		}
		break;
	case BossPhase::attack2:	//攻撃2
		// ↓攻撃処理2　移動場所を絞らせる & ランダムショット
		Attack2();
		//-----------------------
		if (phaseTimer <= 0.0f) {
			phase = BossPhase::defence;
			phaseTimer = 400.0f;
		}
		break;
	case BossPhase::defence:	//回避,防御
		// ↓防御処理	ダメージ軽減効果を出す
		Defence();
		//-----------------------
		if (phaseTimer <= 0.0f) {
			phase = BossPhase::attack;
			phaseTimer = 200.0f;
		}
		break;
	}

	gameObject->Update();
}

void Boss::Draw()
{
	if (isDead == false) {
		gameObject->Draw();
	}
}

void Boss::Attack()	//ライン状に弾を展開回転させる
{

}

void Boss::Attack2()	//移動場所を制限する&ランダムショット
{

}

void Boss::Defence()	//ダメージを軽減させる
{

}
