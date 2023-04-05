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

	//横方向のレールカメラの動き
	if (input->PushKey(DIK_J) && input->PushKey(DIK_L)) {
		if (flameLeft > 0) {
			flameLeft--;
		}

		if (flameRight > 0) {
			flameRight--;
		}
	}
	else if (input->PushKey(DIK_J)) {
		if (flameLeft < MAX_FLAME && flameRight == 0) {
			flameLeft++;
		}

		if (flameRight > 0 && flameLeft == 0) {
			flameRight--;
		}
		
	}
	else if (input->PushKey(DIK_L)) {
		if (flameLeft > 0 && flameRight == 0) {
			flameLeft--;
		}

		if (flameRight < MAX_FLAME && flameLeft == 0) {
			flameRight++;
		}
		
	}
	else {
		if (flameLeft > 0) {
			flameLeft--;
		}

		if (flameRight > 0) {
			flameRight--;
		}
	}

	rotation.y += easeInOutSine(flameLeft / MAX_FLAME) * 0.01f;
	worldTransform_.rotation.y -= easeInOutSine(flameLeft / MAX_FLAME) * 0.01f * 3.14f;
	rotation.y -= easeInOutSine(flameRight / MAX_FLAME) * 0.01f;
	worldTransform_.rotation.y += easeInOutSine(flameRight / MAX_FLAME) * 0.01f * 3.14f;

	//縦方向のレールカメラの動き
	if (input->PushKey(DIK_I) && input->PushKey(DIK_K)) {
		if (flameUp > 0) {
			flameUp--;
		}

		if (flameDown > 0) {
			flameDown--;
		}
	}
	else if (input->PushKey(DIK_I)) {

		if (flameUp > 0 && flameDown == 0) {
			flameUp--;
		}

		if (flameDown < MAX_FLAME && flameUp == 0) {
			flameDown++;
		}

	}
	else if (input->PushKey(DIK_K)) {
		if (flameUp < MAX_FLAME && flameDown == 0) {
			flameUp++;
		}

		if (flameDown > 0 && flameUp == 0) {
			flameDown--;
		}

	}
	else {
		if (flameUp > 0) {
			flameUp--;
		}

		if (flameDown > 0) {
			flameDown--;
		}
	}

	rotation.x += easeInOutSine(flameDown / MAX_FLAME) * 0.01f;
	worldTransform_.rotation.x -= easeInOutSine(flameDown / MAX_FLAME) * 0.01f * 3.14f;
	rotation.x -= easeInOutSine(flameUp / MAX_FLAME) * 0.01f;
	worldTransform_.rotation.x += easeInOutSine(flameUp / MAX_FLAME) * 0.01f * 3.14f;

	if (rotation.x >= 0.2f) {
		flameDown = 0.0f;
	}

	if (rotation.x <= -0.2f) {
		flameUp = 0.0f;
	}
	
	worldTransform_.UpdateMatWorld();
}