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

	//回転
	Vector3 rotation = { 0,0.5f,0 };
	//回転の速度
	Vector3 rotSpeeed = { 0.005f,0.005f,0.005f };
};

