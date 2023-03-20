#include "Boss.h"

void Boss::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection)
{
	gameObject = new GameObject3D();
	gameObject->PreLoadTexture(L"Resources/red.png");
	gameObject->SetViewProjection(viewProjection);
	gameObject->SetMatProjection(matProjection);
	gameObject->Initialize();


	gameObject->worldTransform.translation = { 0 , 0 , 600 };
	gameObject->worldTransform.scale = { 5 , 5 , 5 };
}

void Boss::Update()
{
	gameObject->Update();
}

void Boss::Draw()
{
	gameObject->Draw();
}
