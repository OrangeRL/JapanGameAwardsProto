#include "ReilCamera.h"

void ReilCamera::Initialize(Vector3 pos, Vector3 rot) {
	worldTransform_.translation = pos;
	worldTransform_.rotation = rot;
	worldTransform_.scale = { 1.0f,1.0f,1.0f };

	viewProjection_.Initialize();
}

void ReilCamera::Update(Input* input) {
	worldTransform_.translation.x += cosf(3.14f * rotation.y) * 0.1f;
	worldTransform_.translation.y += sinf(3.14f * rotation.x) * 0.1f;
	worldTransform_.translation.z += sinf(3.14f * rotation.y) * 0.1f;
	
	viewProjection_.eye = worldTransform_.translation;

	viewProjection_.target.x = viewProjection_.eye.x + cosf(3.14f * rotation.y);
	viewProjection_.target.z = viewProjection_.eye.z + sinf(3.14f * rotation.y);
	viewProjection_.target.y = viewProjection_.eye.y + sinf(3.14f * rotation.x);

	if (input->PushKey(DIK_J)) {
		rotation.y += 0.005f;
		worldTransform_.rotation.y -= rotSpeeed.y * 3.14;
	}
	if (input->PushKey(DIK_L)) {
		rotation.y -= 0.005f;
		worldTransform_.rotation.y += rotSpeeed.y * 3.14;
	}

	if (input->PushKey(DIK_I)) {
		rotation.x += 0.005f;
		worldTransform_.rotation.x -= cosf(3.14f * rotation.x) * 0.01f;
	}
	if (input->PushKey(DIK_K)) {
		rotation.x -= 0.005f;
		worldTransform_.rotation.x += cosf(3.14f * rotation.x) * 0.01f;
	}

	worldTransform_.UpdateMatWorld();
	viewProjection_.UpdateView();
}