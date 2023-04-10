#pragma once
#include "GameObject3D.h"
#include "MathFunc.h"
#include "Input.h"
#include "Rhythm.h"

class PlayerBullet {
public:
	//コンストラクタ
	PlayerBullet();
	//デストラクタ
	~PlayerBullet();

	//初期化
	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection, Vector3 playerPos, Vector3 bossPos, Weapons weapon);
	//更新
	void Update(Vector3 vec, float shotAngle);
	//描画
	void Draw();

	bool IsDead() const { return isDead_; }

	int GetIsDead();
	Vector3 GetAngle();
	WorldTransform GetWorldTransform();

	void OnCollision();

	//攻撃
	void AttackPress();

	void Attack(Vector3 playerPos, Vector3 bossPos, Vector3 vec, float shotAngle);
private:	//メンバ関数
	
private:	//メンバ変数
	//弾数
	const int bulletNum = 10;
	//ゲームオブジェクト
	GameObject3D* gameObject;
	GameObject3D* gameObjectSub[2];
	//表示フラグ
	bool isShot;
	//カウント
	int bulletCount = 0;
	//弾の移動速度
	float speed = -5.0f;
	//弾のベクトル
	Vector3 velocity;
	Vector3 vec1;
	Vector3 vec2;
	//弾の移動範囲
	float canMoveArea = 120;

	//寿命
	static const int32_t kLifeTime = 60 * 1;
	//デスタイマー
	int deathTimer_ = kLifeTime;
	//デスフラグ
	int isDead_ = false;
	Vector3 angle = {};

	Vector3 newEnemyPos = {};
	Vector3 newPlayerPos = {};

	//キー入力
	Input& input = Input::GetInstance();

	Weapons weapon;
};