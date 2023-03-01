#pragma once
#include "GameObject3D.h"
#include "EnemyBullet.h"
#include <memory>
#include <list>

class Enemy {
public:

	//コンストラクタ
	Enemy();
	//チE��トラクタ
	~Enemy();

	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection, const wchar_t* textureFileName);


	void Update(ViewProjection* viewProjection, XMMATRIX* matProjection, const wchar_t* textureFileName, int bulletNum);


	void Draw();
	
	void Reset();

	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets; }
	//���W�擾�p
	WorldTransform GetWorldTransform();
	//���������ꏊ��ݒ�
	WorldTransform Settransform(float x,float y,float z);
	//���x�ݒ�
	float SetSpeed(float speed);
private:
	GameObject3D* gameObject = nullptr; // 座標や大きさ等が入ってぁE��
	std::list<std::unique_ptr<EnemyBullet>> bullets;
	EnemyBullet* enemyBullet = nullptr;

	float moveSpeed = 0; //移動速度
	float attackSpeed = 100.0f;
	bool isAttack = false;

};


