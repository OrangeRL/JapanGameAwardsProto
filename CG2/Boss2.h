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
	void Update();
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

	//�s���ω�
	float phaseTimer = 3.0f;
	Boss2Phase phase = Boss2Phase::spown;

	bool isDead = true;
	int HP = 100;

};


