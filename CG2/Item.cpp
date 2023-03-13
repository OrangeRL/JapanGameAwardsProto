#include "Item.h"

Item::~Item() {
	delete gameObject;
}

void Item::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection, const wchar_t* textureFileName, Vector3 pos) {
	gameObject = new GameObject3D();
	gameObject->PreLoadModel("Resources/tofu/tofu.obj");
	gameObject->PreLoadTexture(textureFileName);
	gameObject->SetViewProjection(viewProjection);
	gameObject->SetMatProjection(matProjection);
	gameObject->Initialize();
	gameObject->worldTransform.translation = pos;

}

void Item::Update() {
	gameObject->worldTransform.translation = { 0.0f,0.0f,0.0f };
	gameObject->worldTransform.rotation.y = rotation.y;
	gameObject->Update();

	gameObject->worldTransform.rotation.y = 0.0f;
	gameObject->worldTransform.translation = { 0.0f,0.0f,10.0f };
	gameObject->Update();

	rotation.y += 0.01f;
}

void Item::Draw() {

	if (isAlive) {
		gameObject->Draw();
	}
}

void Item::OnCollision() {
	isAlive = false;
}
