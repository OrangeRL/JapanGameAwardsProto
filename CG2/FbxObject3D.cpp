//#include "FbxObject3D.h"
//
//#include <d3dcompiler.h>
//#pragma comment(lib,"d3dcompiler.lib")
//
//using namespace Microsoft::WRL;
//using namespace DirectX;
//
//ComPtr<ID3D12RootSignature>FbxObject3D::rootsignature;
//ComPtr<ID3D12PipelineState>FbxObject3D::pipelinestate;
//
//
//void FbxObject3D::Initialize()
//{
//	HRESULT result;
//	//定数バッファの生成
//	CD3DX12_HEAP_PROPERTIES v1 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
//	CD3DX12_RESOURCE_DESC v2 = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataTransform) + 0xff) & ~0xff);
//	result = device->CreateCommittedResource(
//		&v1,
//		D3D12_HEAP_FLAG_NONE,
//		&v2,
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&constBuffTransform)
//	);
//	//ワールド変換の初期化
//	worldTransform.initialize();
//}
//
//void FbxObject3D::Update()
//{
//	HRESULT result;
//
//	worldTransform.UpdateMatWorld();
//	//定数バッファへデータ転送
//	constMapTransform->mat = worldTransform.matWorld;
//	constMapTransform->mat *= camera->matView;
//	constMapTransform->mat *= MathFunc::Utility::ConvertXMMATRIXtoMatrix4(*matProjection);
//	
//	//モデルのメッシュトランスフォーム
//	const XMMATRIX& modelTransform = model->GetModelTransform();
//}
//
//void FbxObject3D::Draw(ID3D12GraphicsCommandList* cmdList)
//{
//	//モデルの割り当てがなければ描画市内
//	if (model == nullptr)
//	{
//		return;
//	}
//
//	//パイプラインステートの設定
//	cmdList->SetPipelineState(pipelinestate.Get());
//	//ルートシグネチャの設定
//	cmdList->SetGraphicsRootSignature(rootsignature.Get());
//	//プリミティブ形状の設定
//	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//	//定数バッファビューをセット
//	cmdList->SetGraphicsRootConstantBufferView(0, 
//		constBuffTransform->GetGPUVirtualAddress());
//	
//
//	//モデル描画
//	model->Draw(cmdList);
//}
//
//
//void FbxObject3D::CreateGraphicsPipeline()
//{
//	HRESULT result = S_FALSE;
//	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
//	ComPtr<ID3DBlob> psBlob;    // ピクセルシェーダオブジェクト
//	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト
//
//	assert(device);
//
//	// 頂点シェーダの読み込みとコンパイル
//	result = D3DCompileFromFile(
//		L"FbxVertexShader.hlsl",    // シェーダファイル名
//		nullptr,
//		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
//		"main", "vs_5_0",    // エントリーポイント名、シェーダーモデル指定
//		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
//		0,
//		&vsBlob, &errorBlob);
//
//	// デスクリプタレンジ
//	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
//	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ
//
//	// ルートパラメータ
//	CD3DX12_ROOT_PARAMETER rootparams[2];
//	//CD3DX12_ROOT_PARAMETER rootparams[3];
//	// CBV（座標変換行列用）
//	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
//	// SRV（テクスチャ）
//	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
//}