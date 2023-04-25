//#pragma once
//#include"FbxModel.h"
//#include"ViewProjection.h"
//#include "WorldTransform.h"
//#include<Windows.h>
//#include<wrl.h>
//#include<d3d12.h>
//#include<d3dx12.h>
//#include<DirectXMath.h>
//#include<string>
//
//class FbxObject3D
//{
//protected:	//�G�C���A�X
////Microsoft::WRL::���ȗ�
//	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
//	//DirectX::���ȗ�
//	using XMFLOAT2 = DirectX::XMFLOAT2;
//	using XMFLOAT3 = DirectX::XMFLOAT3;
//	using XMFLOAT4 = DirectX::XMFLOAT4;
//	using XMMATRIX = DirectX::XMMATRIX;
//public:
//	struct ConstBufferDataTransform
//	{
//		XMMATRIX viewproj;
//		XMMATRIX world;
//		XMFLOAT3 cameraPos;
//		Matrix4 mat; //3D�ϊ��s��
//	};
////�Z�b�^�[
//	static void SetDevice(ComPtr<ID3D12Device> device) { FbxObject3D::device = device; }
//	static void SetCamera(ViewProjection* camera) { FbxObject3D::camera = camera; }
//private://�ÓI�����o�ϐ�
//	static ComPtr<ID3D12Device> device;
//	static ViewProjection* camera;
//public://�����o�֐�
//	//������
//	void Initialize();
//	//�X�V
//	void Update();
//	//�`��
//	void Draw(ID3D12GraphicsCommandList* cmdList);
//
//	void SetModel(FbxModel* model) { this->model = model; }
//	//�O���t�B�b�N�X�p�C�v���C���̐���
//	static void CreateGraphicsPipeline();
//private://�����o�ϐ�
//	//�萔�o�b�t�@
//	//ComPtr<ID3D12Resource>constBuffTransform;
//	//�萔�o�b�t�@(�s��p)
//	ComPtr<ID3D12Resource> constBuffTransform = nullptr;
//	//�萔�o�b�t�@�}�b�s���O(�s��p)
//	ConstBufferDataTransform* constMapTransform = nullptr;
//
//	//���[�g�V�O�l�`��
//	static ComPtr<ID3D12RootSignature>rootsignature;
//	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
//	static ComPtr<ID3D12PipelineState>pipelinestate;
//private:
//	//���[�J���X�P�[��
//	XMFLOAT3 scale = { 1,1,1 };
//	//X,Y,Z�����̃��[�J���s��
//	XMFLOAT3 rotation = { 0,0,0 };
//	//���[�J�����W
//	XMFLOAT3 position = { 0,0,0 };
//	//���[�J�����[���h�ϊ��s��
//	XMMATRIX matWorld;
//	//���f��
//	FbxModel* model = nullptr;
//	//���[���h�ϊ�
//	WorldTransform worldTransform;
//	XMMATRIX* matProjection;
//};