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
	case BossPhase::spown:	// �a��
		if (phaseTimer <= 0.0f) {
			phaseTimer = 300.0f;
			phase = BossPhase::attack;
		}
		break;
	case BossPhase::attack:	//�U��1 ���C��(�})�𒣂肻�����]������
		//-----------------------
		Attack();	//��������]���邾���̈ړ�
		if (phaseTimer <= 0.0f) {
			phaseTimer = 300.0f;
			phase = BossPhase::attack2;
		}
		break;
	case BossPhase::attack2:	//�U��2 �ړ��ꏊ���i�点�� & �����_���V���b�g
		// ���U������2�@
		Attack2();
		//-----------------------
		if (phaseTimer <= 0.0f) {
			phaseTimer = 100.0f;
			phase = BossPhase::defence;
		}
		break;
	case BossPhase::defence:	//���,�h��
		// ���h�䏈��	�_���[�W�y�����ʂ��o��
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

void Boss::Attack()	//���C����ɒe��W�J��]������ : ���@����]������
{
	if (gameObject->worldTransform.translation.x != 0.0f) {
		gameObject->worldTransform.translation.x = 0.0f;
	}
}

void Boss::Attack2()	//�ړ��ꏊ�𐧌�����&�����_���V���b�g : 
{
	
	gameObject->worldTransform.translation += moveSpeed;

	if (gameObject->worldTransform.translation.x >= 5.0f|| gameObject->worldTransform.translation.x <= -5.0f) {
		moveSpeed = -moveSpeed;
	}
}

void Boss::Defence()	//�_���[�W���y�������� : 
{
	if (gameObject->worldTransform.translation.x != 0.0f) {
		gameObject->worldTransform.translation.x = 0.0f;
	}
}

float Boss::Random(float minValue, float maxValue)
{
	//�V�[�h�l����������
	std::random_device rnd;
	//�����Z���k�E�c�C�X�^�����g���ė��������
	std::mt19937_64 mt64(rnd());
	//�͈͓��̗��U���z�����
	std::uniform_real_distribution<float> genRandFloat(minValue, maxValue);
	//���z�̒����琶�������������g����1�����l�����o��
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
