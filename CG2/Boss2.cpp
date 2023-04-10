#include "Boss2.h"

void Boss2::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection)
{
	gameObject = new GameObject3D();
	gameObject->PreLoadModel("Resources/tree/tree.obj");
	gameObject->PreLoadTexture(L"Resources/tree/tree.png");
	gameObject->SetViewProjection(viewProjection);
	gameObject->SetMatProjection(matProjection);
	gameObject->Initialize();

	gameObject->worldTransform.scale = { 2 , 2 , 2 };
	gameObject->worldTransform.rotation = { 0,90,0 };
	gameObject->worldTransform.translation = { 0 , 0 , 1500 };
	for (int i = 0; i < defenceValue; i++) {
		defenceObject[i] = new GameObject3D();
		//defenceObject[i]->PreLoadModel();
		defenceObject[i]->PreLoadTexture(L"resources/red1x1.png");
		defenceObject[i]->SetViewProjection(viewProjection);
		defenceObject[i]->SetMatProjection(matProjection);
		defenceObject[i]->Initialize();

		defenceObject[i]->worldTransform.scale = { 2,2,2 };
		defenceObject[i]->worldTransform.translation = gameObject->worldTransform.translation;
	}
	
}

void Boss2::Update(Vector3 player)
{
	phaseTimer -= 0.1f;
	switch (phase)
	{
		//1��̂�
	case Boss2Phase::respown:	// �a��
		if (phaseTimer <= 0.0f) {
			isDead = false;
			phaseTimer = 500.0f;
			phase = Boss2Phase::move;
		}
		break;

	case Boss2Phase::move:	//�ړ�
		moveCoolDown--;
		if (moveCoolDown <= 0.0f) {
			gameObject->worldTransform.translation.x += speed;
			if (gameObject->worldTransform.translation.x >= 15.0f || gameObject->worldTransform.translation.x <= -15.0f) {
				speed = -speed;
			}
			moveCoolDown = 30.0f;
		}


		if (phaseTimer <= 0.0f) {
			isDead = false;
			phaseTimer = 500.0f;
			gameObject->worldTransform.translation.x = 0.0f;
			phase = Boss2Phase::rush;
		}
		break;
	case Boss2Phase::rush:	//�O������ːi
		if (rushTimer >= 0.0f) {
			shakeSpeed = Random(-0.1f, 0.01f);
		}
		gameObject->worldTransform.translation = { shakeSpeed,shakeSpeed,0.0f };
		if (phaseTimer <= 0.0f) {
			isDead = false;
			phaseTimer = 500.0f;
			rushTimer = 10.0f;
			gameObject->worldTransform.translation = { 0, 0,1500 };
			phase = Boss2Phase::limit;
		}

		break;
	case Boss2Phase::limit:	//����
		for (int i = 0; i < defenceValue; i++) {
			defenceObject[i]->worldTransform.scale.x += 0.3f;
			defenceObject[0]->worldTransform.translation.y = 5.5f;
			defenceObject[1]->worldTransform.translation.y = -5.5f;
			defenceObject[i]->worldTransform.translation.z = player.z;
		}

		if (phaseTimer <= 0.0f) {
			isDead = false;
			phaseTimer = 500.0f;
			phase = Boss2Phase::move;
		}
		break;
	}
	gameObject->Update();
}

void Boss2::Draw()
{
	if (isDead == false) {
		gameObject->Draw();
		for (int i = 0; i < defenceValue; i++) {
			defenceObject[i]->Draw();
		}
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
