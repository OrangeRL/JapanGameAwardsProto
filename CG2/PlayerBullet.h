#pragma once
#include "GameObject3D.h"
#include "MathFunc.h"
#include "Input.h"

class PlayerBullet {
public:
	//コンストラクタ
	PlayerBullet();
	//デストラクタ
	~PlayerBullet();

	//初期化
	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection, Vector3 playerPos, Vector3 bossPos);
	//更新
	void Update();
	//描画
	void Draw();

	bool IsDead() const { return isDead_; }

	int GetIsDead();
	Vector3 GetAngle();
	WorldTransform GetWorldTransform();

	void OnCollision();

	//攻撃
	void AttackPress();

	void Attack(Vector3 playerPos, Vector3 bossPos);
private:	//メンバ関数
	
private:	//メンバ変数
	//弾数
	const int bulletNum = 10;
	//ゲームオブジェクト
	GameObject3D* gameObject;
	//表示フラグ
	bool isShot;
	//カウント
	int bulletCount = 0;
	//弾の移動速度
	const float speed = -1.0f;
	//弾のベクトル
	Vector3 velocity;
	//弾の移動範囲
	float canMoveArea = 120;

	//寿命
	static const int32_t kLifeTime = 60 * 5;
	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	int isDead_ = false;
	Vector3 angle = {};

	Vector3 newEnemyPos = {};
	Vector3 newPlayerPos = {};

	//キー入力
	Input& input = Input::GetInstance();
};