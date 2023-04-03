#pragma once
#include "GameObject3D.h"
class BossBullet
{
public:
	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection, Vector3 player, Vector3 enemy);
	void Update();
	void Draw();

	WorldTransform GetWorldTransform();
	//生成する場所を設定
	Vector3 SetTransform(Vector3 transform);
	Vector3 SetScale(Vector3 scale);

	bool IsDead()const { return isDelete_; }
	//弾の種類を設定
	int SetBullet(int bulletNum);

	void Aim(Vector3 player, Vector3 enemy);
private:
	GameObject3D* gameObject = nullptr;

	//消えるまでの時間
	// 60 * 消えるまでの時間:
	static const int32_t deleteTime = 60 * 150;
	Vector3 speed;
	//タイマー
	int32_t deleteTimer_ = deleteTime;
	//フラグ
	bool isDelete_ = false;
	int bulletNum;

	Vector3 posA;
	Vector3 posB;
	Vector3 posC;

};

