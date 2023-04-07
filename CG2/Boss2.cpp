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
		//1âÒÇÃÇ›
	case Boss2Phase::spown:	// íaê∂
		if (phaseTimer <= 0.0f) {
			isDead = false;
			phaseTimer = 300.0f;
			phase = Boss2Phase::attack;
		}
		break;
	case Boss2Phase::attack:

		if (phaseTimer <= 0.0f) {
			phaseTimer = 300.0f;
			phase = Boss2Phase::attack2;
		}
		break;
	case Boss2Phase::attack2:

		if (phaseTimer <= 0.0f) {
			phaseTimer = 100.0f;
			phase = Boss2Phase::defence;
		}
		break;
	case Boss2Phase::defence:

		if (phaseTimer <= 0.0f) {
			phaseTimer = 300.0f;
			phase = Boss2Phase::attack;
		}
		break;
	}
	gameObject->Update();
}

void Boss2::Draw()
{
	gameObject->Draw();
}

WorldTransform Boss2::GetWorldTransform()
{
    return WorldTransform();
}

void Boss2::OnCollision()
{
}
