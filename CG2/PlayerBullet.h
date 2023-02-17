#pragma once
#include "GameObject3D.h"
#include "MathFunc.h"

class PlayerBullet {
public:
	//コンストラクタ
	PlayerBullet();
	//デストラクタ
	~PlayerBullet();

	//初期化
	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection);
	//更新
	void Update(Vector3 playerPos, Vector3 bossPos);
	//描画
	void Draw();
private:	//メンバ関数
	//攻撃
	void Attack(Vector3 playerPos, Vector3 bossPos);
private:	//メンバ変数
	//弾数
	const int bulletNum = 300;
	//ゲームオブジェクト
	GameObject3D* gameObject[300];
	//表示フラグ
	bool isShot[300];
	//カウント
	int bulletCount = 0;
	//弾の移動速度
	const float speed = -1.0f;
	//弾のベクトル
	Vector3 velocity[300];
	//弾の移動範囲
	float canMoveArea = 120;
};