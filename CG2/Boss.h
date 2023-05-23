#pragma once
#include "GameObject3D.h"
#include "MathFunc.h"
#include <memory>
#include <list>
#include <random>

enum BossPhase {
	spown,
	attack,
	attack2,
	end,
};

class Boss
{
public:
	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection);
	void Update();
	void Draw();
	//↓行動指示用
	void Attack();
	void Attack2();
	void End();

	void Reset();

	float Random(float minValue, float maxValue);

	//座標取得用
	WorldTransform GetWorldTransform();
	BossPhase GetPhase();
	
	float GetAttackSpeed();
	float SetAttackSpeed(float speed);
	bool GetIsAttack();
	bool SetIsAttack(bool isAttack);
	bool GetIsDead();
	int GetHP();
	int SetHP(int HP) { return this->HP = HP; }
	bool GetSceneChange() {return sceneChange; }

	//死亡用
	void OnCollision();
private:
	GameObject3D* gameObject = nullptr;

	Vector3 moveSpeed = { 0.1f,0,0 };

	//行動変化
	float phaseTimer = 3.0f;
	BossPhase phase = BossPhase::spown;

	float responTimer = 10.0f;
	bool trueDead = false;
	bool isDead = false;
	int HP = 25;

	float attackSpeed = 200.0f;
	bool isAttack = false;

	bool sceneChange = false;
};


