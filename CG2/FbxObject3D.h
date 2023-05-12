#pragma once
#include"FbxModel.h"
#include"ViewProjection.h"
#include "WorldTransform.h"
#include<Windows.h>
#include<wrl.h>
#include<d3d12.h>
#include<d3dx12.h>
#include<DirectXMath.h>
#include<string>
#include "DX12base.h"

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
public:
	struct ConstBufferDataTransform
	{
		Matrix4 viewproj;
		Matrix4 world;
		//XMFLOAT3 cameraPos;
		Matrix4 mat; //3D変換行列
	};
	struct ConstBufferDataMaterial {
		Vector4 color; //色(RGBA)
	};
//セッター
	static void SetDevice(ComPtr<ID3D12Device> device) { FbxObject3D::device = device; }
	static void SetCamera(ViewProjection* camera) { FbxObject3D::camera = camera; }
private://静的メンバ変数
	static ComPtr<ID3D12Device> device;
	static ViewProjection* camera;
	static ID3D12GraphicsCommandList* cmdList;
public://メンバ関数
	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw(ID3D12GraphicsCommandList* CmdList);

	void SetModel(FbxModel* model) { this->model = model; }
	//グラフィックスパイプラインの生成
	static void CreateGraphicsPipeline();

	void InitializeConstMapTransform();
	void InitializeConstMapMaterial();

	void SetViewProjection(ViewProjection* viewProjection);
	void SetMatProjection(XMMATRIX* matProjection);
private://メンバ変数
	//定数バッファ
	//ComPtr<ID3D12Resource>constBuffTransform;
	//定数バッファ(行列用)
	ComPtr<ID3D12Resource> constBuffTransform = nullptr;
	//定数バッファマッピング(行列用)
	ConstBufferDataTransform* constMapTransform = nullptr;

	ComPtr<ID3D12Resource> constBuffMaterial = nullptr;
	//ルートシグネチャ
	static ComPtr<ID3D12RootSignature>rootsignature;
	//パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState>pipelinestate;
	ConstBufferDataMaterial* constMapMaterial = nullptr;
private:
	//ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	//X,Y,Z軸回りのローカル行列
	XMFLOAT3 rotation = { 0,0,0 };
	//ローカル座標
	XMFLOAT3 position = { 0,0,0 };
	//ローカルワールド変換行列
	XMMATRIX matWorld;
	//モデル
	FbxModel* model = nullptr;
	//ワールド変換
	WorldTransform worldTransform;
	XMMATRIX* matProjection;
	//ビュープロジェクション
	ViewProjection* viewProjection;
	DX12base& dx12base = DX12base::GetInstance();
	Vector4 color = { 1.0f,1.0f,1.0f,1.0f };
};