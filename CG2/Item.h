#pragma once
#include "GameObject3D.h"

class Item
{
public:
	~Item();

	void Initialize(ViewProjection* viewProjection, XMMATRIX* matProjection, const wchar_t* textureFileName, Vector3 pos);

	void Update();

	void Draw();

	void OnCollision();

	WorldTransform GetWorldTransform() { return gameObject->worldTransform; }
	bool GetIsAlve() { return isAlive; }

private:
	GameObject3D* gameObject = nullptr;

	bool isAlive = true;
	Vector3 rotation = { 0.0f,0.0f,0.0f };
};

