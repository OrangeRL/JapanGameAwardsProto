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
		case BossPhase::spown:	// íaê∂
			if (phaseTimer <= 0.0f) {
				phaseTimer = 300.0f;
				phase = BossPhase::attack;
			}
			break;
		case BossPhase::attack:	//çUåÇ1 ÉâÉCÉì(é})Çí£ÇËÇªÇÍÇâÒì]Ç≥ÇπÇÈ
			//-----------------------
			Attack();	//é©ï™Ç™âÒì]Ç∑ÇÈÇæÇØÇÃà⁄ìÆ
			if (phaseTimer <= 0.0f) {
				phaseTimer = 300.0f;
				phase = BossPhase::attack2;
			}
			break;
		case BossPhase::attack2:	//çUåÇ2 à⁄ìÆèÍèäÇçiÇÁÇπÇÈ & ÉâÉìÉ_ÉÄÉVÉáÉbÉg
			// Å´çUåÇèàóù2
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

void Boss::Attack()	//ÉâÉCÉìèÛÇ…íeÇìWäJâÒì]Ç≥ÇπÇÈ : é©ã@ÇâÒì]Ç≥ÇπÇÈ
{
	if (gameObject->worldTransform.translation.x != 0.0f) {
		gameObject->worldTransform.translation.x = 0.0f;
	}
}

void Boss::Attack2()	//à⁄ìÆèÍèäÇêßå¿Ç∑ÇÈ&ÉâÉìÉ_ÉÄÉVÉáÉbÉg : 
{
	
}

void Boss::End()	//éÄñSââèo: 
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
	gameObject->worldTransform.scale = { 3 , 3 , 3 };
	gameObject->worldTransform.rotation = { 0,90,0 };
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
