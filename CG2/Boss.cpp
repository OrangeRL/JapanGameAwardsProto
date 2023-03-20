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
	case BossPhase::spown:	// ’a¶
		if (phaseTimer >= 0.0f) {
			phase = BossPhase::attack;
			phaseTimer = 400.0f;
		}
		break;
	case BossPhase::attack:	//UŒ‚1
		if (phaseTimer >= 0.0f) {
			phase = BossPhase::attack2;
			phaseTimer = 400.0f;
		}
		break;
	case BossPhase::attack2:	//UŒ‚2
		if (phaseTimer >= 0.0f) {
			phase = BossPhase::defence;
			phaseTimer = 400.0f;
		}
		break;
	case BossPhase::defence:	//‰ñ”ð,–hŒä
		if (phaseTimer >= 0.0f) {
			phase = BossPhase::attack;
			phaseTimer = 200.0f;
		}
		break;
	}

	gameObject->Update();
}

void Boss::Draw()
{
	gameObject->Draw();
}