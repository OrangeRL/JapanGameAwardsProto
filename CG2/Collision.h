#pragma once
#include "DirectXMath.h"
#include "Vector3.h"

class Collision
{
private:
	//当たり判定の計算に必要な構造体
	struct CubeVertex
	{
		Vector3 c;	//中心
		//DirectX::XMFLOAT3 v[8];	//各頂点の座標
		Vector3 min;	//-x,-y,-z
		Vector3 max;	//x,y,z
	};
	//当たり判定の計算に必要な構造体
	struct SphereVertex
	{
		Vector3 c;	//中心
		Vector3 r;	//半径
		Vector3 min;
		Vector3 max;
	};
public:
	//衝突判定を行うオブジェクトをセット(直方体と球)
	void SetObject(Vector3 cubePos, Vector3 cubeScale);
	//更新
	bool Update(Vector3 spherePos, Vector3 sphereScale);

	//ゲッター
	Vector3 GetPosition() { return cubeVertex1.c; }
	Vector3 GetMin() { return cubeVertex1.min; }
	Vector3 GetMax() { return cubeVertex1.max; }
private:
	//当たり判定に必要な変数
	CubeVertex cubeVertex1;
	SphereVertex sphereVertex1;

	/*DirectX::XMFLOAT3 cubePos;
	DirectX::XMFLOAT3 cubeScale;*/

	bool hit;
};