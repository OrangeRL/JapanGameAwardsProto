#include "Goal.h"

Goal::Goal() {

}

Goal::~Goal() {
	delete gameObject;
}

void Goal::Initialize(ViewProjection* viewProjection , XMMATRIX* matProjection) {
	for (int i = 0; i < 2; i++) {
		gameObject = new GameObject3D();
		gameObject->PreLoadTexture(L"Resources/black1x1.png");
		gameObject->SetViewProjection(viewProjection);
		gameObject->SetMatProjection(matProjection);
		gameObject->Initialize();

		gameObject->worldTransform.translation = { 39 , 0 , 38 };
		gameObject->worldTransform.scale = { 2 , 2 , 2 };
	}
}

void Goal::Update() {

	

	gameObject->Update();

}

void Goal::Draw() {
	gameObject->Draw();
}

WorldTransform Goal::GetWorldTransform() {
	return gameObject->worldTransform;
}