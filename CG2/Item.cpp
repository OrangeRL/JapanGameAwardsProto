#include "Item.h"

Item::~Item() {
	delete gameObject;
}

void Item::Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection, const wchar_t* textureFileName, Vector3 pos, int weapon) {
	gameObject = new GameObject3D();
	gameObject->PreLoadModel("Resources/tofu/tofu.obj");
	gameObject->PreLoadTexture(textureFileName);
	gameObject->SetViewProjection(viewProjection);
	gameObject->SetMatProjection(matProjection);
	gameObject->Initialize();
	gameObject->worldTransform.translation = pos;

	this->weapon = weapon;

}

void Item::Update() {

	//‰ñ“]‚³‚¹‚é
	gameObject->worldTransform.rotation += rotation;
	if (gameObject->worldTransform.rotation.y > 2.0f * 3.14f) {
		gameObject->worldTransform.rotation.y = 0.0f;
	}
	
	gameObject->Update();
}

void Item::Draw() {

	if (isDead == false) {
		gameObject->Draw();
	}
}

void Item::OnCollision() {
	isDead = true;
}

Vector3 Item::GetPos() {
	return {
		gameObject->worldTransform.matWorld.m[3][0],
		gameObject->worldTransform.matWorld.m[3][1],
		gameObject->worldTransform.matWorld.m[3][2]
	};
}
