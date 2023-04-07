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

	//座標取得用
	WorldTransform GetWorldTransform();

	//死亡用
	void OnCollision();
private:
	//乱数取得用
	int Random(float minValue, float maxValue);
private:
	GameObject3D* gameObject = nullptr;
	static const int defenceValue = 2;
	GameObject3D* defenceObject[defenceValue] = {};

	//行動変化
	float phaseTimer = 3.0f;
	Boss2Phase phase = Boss2Phase::spown;

	bool isDead = true;

	float rushTimer = 10.0f;
	float shakeSpeed = 0;
	int speed = 5;
};


