#pragma once
#include"FbxModel.h"
#include"ViewProjection.h"

#include<Windows.h>
#include<wrl.h>
#include<d3d12.h>
#include<d3dx12.h>
#include<DirectXMath.h>
#include<string>
class FbxObject3D
{
protected:	//エイリアス
//Microsoft::WRL::を省略
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:	//静的メンバ関数
//セッター
	static void SetDevice(ComPtr<ID3D12Device> device) { FbxObject3D::device = device; }
	static void SetCamera(ViewProjection* camera) { FbxObject3D::camera = camera; }
private://静的メンバ変数
	static ComPtr<ID3D12Device> device;
	static ViewProjection* camera;
};