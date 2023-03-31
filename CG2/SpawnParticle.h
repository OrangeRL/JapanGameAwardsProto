#pragma once
#include "GameObject3D.h"
#include "Player.h"

class SpawnParticle
{
public:
	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection, Vector3 initPos);
	void Update(Vector3 initPos);
	void Draw();

	bool IsDead() { return isDead; };

	void SetScale(Vector3 scale);
private:
	int	timer = 60;

	Vector3 move = { 0,0,0 };
	Vector3 moveVec = { 0,0,0 };

	Vector3 initScale = { 0.5f,0.5f,0.5f };

	bool isDead = false;

	GameObject3D* gameObject;

	Vector3 velocity = {};
	Vector3 angle= {};

	const float speed = 1;

	int particleOff = { 0 };

	Player* player = nullptr;
	Enemy* enemy = nullptr;
};
