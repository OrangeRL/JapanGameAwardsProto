﻿//#pragma once
//
//#include "fbxsdk.h"
//#include<string>
//#include <d3d12.h>
//#include <d3dx12.h>
//#include <Windows.h>
//#include <wrl.h>
//#include <DirectXMath.h>
//
//#include"FbxModel.h"
//
//class FbxLoader
//{
//private:
//	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
//
//public:
//	/// <summary>
//	/// シングルトンインスタンスの取得
//	/// </summary>
//	/// <returns>インスタンス</returns>
//	static FbxLoader* GetInstance();
//
//	///<summary>
//	///初期化
//	///</summary>
//	///<paramname="device">D3D12デバイス</param>
//	void Initialize(ComPtr<ID3D12Device> device);
//	///<summary> 
//	///後始末 
//	///</summary>
//	void Finalize();
//
//	///<summary>///ファイルからFBXモデル読込///</summary>///<paramname="modelName">モデル名</param>
//	//void LoadModelFromFile(const std::string& modelName);
//	FbxModel* LoadModelFromFile(const std::string& modelName);
//
//	///<summary>///再帰的にノード構成を解析///</summary>///<paramname="model">読み込み先モデルオブジェクト</param>///<paramname="fbxNode">解析対象のノード</param>
//	void ParseNodeRecursive(FbxModel* model, FbxNode* fbxNode, Node* parent = nullptr);
//
//	///<summary>///メッシュ読み取り///</summary>///<paramname="model">読み込み先モデルオブジェクト</param>///<paramname="fbxNode">解析対象のノード</param>
//	void ParseMesh(FbxModel* model, FbxNode* fbxNode);
//
//	//頂点座標読み取り
//	void ParseMeshVertices(FbxModel* model, FbxMesh* fbxMesh);
//	//面情報読み取り
//	void ParseMeshFaces(FbxModel* model, FbxMesh* fbxMesh);
//	//マテリアル読み取り
//	void ParseMaterial(FbxModel* model, FbxNode* fbxNode);
//	//テクスチャ読み取り
//	void LoadTexture(FbxModel* model, const std::string& fullpath);
//	//ディレクトリを含んだファイルパスからファイル名を抽出する
//	std::string ExtractFileName(const std::string& path);
//
//private:
//	// privateなコンストラクタ（シングルトンパターン）
//	FbxLoader() = default;
//	// privateなデストラクタ（シングルトンパターン）
//	~FbxLoader() = default;
//	// コピーコンストラクタを禁止（シングルトンパターン）
//	FbxLoader(const FbxLoader& obj) = delete;
//	// コピー代入演算子を禁止（シングルトンパターン）
//	void operator=(const FbxLoader& obj) = delete;
//
//	//D3D12デバイス
//	static ComPtr<ID3D12Device> device;
//	//FBXマネージャ
//	FbxManager* fbxManager = nullptr;
//	//FBXインポータ
//	FbxImporter* fbxImporter = nullptr;
//
//	using string = std::string;
//
//public:
//	//モデル格納ルートパス
//	static const string baseDirectory;
//	//テクスチャがない場合の標準テクスチャファイル名
//	static const string defaultTextureFileName;
//};