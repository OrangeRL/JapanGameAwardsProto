#pragma once
#include "GameObject3D.h"

class Enemy {
public:

	//コンストラクタ
	Enemy();
	//デストラクタ
	~Enemy();

	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection);

	void Update(Vector3 playerPos, Vector3 bossPos);

	void Draw();

	void Reset();

	WorldTransform GetWorldTransform();

private:
	GameObject3D* gameObject = nullptr;

	float moveSpeed = 0;

	const float g = 3;

	float timer = 0;

	float ct = 0;

	const float speed = -0.5f;

	Vector3 velocity;
};


