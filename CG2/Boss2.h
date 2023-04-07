#pragma once
#include "GameObject3D.h"
#include "MathFunc.h"
#include <memory>
#include <list>
#include <random>

enum Boss2Phase {
	spown,
	attack,
	attack2,
	defence,
};

class Boss2
{
public:
	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection);
	void Update(Vector3 player);
	void Draw();

	//���W�擾�p
	WorldTransform GetWorldTransform();

	//���S�p
	void OnCollision();
private:
	//�����擾�p
	int Random(float minValue, float maxValue);
private:
	GameObject3D* gameObject = nullptr;
	static const int defenceValue = 2;
	GameObject3D* defenceObject[defenceValue] = {};

	//�s���ω�
	float phaseTimer = 3.0f;
	Boss2Phase phase = Boss2Phase::spown;

	bool isDead = true;

	float rushTimer = 10.0f;
	float shakeSpeed = 0;
	int speed = 5;
};


