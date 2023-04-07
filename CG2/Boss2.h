#pragma once
#include "GameObject3D.h"
#include "MathFunc.h"
#include <memory>
#include <list>
#include <random>

enum Boss2Phase {
	respown,
	move,
	rush,
	limit,
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
	Boss2Phase phase = Boss2Phase::respown;

	bool isDead = true;

	float moveCoolDown = 100.0f;
	float rushTimer = 30.0f;
	float shakeSpeed = 0;
	int speed = 5;
};


