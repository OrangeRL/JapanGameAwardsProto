#pragma once
#include "GameObject3D.h"
#include "Input.h"
#include "MathFunc.h"
#include "PlayerBullet.h"
#include "Rhythm.h"
#include "Pattern2.h"
#include "EnemyBullet.h"
#include <memory>
#include<list>


class Map;
class Goal;
class Enemy;
class GameScene;

class Player {
public:
	//コンストラクタ
	Player();
	//デストラクタ
	~Player();

	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection);

	void Update(WorldTransform wt, Vector3 vec);

	void Aim(Vector3 player, Vector3 enemy, Vector3 vec, float shotAngle);

	void Draw();

	void Reset();

	void OnCollision(Rhythm* rhythm);

	//アクセッサ
	void SetMap(Map* map);
	void SetGoal(Goal* goal);
	void SetEnemy(Enemy* enemy);
	void NewBullet(ViewProjection* viewProjection, XMMATRIX* matProjection, Vector3 enemyPos, Vector3 playerPos, Weapons weapon);

	int GetIsGoal();
	void SetIsGoal(int flag);
	void SetPos(Vector3 pos) { gameObject->worldTransform.translation = pos; }

	int GetIsEnemy();
	void SetIsEnemy(int flag);

	int GetIsDead();
	int GetLife() { return life; }

	Vector3 GetAngle();
	ViewProjection* GetViewProjection() { return viewProjection; }
	WorldTransform GetWorldTransform();
	Vector3 GetPos();
	Vector3 GetAimPos();
	int GetIsAimHit();
	void AimHit();
	void NotAimHit();

	Vector3 centerVec = { 0,0,0 };
	Vector3 angle = {};


	//弾リストを取得
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }
	const std::list<std::unique_ptr<Pattern2>>& GetAim() { return bulletsAim_; }
	//メンバ関数
private:
	void Rotate();
	void Move();
	void Collision();


	//メンバ変数
private:
	//キー入力
	Input& input = Input::GetInstance();

	//ゲームオブジェクト
	GameObject3D* gameObject = nullptr;
	GameObject3D* aimObject = nullptr;
	GameObject3D* aimObject2 = nullptr;
	GameObject3D* aimObject3 = nullptr;
	GameObject3D* aimObject4 = nullptr;

	ViewProjection* viewProjection;

	Vector3 velocity = {};

	int life = 3;
	bool isInvincible = 0;
	int invincibleTimer = 120;

	const float accelaration = 0.0025f;

	float moveSpeed = 0;
	float distance = 1;
	const float maxSpeed = 0.5;
	float ct = 0;
	const float g = 3;
	float timer = 0;
	int jumpFlag = 0;
	Vector3 target;

	int isDead = false;

	int isAimHit = false;

	int aimTouch = false;

	int isHitMap = false;

	int isGoal = false;
	int isEnemy = false;
	bool newBulletFired = false;
	Map* map = nullptr;
	Goal* goal = nullptr;
	Enemy* enemy = nullptr;

	Vector3 enemyPos = {};
	Vector3 playerPos = {};

	Vector3 move = {};
	const Vector2 moveLimit = { 15.0f,10.0f };

	ViewProjection* viewProjection_;
	XMMATRIX* matProjection_;
	//弾
	std::list<std::unique_ptr<PlayerBullet>>bullets_;
	std::list<std::unique_ptr<PlayerBullet>>bulletsSub_[2];
	PlayerBullet* playerBullet = nullptr;

	std::list<std::unique_ptr<Pattern2>>bulletsAim_;
	Pattern2* playerBulletAim = nullptr;

	//3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;

	Vector3 posA;
	Vector3 posB;
	Vector3 posC;
};
