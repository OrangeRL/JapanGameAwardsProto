#pragma once
#include "GameObject3D.h"
#include "EnemyBullet.h"
#include <memory>
#include <list>
enum class Phase {
	normal,
	move,
	leave,
};

class Enemy {
public:

	//コンストラクタ―
	Enemy();
	//デストラクタ
	~Enemy();

	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection, const wchar_t* textureFileName);


	void Update(ViewProjection* viewProjection, XMMATRIX* matProjection, int enemyNum);


	void Draw();
	
	void Reset();

	//反復
	void Repetition();
	//離脱
	void Leave(Vector3 leaveSpeedt, Vector3 leaveSpeedf);

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

	bool IsDead()const { return isDelete_; }
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

	//消えるまでの時間
	// 60 * 消えるまでの時間:
	static const int32_t deleteTime = 60 * 8;
	//タイマー
	int32_t deleteTimer_ = deleteTime;
	//フラグ
	bool isDelete_ = false;
};


