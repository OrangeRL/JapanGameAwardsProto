#pragma once
#include "GameObject3D.h"
#include "Input.h"
class EnemyBullet
{
public:
	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection, const wchar_t* textureFileName);
	void Update(WorldTransform enemy);
	void Draw();
	Vector3 SetTransform(Vector3 transform);
private:
	GameObject3D* gameObject = nullptr;
	//ÉLÅ[ì¸óÕ
	Input& input = Input::GetInstance();
};

