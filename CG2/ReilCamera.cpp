#include "ReilCamera.h"

void ReilCamera::Initialize(Vector3 pos, Vector3 rot) {
	worldTransform_.translation = pos;
	worldTransform_.rotation = rot;
	worldTransform_.scale = { 1.0f,1.0f,1.0f };

	viewProjection_.Initialize();
}

void ReilCamera::Update(Input* input,float wave) {
	if (wave != 3) {
		worldTransform_.translation.x += cosf(3.14f * rotation.y) * 0.1f;
		worldTransform_.translation.y += sinf(3.14f * rotation.x) * 0.1f;
		worldTransform_.translation.z += sinf(3.14f * rotation.y) * 0.1f;
	}
	
	viewProjection_.eye = worldTransform_.translation;
	viewProjection_.target.y = worldTransform_.translation.y - a;

	viewProjection_.target.x = worldTransform_.translation.x + cosf(3.14f * rotation.y);
	viewProjection_.target.z = worldTransform_.translation.z + sinf(3.14f * rotation.y);
	viewProjection_.target.y = worldTransform_.translation.y + sinf(3.14f * rotation.x);

	rotation.y += MathFunc::easeInOutSine(flameLeft / MAX_FLAME) * 0.01f;
	worldTransform_.rotation.y -= MathFunc::easeInOutSine(flameLeft / MAX_FLAME) * 0.01f * 3.14f;
	rotation.y -= MathFunc::easeInOutSine(flameRight / MAX_FLAME) * 0.01f;
	worldTransform_.rotation.y += MathFunc::easeInOutSine(flameRight / MAX_FLAME) * 0.01f * 3.14f;

	rotation.x += MathFunc::easeInOutSine(flameDown / MAX_FLAME) * 0.01f;
	worldTransform_.rotation.x -= MathFunc::easeInOutSine(flameDown / MAX_FLAME) * 0.01f * 3.14f;
	rotation.x -= MathFunc::easeInOutSine(flameUp / MAX_FLAME) * 0.01f;
	worldTransform_.rotation.x += MathFunc::easeInOutSine(flameUp / MAX_FLAME) * 0.01f * 3.14f;

	if (rotation.x >= 0.2f) {
		flameDown = 0.0f;
	}

	if (rotation.x <= -0.2f) {
		flameUp = 0.0f;
	}
	
	worldTransform_.UpdateMatWorld();
}