#pragma once
#include "GameObject3D.h"
#include "EnemyBullet.h"
#include "Rhythm.h"
#include "SpawnParticleManager.h"
#include "Particle2.h"
#include <memory>
#include <list>
enum class Phase {
	Attack,
	CoolDown,
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
	void SetPosition(Vector3 pos) { position = pos; }
	//反復
	void Repetition();
	//離脱
	void Leave(Vector3 leaveSpeedt, Vector3 leaveSpeedf,int enemyNum);

	void Spawn();
	void AimCheck();

	void OnCollision(Rhythm* rhythm);

	/*const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets; }*/

	//座標取得用
	WorldTransform GetWorldTransform();
	//生成される場所を設定
	WorldTransform Settransform(float x,float y,float z);
	WorldTransform Settransform(Vector3 x);
	float GetAttackSpeed();
	float SetAttackSpeed(float speed);
	bool GetCoolDown();
	//速度設定
	Vector3 SetSpeed(float x, float y, float z);
	void SetBulletNum(int32_t bulletNum);
	void SetMoveNum(int32_t moveNum);
	int32_t GetBulletNum();

	bool GetIsAttack();
	bool SetIsAttack(bool isAttack);
	bool GetIsDead();
	bool SetisDead(bool isDead) {return isDelete_ = isDead; }

	int GetSpownFlag();
	int GetAimFlag();
	int SetSpownFlag(int spownFlag) { return this->spawnFlag = spownFlag; }

	bool IsDead()const { return isDelete_; }

	Phase GetPhase();

private:
	Phase phase = Phase::Attack;
	//行動変化
	float phaseTimer = 300.0f;
	
	GameObject3D* gameObject = nullptr;
	GameObject3D* aimObject = nullptr;

	Vector3 position = { 10.0f,5.0f,0.0f };

	Vector3 moveSpeed = { 0.0f,0.0f,0.0f };
	float attackSpeed = 100.0f;
	bool isAttack = false;
	//クールタイム
	bool isCoolDown = true;
	float coolTime = 1.0f;
	
	//消えるまでの時間
	// 60 * 消えるまでの時間:
	static const int32_t deleteTime = 60 * 15;
	//タイマー
	int32_t deleteTimer_ = deleteTime;
	//フラグ
	bool isDelete_ = false;

	int32_t useBullet;
	int32_t moveNum;

	SpawnParticleManager spManager;
	Particle2 pManager;

	int spawnFlag = false;
	int aimFlag = false;
};


