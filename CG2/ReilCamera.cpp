#include "ReilCamera.h"

void ReilCamera::Initialize(Vector3 pos, Vector3 rot) {
	worldTransform_.translation = pos;
	worldTransform_.rotation = rot;

	viewProjection_.Initialize();
}

void ReilCamera::Update(Input* input) {
	worldTransform_.translation.x += cosf(3.14f * worldTransform_.rotation.x);
	worldTransform_.translation.y += sinf(3.14f * worldTransform_.rotation.y);
	worldTransform_.translation.z += sinf(3.14f * worldTransform_.rotation.x);
	
	viewProjection_.eye = worldTransform_.translation;

	Vector3 forward(0, 0, 1);	

	Vector3 up(0, 1, 0);

	viewProjection_.target.x = viewProjection_.eye.x + cosf(3.14f * worldTransform_.rotation.x);
	viewProjection_.target.z = viewProjection_.eye.z + sinf(3.14f * worldTransform_.rotation.x);
	viewProjection_.target.y = viewProjection_.eye.y + sinf(3.14f * worldTransform_.rotation.y);

	if (input->PushKey(DIK_H)) {
		worldTransform_.rotation.x += 0.005f;
	}
	if (input->PushKey(DIK_K)) {
		worldTransform_.rotation.x -= 0.005f;
	}

	if (input->PushKey(DIK_U)) {
		worldTransform_.rotation.y += 0.005f;
	}
	if (input->PushKey(DIK_J)) {
		worldTransform_.rotation.y -= 0.005f;
	}

	worldTransform_.UpdateMatWorld();
	viewProjection_.UpdateView();
}