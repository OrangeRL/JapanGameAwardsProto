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
	gameObject->worldTransform.translation = { 0 + shakeSpeed , 0 , 1500 };
	for (int i = 0; i < defenceValue; i++) {
		defenceObject[i] = new GameObject3D();
		//defenceObject[i]->PreLoadModel();
		defenceObject[i]->PreLoadTexture(L"resources/red1x1.png");
		defenceObject[i]->SetViewProjection(viewProjection);
		defenceObject[i]->SetMatProjection(matProjection);
		defenceObject[i]->Initialize();
	}
	
}

void Boss2::Update(Vector3 player)
{
	phaseTimer -= 0.1f;
	switch (phase)
	{
		//1回のみ
	case Boss2Phase::spown:	// 誕生
		if (phaseTimer <= 0.0f) {
			isDead = false;
			phaseTimer = 500.0f;
			phase = Boss2Phase::attack;
		}
		break;
	case Boss2Phase::attack:	//
		gameObject->worldTransform.translation.x += speed;
		if (gameObject->worldTransform.translation.x >= 10.0f|| gameObject->worldTransform.translation.x <= -10.0f) {
			speed = -speed;
		}

		if (phaseTimer <= 0.0f) {
			isDead = false;
			phaseTimer = 500.0f;
			gameObject->worldTransform.translation.x = 0.0f;
			phase = Boss2Phase::attack2;
		}
		break;
	case Boss2Phase::attack2:	//前方から突進
		if (rushTimer > 0.0f) {
			rushTimer--;
			shakeSpeed = Random(-2, 2);
		}
		else if (rushTimer <= 0.0f && gameObject->worldTransform.translation.z != player.z - 5) {
			gameObject->worldTransform.translation.z -= 5.0f;
			if (gameObject->worldTransform.translation.z <= player.z - 5)
			{
				gameObject->worldTransform.translation.z = 1500;
				rushTimer = 10.0f;
			}
		}

		if (phaseTimer <= 0.0f) {
			isDead = false;
			phaseTimer = 500.0f;
			rushTimer = 10.0f;
			gameObject->worldTransform.translation.z = 1500;
			phase = Boss2Phase::defence;
		}

		break;
	case Boss2Phase::defence:	//制限
		for (int i = 0; i < defenceValue; i++) {
			defenceObject[i]->worldTransform.scale.x += 0.3f;
			defenceObject[0]->worldTransform.translation.y = 5.5f;
			defenceObject[1]->worldTransform.translation.y = -5.5f;
			defenceObject[i]->worldTransform.translation.z = player.z;
		}

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
	//シード値乱数生成器
	std::random_device rnd;
	//メルセンヌ・ツイスタ方を使って乱数を作る
	std::mt19937_64 mt64(rnd());
	//範囲内の離散分布を作る
	std::uniform_real_distribution<float> genRandFloat(minValue, maxValue);
	//分布の中から生成した乱数を使って1つだけ値を取り出す
	return genRandFloat(mt64);
}
