#pragma once
#include "Matrix4.h"
#include "Vector3.h"
#include "MathFunc.h"
#include "WinApp.h"
class ViewProjection
{
public:
	Matrix4 matView;
	Vector3 eye;	//視点座標
	Vector3 target={0,0,0};	//注視点座標
	Vector3 up;		//上方向ベクトル
	XMMATRIX matProjection = {};
	WinApp* winApp = nullptr;
public:
	void Initialize(WinApp* winApp);
	void UpdateView();
	void FOV(WinApp* winApp);
};

