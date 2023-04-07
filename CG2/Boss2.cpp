#include "Boss2.h"

void Boss2::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection)
{
	gameObject = new GameObject3D();
	gameObject->PreLoadModel("Resources/tree/tree.obj");
	gameObject->PreLoadTexture(L"Resources/tree/tree.png");
	gameObject->SetViewProjection(viewProjection);
	gameObject->SetMatProjection(matProjection);
	gameObject->Initialize();

	gameObject->worldTransform.scale = { 3 , 3 , 3 };
	gameObject->worldTransform.rotation = { 0,90,0 };
	gameObject->worldTransform.translation = { 0 , 0 , 1500 };
}

void Boss2::Update()
{
	phaseTimer -= 0.1f;
	switch (phase)
	{
		//1��̂�
	case Boss2Phase::spown:	// �a��


		if (phaseTimer <= 0.0f) {
			isDead = false;
			phaseTimer = 500.0f;
			phase = Boss2Phase::attack;
		}
		break;
	case Boss2Phase::attack:	//�������Ȃ�

		if (phaseTimer <= 0.0f) {
			isDead = false;
			phaseTimer = 500.0f;
			phase = Boss2Phase::attack2;
		}
		break;
	case Boss2Phase::attack2:	//�O������ːi


		if (phaseTimer <= 0.0f) {
			isDead = false;
			phaseTimer = 500.0f;
			phase = Boss2Phase::defence;
		}

		break;
	case Boss2Phase::defence:	//����

		if (phaseTimer <= 0.0f) {
			isDead = false;
			phaseTimer = 500.0f;
			phase = Boss2Phase::attack;
		}
		break;
	}
	gameObject->Update();
}

void Boss2::Draw()
{
	if (isDead == false) {
		gameObject->Draw();
	}
}

WorldTransform Boss2::GetWorldTransform()
{
	return gameObject->worldTransform;
}

void Boss2::OnCollision()
{
	isDead = true;
}

int Boss2::Random(float minValue, float maxValue)
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
