#pragma once
#include "GameObject3D.h"
#include "Input.h"
#include "MathFunc.h"

class Map;
class Goal;
class Enemy;

class Player {
public:
	//コンストラクタ
	Player();
	//デストラクタ
	~Player();

	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection);

	void Update();

	void Draw();

	void Reset();

	//アクセッサ
	void SetMap(Map* map);
	void SetGoal(Goal* goal);
	void SetEnemy(Enemy* enemy);

	int GetIsGoal();
	void SetIsGoal(int flag);

	int GetIsEnemy();
	void SetIsEnemy(int flag);

	int GetIsDead();

	Vector3 GetAngle();
	ViewProjection* GetViewProjection() { return viewProjection; }
	WorldTransform GetWorldTransform();
Vector3 centerVec = { 0,0,0 };
Vector3 angle = {};
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

	ViewProjection* viewProjection;

	Vector3 velocity = {};
	//正面ベクトル
	
	const float accelaration = 0.0025f;

	float moveSpeed = 0;

	const float maxSpeed = 0.5;
	float ct = 0;
	const float g = 3;
	float timer = 0;
	int jumpFlag = 0;
	Vector3 target;

	int isDead = false;

	int isHitMap = false;

	int isGoal = false;
	int isEnemy = false;

	Map* map = nullptr;
	Goal* goal = nullptr;
	Enemy* enemy = nullptr;

	
	Vector3 move = {};
};

