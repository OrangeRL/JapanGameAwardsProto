#pragma once
#include "GameObject3D.h"

class Enemy {
public:

	//コンストラクタ
	Enemy();
	//デストラクタ
	~Enemy();

	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection, const wchar_t* textureFileName);

	void Update();

	void Draw();

	WorldTransform GetWorldTransform();
	WorldTransform Settransform(float x,float y,float z);
	Vector3 SetSpeed(float x, float y, float z);
private:
	GameObject3D* gameObject = nullptr; // 座標や大きさ等が入っている

	Vector3 moveSpeed = {0,0,0}; //移動速度

};


