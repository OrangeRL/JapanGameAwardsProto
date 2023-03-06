#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "MathFunc.h"
#include "Input.h"

class ReilCamera
{
public:
	void Initialize(Vector3 pos, Vector3 rot);

	void Update(Input* input);

	ViewProjection GetViewProjection() { return viewProjection_; }

	WorldTransform GetWorldTransform() { return worldTransform_; }

private:
	Input* input = nullptr;
	//ワールド行列
	WorldTransform worldTransform_;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	Vector3 up = { 0,1,0 };
	float radian = 0.5f;
};

