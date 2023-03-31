#pragma once
#include "GameObject3D.h"

class Item
{
public:
	~Item();

	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection, const wchar_t* textureFileName, Vector3 pos, int weapon);

	void Update();

	void Draw();

	void OnCollision();

	Vector3 GetPos();
	WorldTransform GetWorldTransform() { return gameObject->worldTransform; }
	bool GetIsDead() { return isDead; }
	int GetWeapon() { return weapon; }

private:
	GameObject3D* gameObject = nullptr;

	bool isDead = false;
	Vector3 rotation = { 0.0f,0.01f,0.0f };
	int weapon = 0;
};

