#pragma once
#include "GameObject3D.h"
#include "EnemyBullet.h"
#include <memory>
#include <list>

class Enemy {
public:

	//コンストラクタ
	Enemy();
	//デストラクタ
	~Enemy();

	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection, const wchar_t* textureFileName);


	void Update(ViewProjection* viewProjection, XMMATRIX* matProjection, const wchar_t* textureFileName, int bulletNum);


	void Draw();
	//座標取得用

	void Reset();

	WorldTransform GetWorldTransform();
	//生成される場所を設定
	WorldTransform Settransform(float x,float y,float z);
	//速度設定
	float SetSpeed(float speed);
private:
	GameObject3D* gameObject = nullptr; // 座標や大きさ等が入っている
	std::list<std::unique_ptr<EnemyBullet>> bullets;


	float moveSpeed = 0; //移動速度
	float attackSpeed = 100.0f;
	bool isAttack = false;

};


