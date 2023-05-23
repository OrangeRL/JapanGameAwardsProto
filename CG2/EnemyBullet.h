#pragma once
#include "GameObject3D.h"
#include "Input.h"
#include <random>

class EnemyBullet
{
public:
	EnemyBullet();
	~EnemyBullet();

	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection, const wchar_t* textureFileName,Vector3 player, Vector3 enemy);
	void Update(bool enemy);
	void Draw();
	void OnCollision();
	WorldTransform GetWorldTransform();
	//生成する場所を設定
	Vector3 SetTransform(Vector3 transform);
	Vector3 SetScale(Vector3 scale);
	//弾の種類を設定
	int SetBullet(int bulletNum);
	void Aim(Vector3 player, Vector3 enemy);

	bool IsDead()const { return isDelete_; }
	bool GetIsDead();
	void SetIsDead(bool isDelete) { this->isDelete_ = isDelete; }

private:
	GameObject3D* gameObject = nullptr;
	//キー入力
	Input& input = Input::GetInstance();
	//弾の種類	0=プレイヤー狙い, 1=正面カーテン等
	int bulletNum;

	float bulletSpeed = 0.01f;
	float countSpeed;

	Vector3 posA;
	Vector3 posB;
	Vector3 posC;

	//消えるまでの時間
	// 60 * 消えるまでの時間:
	static const int32_t deleteTime = 60 * 50;
	//タイマー
	int32_t deleteTimer_ = deleteTime;
	//フラグ
	bool isDelete_ = false;
};

