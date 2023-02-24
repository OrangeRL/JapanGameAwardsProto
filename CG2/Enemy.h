#pragma once
#include "GameObject3D.h"
#include "EnemyBullet.h"

class Enemy {
public:

	//コンストラクタ
	Enemy();
	//デストラクタ
	~Enemy();

	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection, const wchar_t* textureFileName);

	void Update(ViewProjection* viewProjection, XMMATRIX* matProjection, const wchar_t* textureFileName);

	void Draw();

	WorldTransform GetWorldTransform();
	WorldTransform Settransform(float x,float y,float z);
	float SetSpeed(float speed);
private:
	GameObject3D* gameObject = nullptr; // 座標や大きさ等が入っている

	float moveSpeed = 0; //移動速度
	float attackSpeed = 100.0f;
	bool isAttack = false;
};


