#include "BossBullet.h"

void BossBullet::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection, Vector3 player, Vector3 enemy, BossPhase phase)
{
	gameObject = new GameObject3D();
	gameObject->PreLoadTexture(L"Resources/white1x1.png");
	gameObject->SetViewProjection(viewProjection);
	gameObject->SetMatProjection(matProjection);
	gameObject->Initialize();

	Aim(player, enemy);
	if (phase == attack2) {
		if (isSelect = false)
		{
			selectPos = Random(0, 2);
			isSelect = true;
		}
		if (selectPos == 0)
		{
			gameObject->worldTransform.scale = { 15,1,1 };
			gameObject->worldTransform.translation = { 0,9,player.z };
		}
		else if (selectPos == 1)
		{
			gameObject->worldTransform.scale = { 15,1,1 };
			gameObject->worldTransform.translation = { 0,-9,player.z };
		}
		else
		{
			gameObject->worldTransform.scale = { 15,1,1 };
			gameObject->worldTransform.translation = { 0,0,player.z };
		}
	}
}

void BossBullet::Update(BossPhase phase, Vector3 player)
{
	if (phase == attack) {
		gameObject->worldTransform.scale = { 1,1,1 };
		gameObject->worldTransform.translation -= posC;
	}
	else if (phase == attack2) {
		moveSpeed += speed;
		if (moveSpeed.y >= 1.5f)
		{
			
			speed = -speed;
		}
		gameObject->worldTransform.translation += speed;

	}

	if (--deleteTimer_ <= 0) {
		isDelete_ = true;
	}

	gameObject->Update();
}

void BossBullet::Draw()
{
	gameObject->Draw();
}

WorldTransform BossBullet::GetWorldTransform()
{
	return gameObject->worldTransform;
}

//WorldTransform BossBullet::GetWorldTransform()
//{
//	return gameObject->worldTransform;
//}

Vector3 BossBullet::SetTransform(Vector3 transform)
{
	this->gameObject->worldTransform.translation = transform;
	return this->gameObject->worldTransform.translation;
}

Vector3 BossBullet::SetScale(Vector3 scale)
{
	this->gameObject->worldTransform.scale = scale;
	return this->gameObject->worldTransform.scale;
}

void BossBullet::Aim(Vector3 player, Vector3 enemy)
{
	posA = player;
	posB = enemy;
	posC = posA - posB;
	posC.nomalize();
	posC *= -1.5f;
}

int BossBullet::Random(int minValue, int maxValue)
{
	//シード値乱数生成器
	std::random_device rnd;
	//メルセンヌ・ツイスタ方を使って乱数を作る
	std::mt19937_64 mt64(rnd());
	//範囲内の離散分布を作る
	std::uniform_real_distribution<int> genRandFloat(minValue, maxValue);
	//分布の中から生成した乱数を使って1つだけ値を取り出す
	return genRandFloat(mt64);
}
