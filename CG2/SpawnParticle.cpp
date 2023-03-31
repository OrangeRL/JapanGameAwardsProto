#include "SpawnParticle.h"
#include "MathFunc.h"
#include <random>

//乱数シード生成器
std::random_device seed_gen2;
//メルセンヌ・ツイスターの乱数エンジン
std::mt19937_64 engine2(seed_gen2());
//乱数範囲の指定
std::uniform_real_distribution<float> distRot2(MathFunc::Utility::Deg2Rad(-6), MathFunc::Utility::Deg2Rad(6));

void SpawnParticle::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection, Vector3 initPos)
{

	gameObject = new GameObject3D;
	//gameObject->PreLoadTexture(L"Resources/red.png");
	gameObject->SetViewProjection(viewProjection);
	gameObject->SetMatProjection(matProjection);
	gameObject->Initialize();

	gameObject->worldTransform.translation = initPos;
	moveVec.x = MathFunc::RNG(-100, 100) * 0.01f;
	//moveVec.y = RNG(-1,1);
	moveVec.y = 0.1f;
	moveVec.z = MathFunc::RNG(-100, 100) * 0.01f;

	gameObject->worldTransform.translation.x += moveVec.x;
	gameObject->worldTransform.translation.z += moveVec.z;

	gameObject->worldTransform.translation.y += MathFunc::RNG(0, 500) * 0.01f;
	gameObject->worldTransform.scale = initScale;

	gameObject->Update();

}

void SpawnParticle::Update(Vector3 initPos)
{
	move = { 0,0,0 };

	--timer;
	if (timer <= 0)
	{
		isDead = true;
	}

	if (isDead == false)
	{
		move.y += moveVec.y;

		if (gameObject->worldTransform.scale.x >= 0)
		{
			gameObject->worldTransform.scale.x -= 0.01f;
			gameObject->worldTransform.scale.y -= 0.01f;
			gameObject->worldTransform.scale.z -= 0.01f;
		}
	}

	gameObject->worldTransform.translation += move;

	gameObject->Update();
}

void SpawnParticle::Draw()
{
	if (gameObject->worldTransform.scale.x > 0.08f)
	{
		gameObject->Draw();
	}
}

void SpawnParticle::SetScale(Vector3 scale)
{
	gameObject->worldTransform.scale = scale;
	initScale = scale;
}