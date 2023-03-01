#pragma once
#include "GameObject3D.h"
#include "Input.h"
class EnemyBullet
{
public:
	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection, const wchar_t* textureFileName);
	void Update(Vector3 player, Vector3 enemy);
	void Draw();

	WorldTransform GetWorldTransform();
	//生成する場所を設定
	Vector3 SetTransform(Vector3 transform);
	//弾の種類を設定
	int SetBullet(int bulletNum);

	bool IsDead()const { return isDelete_; }
private:
	GameObject3D* gameObject = nullptr;
	//キー入力
	Input& input = Input::GetInstance();
	//弾の種類	0=プレイヤー狙い, 1=正面カーテン等
	int bulletNum;

	//消えるまでの時間
	// 60 * 消えるまでの時間:
	static const int32_t deleteTime = 60 * 25;
	//タイマー
	int32_t deleteTimer_ = deleteTime;
	//フラグ
	bool isDelete_ = false;
};

