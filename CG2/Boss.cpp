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
	attackSpeed -= 0.5f;
	phaseTimer--;
	switch (phase)
	{
	case BossPhase::spown:	// íaê∂
		if (phaseTimer <= 0.0f) {
			phase = BossPhase::attack;
			isDead = false;
			phaseTimer = 500.0f;
		}
		break;
	case BossPhase::attack:	//çUåÇ1 ÉâÉCÉì(é})Çí£ÇËÇªÇÍÇâÒì]Ç≥ÇπÇÈ
		//-----------------------
		Attack();	//é©ï™Ç™âÒì]Ç∑ÇÈÇæÇØÇÃà⁄ìÆ
		if (phaseTimer <= 0.0f) {
			phase = BossPhase::attack2;
			phaseTimer = 1000.0f;
		}
		break;
	case BossPhase::attack2:	//çUåÇ2 à⁄ìÆèÍèäÇçiÇÁÇπÇÈ & ÉâÉìÉ_ÉÄÉVÉáÉbÉg
		// Å´çUåÇèàóù2Å@
		Attack2();
		//-----------------------
		if (phaseTimer <= 0.0f) {
			phase = BossPhase::defence;
			phaseTimer = 400.0f;
		}
		break;
	case BossPhase::defence:	//âÒî,ñhå‰
		// Å´ñhå‰èàóù	É_ÉÅÅ[ÉWåyå∏å¯â ÇèoÇ∑
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

void Boss::Attack()	//ÉâÉCÉìèÛÇ…íeÇìWäJâÒì]Ç≥ÇπÇÈ : é©ã@ÇâÒì]Ç≥ÇπÇÈ
{

}

void Boss::Attack2()	//à⁄ìÆèÍèäÇêßå¿Ç∑ÇÈ&ÉâÉìÉ_ÉÄÉVÉáÉbÉg : 
{

}

void Boss::Defence()	//É_ÉÅÅ[ÉWÇåyå∏Ç≥ÇπÇÈ : 
{

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
