#pragma once
#include "GameObject3D.h"
#include "EnemyBullet.h"
#include <memory>
#include <list>
enum class Phase {
	normal,
	move,
};

class Enemy {
public:

	//コンストラクタ―
	Enemy();
	//デストラクタ
	~Enemy();

	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection, const wchar_t* textureFileName);


	void Update(ViewProjection* viewProjection, XMMATRIX* matProjection, const wchar_t* textureFileName, int enemyNum);


	void Draw();
	
	void Reset();

	//反復
	void Repetition();

	void CoolTime();

	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets; }
	//座標取得用
	WorldTransform GetWorldTransform();
	//生成される場所を設定
	WorldTransform Settransform(float x,float y,float z);
	float GetAttackSpeed();
	float SetAttackSpeed(float speed);
	bool GetCoolDown();
	//速度設定
	Vector3 SetSpeed(float x, float y, float z);

	bool GetIsAttack();
	bool SetIsAttack(bool isAttack);

	Phase GetPhase();
private:
	Phase phase = Phase::normal;

	GameObject3D* gameObject = nullptr;
	std::list<std::unique_ptr<EnemyBullet>> bullets;
	EnemyBullet* enemyBullet = nullptr;

	Vector3 moveSpeed = { 0,0,0 };
	float attackSpeed = 100.0f;
	bool isAttack = false;
	//クールタイム
	bool isCoolDown = true;
	float coolTime = 150.0f;
	//行動変化
	float phaseTimer = 300.0f;
};


