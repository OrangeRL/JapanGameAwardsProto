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
//	//�萔�o�b�t�@�̐���
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
//	//���[���h�ϊ��̏�����
//	worldTransform.initialize();
//}
//
//void FbxObject3D::Update()
//{
//	HRESULT result;
//
//	worldTransform.UpdateMatWorld();
//	//�萔�o�b�t�@�փf�[�^�]��
//	constMapTransform->mat = worldTransform.matWorld;
//	constMapTransform->mat *= camera->matView;
//	constMapTransform->mat *= MathFunc::Utility::ConvertXMMATRIXtoMatrix4(*matProjection);
//	
//	//���f���̃��b�V���g�����X�t�H�[��
//	const XMMATRIX& modelTransform = model->GetModelTransform();
//}
//
//void FbxObject3D::Draw(ID3D12GraphicsCommandList* cmdList)
//{
//	//���f���̊��蓖�Ă��Ȃ���Ε`��s��
//	if (model == nullptr)
//	{
//		return;
//	}
//
//	//�p�C�v���C���X�e�[�g�̐ݒ�
//	cmdList->SetPipelineState(pipelinestate.Get());
//	//���[�g�V�O�l�`���̐ݒ�
//	cmdList->SetGraphicsRootSignature(rootsignature.Get());
//	//�v���~�e�B�u�`��̐ݒ�
//	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//	//�萔�o�b�t�@�r���[���Z�b�g
//	cmdList->SetGraphicsRootConstantBufferView(0, 
//		constBuffTransform->GetGPUVirtualAddress());
//	
//
//	//���f���`��
//	model->Draw(cmdList);
//}
//
//
//void FbxObject3D::CreateGraphicsPipeline()
//{
//	HRESULT result = S_FALSE;
//	ComPtr<ID3DBlob> vsBlob; // ���_�V�F�[�_�I�u�W�F�N�g
//	ComPtr<ID3DBlob> psBlob;    // �s�N�Z���V�F�[�_�I�u�W�F�N�g
//	ComPtr<ID3DBlob> errorBlob; // �G���[�I�u�W�F�N�g
//
//	assert(device);
//
//	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
//	result = D3DCompileFromFile(
//		L"FbxVertexShader.hlsl",    // �V�F�[�_�t�@�C����
//		nullptr,
//		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
//		"main", "vs_5_0",    // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
//		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
//		0,
//		&vsBlob, &errorBlob);
//
//	// �f�X�N���v�^�����W
//	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
//	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 ���W�X�^
//
//	// ���[�g�p�����[�^
//	CD3DX12_ROOT_PARAMETER rootparams[2];
//	//CD3DX12_ROOT_PARAMETER rootparams[3];
//	// CBV�i���W�ϊ��s��p�j
//	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
//	// SRV�i�e�N�X�`���j
//	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
//}