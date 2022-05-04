#pragma region �W��.h include

#include <d3dcompiler.h>
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <string>
#include <DirectXMath.h>
#include <wrl.h>
#include <cassert>
#include <memory>
#include <DirectXTex.h>

#pragma endregion �W��.h include

#pragma region ����.h include

#include "Input.h"
#include "DxWindow.h"
#include "Dx12.h"
#include "View.h"
#include "Projection.h"
#include "WorldMatrix.h"
#include "Depth.h"
#include "Object3d.h"
#include "ObjectMaterial.h"
#include "TextureFile.h"
#include "MipMap.h"
#include "TexImgData.h"
#include "TextureBuffer.h"
#include "Descriptor.h"
#include "RootParameter.h"
#include "Vertex.h"
#include "MCBMatrix.h"
#include "Util.h"

#pragma endregion ����.h include

#pragma region pragma comment

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#pragma endregion pragma comment

#pragma region using namespace

using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;
using namespace MCB;

#pragma endregion using namespace


// Windows�A�v���ł̃G���g���[�|�C���g(main�֐�) 
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) 
{  

    DxWindow dxWindow;

#pragma region DirectX������
    //�f�o�b�N���̂�----------
#pragma region �f�o�b�N���̂�

#ifdef _DEBUG
//�f�o�b�N���C���[���I����
    ComPtr<ID3D12Debug> debugController;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
    {
        debugController->EnableDebugLayer();
    }

#endif

#pragma endregion �f�o�b�N���̂�
    //-------------

    //DirectX�N���X����
    Dx12 dx(dxWindow);
    //input�N���X����
    Input input(dx.result,dxWindow.window,dxWindow.hwnd);

#pragma endregion 


#pragma region �`�揉��������

    //�[�x�o�b�t�@----
    Depth depth(dxWindow, dx);
    //-------

    //3D�I�u�W�F�N�g�}�e���A���̐���-------------------
    ObjectMaterial objMaterial;
    objMaterial.Init(dx);
    //---------------------

    //3D�I�u�W�F�N�g�̐���-------------------
#pragma region 3D�I�u�W�F�N�g�̐���

     const size_t objectNum = 50;
     Object3d object3D[objectNum];
     for (int i = 0; i < objectNum; i++)
     {
         object3D[i].Init(dx);
         if (i > 0)
         {
             object3D[i].parent = &object3D[i - 1];

             object3D[i].scale = { 0.9f,0.9f,0.9f };

             object3D[i].rotasion = { 0.0f,0.0f,XMConvertToRadians(30.0f) };

             object3D[i].position = { 0.0f,0.0f,-0.8f };
         }
     }

     Object3d Rales[objectNum * 2];
     for (int i = 0; i < objectNum * 2; i++)
     {
         Rales[i].Init(dx);
         Rales[i].position.y = -25.0f;
         if (i > 0)
         {
             Rales[i].position.z = Rales[i - 1].position.z + 20;
         }
     }

#pragma endregion 3D�I�u�W�F�N�g�̐���
    //----------------------

     //�s��-----------------------
#pragma region �s��
        //�r���[�ϊ��s��
        View matView;
        matView.CreateMatrixView(XMFLOAT3(0.0f, 0.0f, -100.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));
        //�ˉe�ϊ��s��
        Projection matProjection;
         matProjection.CreateMatrixProjection(XMConvertToRadians(45.0f),(float)dxWindow.window_width / dxWindow.window_height, 0.1f, 1000.0f);
#pragma endregion �s��
     //---------------------

#pragma region �摜�֌W
     //�摜�t�@�C��--------------------
     TextureFile textureFile;
     dx.result = textureFile.LoadTexture(L"Resources\\tori.png", WIC_FLAGS_NONE);
     //----------------------------

     //�~�b�v�}�b�v�̐���-------------------------
     MipMap mipmap;
     dx.result = mipmap.GenerateMipMap(&textureFile, TEX_FILTER_DEFAULT, 0);
     //----------------------------

     //�摜�C���[�W�f�[�^�̍쐬----------------------
     TexImgData imageData;
     imageData.SetImageDataRGBA(XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
     //------------------------------------

      //�e�N�X�`���o�b�t�@�ݒ�---------------------------------------
      TextureBuffer texBuff;
      texBuff.SetTexHeapProp(D3D12_HEAP_TYPE_CUSTOM,D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,D3D12_MEMORY_POOL_L0);
      texBuff.SetTexResourceDesc(textureFile, D3D12_RESOURCE_DIMENSION_TEXTURE2D, 1);
      //--------------------------------------


      //�e�N�X�`���o�b�t�@�̐���----------------------
      dx.result = texBuff.CommitResouce(dx, D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr);
      texBuff.TransferMipmatToTexBuff(textureFile, nullptr, dx.result);
      //-----------------------------------
#pragma endregion �摜�֌W

         //�f�X�N���v�^�q�[�v�̐���-------------------------
#pragma region �f�X�N���v�^�q�[�v�̐���

     const size_t kMaxSRVCount = 2056;
     Descriptor descriptor;
     descriptor.SetHeapDesc(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, kMaxSRVCount);
     dx.result = descriptor.SetDescriptorHeap(dx);
     descriptor.SetSrvHeap();

#pragma endregion �f�X�N���v�^�q�[�v�̐���
    //-------------------------------

     //�V�F�[�_�[���\�[�X�r���[�̍쐬------------------------------
#pragma region �V�F�[�_�[���\�[�X�r���[�̍쐬

     descriptor.SetSrvDesc(texBuff, D3D12_SRV_DIMENSION_TEXTURE2D);

     descriptor.SetShaderResourceView(dx, texBuff);

#pragma endregion �V�F�[�_�[���\�[�X�r���[�̍쐬
     //----------------------------

     //�f�X�N���v�^�����W�̐ݒ�--------------------------------
#pragma region �f�X�N���v�^�����W�̐ݒ�

     descriptor.SetDescriptorRange(1, D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0);

#pragma endregion �f�X�N���v�^�����W�̐ݒ�
     //-----------------------------------------

     //���[�g�p�����[�^�̐ݒ�---------------------------
#pragma region ���[�g�p�����[�^�̐ݒ�

     RootParameter rootparams;
     rootparams.SetRootParam(D3D12_ROOT_PARAMETER_TYPE_CBV, 0, 0, D3D12_SHADER_VISIBILITY_ALL,descriptor,0);
     rootparams.SetRootParam(D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE, 0, 0, D3D12_SHADER_VISIBILITY_ALL,descriptor,1);
     rootparams.SetRootParam(D3D12_ROOT_PARAMETER_TYPE_CBV, 1, 0, D3D12_SHADER_VISIBILITY_ALL, descriptor, 0);
#pragma endregion ���[�g�p�����[�^�̐ݒ�
     //------------------------

     //���_�f�[�^---------------------------------
#pragma region ���_�f�[�^
   
     Vertex vertex;

#pragma endregion ���_�f�[�^
     //--------------------------
    
     //�C���f�b�N�X�o�b�t�@�̐ݒ�-------------------------
#pragma region �C���f�b�N�X�̐ݒ�
    
     objMaterial.SetIndex(D3D12_RESOURCE_DIMENSION_BUFFER, vertex.sizeIB, 1, 1, 1, 1, D3D12_TEXTURE_LAYOUT_ROW_MAJOR);

#pragma endregion �C���f�b�N�X�̐ݒ�
     //------------------------

#pragma region �C���f�b�N�X�o�b�t�@����

     vertex.CreateIndexBuffer(dx, objMaterial.HeapProp, D3D12_HEAP_FLAG_NONE,objMaterial.Resdesc, D3D12_RESOURCE_STATE_GENERIC_READ);

#pragma endregion �C���f�b�N�X�o�b�t�@����

     //�C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��------------------------------
#pragma region �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��

     dx.result = vertex.IndexMaping();

#pragma endregion �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
    //-------------------------------------

     //�C���f�b�N�X�o�b�t�@�r���[�̍쐬-----------------------------------
#pragma region �C���f�b�N�X�o�b�t�@�r���[�̍쐬
     vertex.SetIbView(DXGI_FORMAT_R16_UINT);
#pragma endregion �C���f�b�N�X�o�b�t�@�r���[�̍쐬
     //------------------------------------------

     //���_�o�b�t�@---------------
#pragma region ���_�o�b�t�@�̐ݒ�
     objMaterial.SetVertexBuffer(D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_DIMENSION_BUFFER, vertex.sizeVB, 1, 1, 1, 1, D3D12_TEXTURE_LAYOUT_ROW_MAJOR);
#pragma endregion ���_�o�b�t�@�̐ݒ�
     //----------------------------------

     // ���_�o�b�t�@�̐���----------------------------
#pragma region ���_�o�b�t�@�̐���

     vertex.CreateVertexBuffer(dx, objMaterial.HeapProp, D3D12_HEAP_FLAG_NONE, objMaterial.Resdesc, D3D12_RESOURCE_STATE_GENERIC_READ);

#pragma endregion ���_�o�b�t�@�̐���
     //-------------------------

     //�@���x�N�g���v�Z---------------------------
#pragma region �@���x�N�g���v�Z
     for (int i = 0; i < _countof(vertex.boxIndices) / 3; i++)
     {
         //�O�p�`1���ƂɌv�Z

         //�O�p�`�̃C���f�b�N�X�����o���āA�ꎞ�I�ȕϐ��ɓ����
         unsigned short index0 = vertex.boxIndices[i * 3 + 0];
         unsigned short index1 = vertex.boxIndices[i * 3 + 1];
         unsigned short index2 = vertex.boxIndices[i * 3 + 2];

         //�O�p�`���\�����钸�_���W
         XMVECTOR p0 = XMLoadFloat3(&vertex.Box[index0].pos);
         XMVECTOR p1 = XMLoadFloat3(&vertex.Box[index1].pos);
         XMVECTOR p2 = XMLoadFloat3(&vertex.Box[index2].pos);

         //p0->p1�x�N�g���Ap0->p2�x�N�g�����v�Z�i�x�N�g���̌��Z�j
         XMVECTOR v1 = XMVectorSubtract(p1, p0);
         XMVECTOR v2 = XMVectorSubtract(p2, p0);

         //�O�ς͗������琂���ȃx�N�g��
         XMVECTOR normal = XMVector3Cross(v1, v2);

         //���K���i��������ɂ���)
         normal = XMVector3Normalize(normal);

         //���߂��@���𒸓_�f�[�^�ɑ��
         XMStoreFloat3(&vertex.Box[index0].normal, normal);
         XMStoreFloat3(&vertex.Box[index1].normal, normal);
         XMStoreFloat3(&vertex.Box[index2].normal, normal);

     }
#pragma endregion �@���x�N�g�����v�Z
     //-------------------------

     // ���_�o�b�t�@�ւ̃f�[�^�]��------------
#pragma region ���_�o�b�t�@�ւ̃f�[�^�]��
     StructVertex* vertMap = nullptr;
     dx.result = vertex.vertBuff->Map(0, nullptr, (void**)&vertMap);
     assert(SUCCEEDED(dx.result));

     // �S���_�ɑ΂���
     for (int i = 0; i < _countof(vertex.Box); i++)
     {
         vertMap[i] = vertex.Box[i];   // ���W���R�s�[
     }

     // �}�b�v������
     vertex.vertBuff->Unmap(0, nullptr);
#pragma endregion ���_�o�b�t�@�ւ̃f�[�^�]��
     //--------------------------------------

     // ���_�o�b�t�@�r���[�̍쐬--------------------------
#pragma region ���_�o�b�t�@�r���[�̍쐬
     D3D12_VERTEX_BUFFER_VIEW vbView{};

     vbView.BufferLocation = vertex.vertBuff->GetGPUVirtualAddress();
     vbView.SizeInBytes = vertex.sizeVB;
     vbView.StrideInBytes = sizeof(vertex.Box[0]);
#pragma endregion ���_�o�b�t�@�r���[�̍쐬
     //-----------------------------------

       //�V�F�[�_�[�I�u�W�F�N�g�錾-------------------------------------------
#pragma region �V�F�[�_�[�I�u�W�F�N�g�錾
     ComPtr<ID3DBlob> vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
     ComPtr<ID3DBlob> psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
     ComPtr<ID3DBlob> errorBlob = nullptr; // �G���[�I�u�W�F�N�g
#pragma endregion �V�F�[�_�[�I�u�W�F�N�g�錾
//---------------------------------

// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��--------------------------------
#pragma region ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��

     dx.result = D3DCompileFromFile(
         L"BasicVS.hlsl",  // �V�F�[�_�t�@�C����
         nullptr,
         D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
         "main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
         D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
         0,
         &vsBlob, &errorBlob);

#pragma endregion ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
     //------------------------------------------


     //  �V�F�[�_�[�̃G���[�Ɋւ���o�͕���-----------------
#pragma region �V�F�[�_�[�̃G���[�Ɋւ���o�͕���

     if (FAILED(dx.result)) {
         // errorBlob����G���[���e��string�^�ɃR�s�[
         string error;
         error.resize(errorBlob->GetBufferSize());

         copy_n((char*)errorBlob->GetBufferPointer(),
             errorBlob->GetBufferSize(),
             error.begin());
         error += "\n";
         // �G���[���e���o�̓E�B���h�E�ɕ\��
         OutputDebugStringA(error.c_str());
         assert(0);
     }

#pragma endregion �V�F�[�_�[�̃G���[�Ɋւ���o�͕���
     //-----------------------------------

     // �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��-------------------------------
#pragma region �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��

     dx.result = D3DCompileFromFile(
         L"BasicPS.hlsl",   // �V�F�[�_�t�@�C����
         nullptr,
         D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
         "main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
         D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
         0,
         &psBlob, &errorBlob);

#pragma endregion �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
     //--------------------------------

     //  �V�F�[�_�[�̃G���[�Ɋւ���o�͕���-----------------
#pragma region �V�F�[�_�[�̃G���[�Ɋւ���o�͕���

     if (FAILED(dx.result)) {
         // errorBlob����G���[���e��string�^�ɃR�s�[
         string error;
         error.resize(errorBlob->GetBufferSize());

         copy_n((char*)errorBlob->GetBufferPointer(),
             errorBlob->GetBufferSize(),
             error.begin());
         error += "\n";
         // �G���[���e���o�̓E�B���h�E�ɕ\��
         OutputDebugStringA(error.c_str());
         assert(0);
     }

#pragma endregion �V�F�[�_�[�̃G���[�Ɋւ���o�͕���
     //-----------------------------------

      // ���_���C�A�E�g------------------
#pragma region ���_���C�A�E�g

     D3D12_INPUT_ELEMENT_DESC inputLayout[] =
     {
         {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}, // (1�s�ŏ������ق������₷��)
         {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},//�@���x�N�g��
         {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}//uv���W
     };

#pragma endregion ���_���C�A�E�g
     //--------------------

     // �O���t�B�b�N�X�p�C�v���C���ݒ�-----------
     D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipelineDesc{};
     //-------------------------

     //���_�V�F�[�_�A�s�N�Z���V�F�[�_���p�C�v���C���ɐݒ�-----------------------------
#pragma region ���_�V�F�[�_�ƃs�N�Z���V�F�[�_���p�C�v���C���ɐݒ�

     gpipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
     gpipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
     gpipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
     gpipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

#pragma endregion ���_�V�F�[�_�ƃs�N�Z���V�F�[�_���p�C�v���C���ɐݒ�
     //-----------------------------------

     //�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�------------------------------------
#pragma region �T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�

     gpipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
     gpipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;  // �w�ʃJ�����O
     gpipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // �|���S�����h��Ԃ�
     gpipelineDesc.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����

#pragma endregion �T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
//------------------------------------


      //�u�����h�X�e�[�g�̐ݒ�-------------------------------
#pragma region �u�����h�X�e�[�g�̐ݒ�
//gpipeline.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;  // RBGA�S�Ẵ`�����l����`��
     D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = gpipelineDesc.BlendState.RenderTarget[0];
     blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//�W���ݒ�

     //���ʐݒ�
     blenddesc.BlendEnable = true;
     blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
     blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
     blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

     //����������
     blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
     blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
     blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

     //���Z����
     //#pragma region ���Z����
     //blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
     //blenddesc.SrcBlend = D3D12_BLEND_ONE;
     //blenddesc.DestBlend = D3D12_BLEND_ONE;
     //#pragma endregion

     //���Z����
   /*  #pragma region ���Z����
     blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
     blenddesc.SrcBlend = D3D12_BLEND_ONE;
     blenddesc.DestBlend = D3D12_BLEND_ONE;
     #pragma endregion*/

     ////�F���]
     //blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
     //blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
     //blenddesc.DestBlend = D3D12_BLEND_ZERO;


#pragma endregion �u�����h�X�e�[�g�̐ݒ�
     //--------------------------

     //���_���C�A�E�g�̐ݒ�------------------
#pragma region ���_���C�A�E�g�̐ݒ�

     gpipelineDesc.InputLayout.pInputElementDescs = inputLayout;
     gpipelineDesc.InputLayout.NumElements = _countof(inputLayout);

#pragma endregion ���_���C�A�E�g�̐ݒ�
     //----------------------------

     //�}�`�̌`����O�p�`�ɐݒ�-------------------------
     gpipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
     //------------------

     //���̑��̐ݒ�----------------
#pragma region ���̑��̐ݒ�

     gpipelineDesc.NumRenderTargets = 1; // �`��Ώۂ�1��
     gpipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0�`255�w���RGBA
     gpipelineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

#pragma endregion ���̑��̐ݒ�
    //----------------

     depth.SetDepthStencilState(gpipelineDesc);

   //�e�N�X�`���T���v���[�̐ݒ�-----------------------
#pragma region �e�N�X�`���T���v���[�̐ݒ�

     D3D12_STATIC_SAMPLER_DESC samplerDesc{};

     samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
     samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
     samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
     samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
     samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
     samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
     samplerDesc.MinLOD = 0.0f;
     samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
     samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

#pragma endregion �e�N�X�`���T���v���[�̐ݒ�
   //----------------------------------

     //���[�g�V�O�l�`���̐���--------------------------
#pragma region ���[�g�V�O�l�`���̐���

     ComPtr <ID3D12RootSignature> rootsignature;

     D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
     rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
     rootSignatureDesc.pParameters = &rootparams.rootparams.front(); //���[�g�p�����[�^�̐擪�A�h���X
     rootSignatureDesc.NumParameters = rootparams.rootparams.size(); //���[�g�p�����[�^��
     rootSignatureDesc.pStaticSamplers = &samplerDesc;
     rootSignatureDesc.NumStaticSamplers = 1;


     ComPtr<ID3DBlob> rootSigBlob = nullptr;
     dx.result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
     dx.result = dx.device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));

     assert(SUCCEEDED(dx.result));

     // �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
     gpipelineDesc.pRootSignature = rootsignature.Get();

#pragma endregion ���[�g�V�O�l�`���̐���
     //--------------------------------

    //�p�C�v���C���X�e�[�g�̐���-------------------------
#pragma region �p�C�v���C���X�e�[�g�̐���

     ComPtr<ID3D12PipelineState> pipelinestate = nullptr;
     dx.result = dx.device->CreateGraphicsPipelineState(&gpipelineDesc, IID_PPV_ARGS(&pipelinestate));
     assert(SUCCEEDED(dx.result));
#pragma endregion �p�C�v���C���X�e�[�g�̐���
     //-----------------------------

     float clearColor[] = { 0.1f,0.25f, 0.5f,0.0f }; // ���ۂ��F

#pragma endregion

     //�Q�[�����[�v�p�ϐ�--------------------------------
#pragma region �Q�[�����[�v�p�ϐ�
     float angle = 0.0f;
     const int DEFAULT_MOVE_LIMIT = 60;

     XMFLOAT3 targetVec = { 0,0,1 };
     XMFLOAT3 Angle = { 0,0,0 };
     XMFLOAT3 BeforePosition = { 0,0,0 };
     XMINT3 moveLimit = { DEFAULT_MOVE_LIMIT,DEFAULT_MOVE_LIMIT,DEFAULT_MOVE_LIMIT};
     XMINT3 moveTime = {0,0,0};
     XMFLOAT3 trackingPos = { 0,0,0 };
     XMFLOAT3 trackingDistance = { 100,100,100 };

#pragma endregion �Q�[�����[�v�p�ϐ�
     //--------------------------
     
     //�Q�[�����[�v-------------------------------------
#pragma region �Q�[�����[�v
    while (true)
    {
        input.UpDateInit(dx.result);

        dxWindow.messageUpdate();

        if (input.IsKeyDown(DIK_ESCAPE) || dxWindow.breakFlag)
        {
            break;
        }

#pragma region �X�V����
        if (input.IsKeyDown(DIK_RIGHT) || input.IsKeyDown(DIK_LEFT) || input.IsKeyDown(DIK_UP) || input.IsKeyDown(DIK_DOWN))
        {

            if (input.IsKeyDown(DIK_RIGHT)) { Angle.y += 0.05f; };
            if (input.IsKeyDown(DIK_LEFT)) { Angle.y -= 0.05f; };

            if (input.IsKeyDown(DIK_UP)) { Angle.x += 0.05f; };
            if (input.IsKeyDown(DIK_DOWN)) { Angle.x -= 0.05f; };

            targetVec.x = sinf(Angle.y);
            targetVec.y = sinf(Angle.x);
            targetVec.z = cosf(Angle.y + Angle.x);


        }

        if (input.IsKeyDown(DIK_D) || input.IsKeyDown(DIK_A) || input.IsKeyDown(DIK_W) || input.IsKeyDown(DIK_S))
        {
            XMFLOAT3 move = { 0.0f,0.0f,0.0f };
            if (input.IsKeyDown(DIK_W)) { move.z += 1.0f; }
            else if (input.IsKeyDown(DIK_S)) { move.z -= 1.0f; }

            //if (IsKeyDown(DIK_W, key)) { move.z += 1.0f; }
            //else if (IsKeyDown(DIK_S, key)) { move.z -= 1.0f; }

            matView.eye.x += targetVec.x * move.z;
            matView.eye.y += targetVec.y * move.z;
            matView.eye.z += targetVec.z * move.z;



        }

        matView.target.x = matView.eye.x + targetVec.x;
        matView.target.y = matView.eye.y + targetVec.y;
        matView.target.z = matView.eye.z + targetVec.z;


        matView.UpDateMatrixView();


        if (input.IsKeyTrigger(DIK_SPACE))
        {
            object3D[0].trackingFlag = !object3D[0].trackingFlag;
        }

        if (!object3D[0].trackingFlag)
        {
            objMaterial.constMapMaterial->color = XMFLOAT4(1.0f,0.0f,0.0f,1.0f);
        }
        else
        {
            objMaterial.constMapMaterial->color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
        }

        if (object3D[0].trackingFlag)
        {

#pragma region �ǔ����ɒǔ��悪�ړ������ꍇ�̏���
            //x��
            if (trackingPos.x != matView.target.x + (targetVec.x * trackingDistance.x))
            {
                trackingPos.x = matView.target.x + (targetVec.x * trackingDistance.x);//�ǔ����ύX
                BeforePosition.x = object3D[0].position.x;//�C�[�W���O�p�ړ��O���W�����݈ʒu�Ɠ���
                moveTime.x = 0;//�^�C�}�[���Z�b�g
            }
            //y��
            if (trackingPos.y != matView.target.y + (targetVec.y * trackingDistance.y))
            {
                trackingPos.y = matView.target.y + (targetVec.y * trackingDistance.y);//�ǔ����ύX
                BeforePosition.y = object3D[0].position.y;//�C�[�W���O�p�ړ��O���W�����݈ʒu�Ɠ���
                moveTime.y = 0;//�^�C�}�[���Z�b�g
            }
            //z��
            if (trackingPos.z != matView.target.z + (targetVec.z * trackingDistance.z))
            {
                trackingPos.z = matView.target.z + (targetVec.z * trackingDistance.z);//�ǔ����ύX
                BeforePosition.z = object3D[0].position.z;//�C�[�W���O�p�ړ��O���W�����݈ʒu�Ɠ���
                moveTime.z = 0;//�^�C�}�[���Z�b�g
            }
#pragma endregion �ǔ����ɒǔ��悪�ړ������ꍇ�̏���

#pragma region �ǔ�
            if (object3D[0].position.x != trackingPos.x)
            {
                moveTime.x++;
                object3D[0].position.x = OutQuad(BeforePosition.x, trackingPos.x, moveLimit.x, moveTime.x);
            }
            else
            {
                BeforePosition.x = object3D[0].position.x;
                moveTime.x = 0;
                moveLimit.x = DEFAULT_MOVE_LIMIT;
            }

            if (object3D[0].position.y != trackingPos.y)
            {
                moveTime.y++;
                object3D[0].position.y = OutQuad(BeforePosition.y, trackingPos.y, moveLimit.x, moveTime.y);
            }
            else
            {
                BeforePosition.y = object3D[0].position.y;
                moveTime.y = 0;
                moveLimit.y = DEFAULT_MOVE_LIMIT;
            }

            if (object3D[0].position.z != trackingPos.z)
            {
                moveTime.z++;
                object3D[0].position.z = OutQuad(BeforePosition.z, trackingPos.z, moveLimit.x, moveTime.z);
            }
            else
            {
                BeforePosition.z = object3D[0].position.z;
                moveTime.z = 0;
                moveLimit.z = DEFAULT_MOVE_LIMIT;
            }
#pragma endregion �ǔ�

        }
        for (int i = 0; i < _countof(object3D); i++)
        {
            object3D[i].Updata(matView, matProjection);
        }


        for (int i = 0; i < _countof(Rales); i++)
        {
            Rales[i].Updata(matView, matProjection);
        }

#pragma endregion �X�V����

#pragma region �`�揈��
        //�o�b�N�o�b�t�@�̔ԍ����擾�i2�Ȃ̂�0�Ԃ�1�ԁj--------------------------
        UINT bbIndex = dx.swapchain->GetCurrentBackBufferIndex();
        //-----------------------------------

        // �P�D���\�[�X�o���A�ŏ������݉\�ɕύX----
#pragma region �P�D���\�[�X�o���A�ŏ������݉\�ɕύX

        D3D12_RESOURCE_BARRIER barrierDesc{};
        barrierDesc.Transition.pResource = dx.backBuffers[bbIndex].Get(); // �o�b�N�o�b�t�@���w��
        barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT; // �\������
        barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // �`��
        dx.commandList->ResourceBarrier(1, &barrierDesc);

#pragma endregion 1�D���\�[�X�o���A�ŏ������݉\�ɕύX
        //--------------------------

        // �Q�D�`���w��----------------
#pragma region �Q�D�`���w��

// �����_�[�^�[�Q�b�g�r���[�p�f�B�X�N���v�^�q�[�v�̃n���h�����擾
        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = dx.rtvHeaps->GetCPUDescriptorHandleForHeapStart();
        rtvHandle.ptr += bbIndex * dx.device->GetDescriptorHandleIncrementSize(dx.rtvHeapDesc.Type);
        D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = depth.dsvHeap->GetCPUDescriptorHandleForHeapStart();
        dx.commandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

#pragma endregion 2�D�`���w��
        //-------------------
        
        //�R�D��ʃN���A-------------
#pragma region 3.��ʃN���A
        dx.commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
        dx.commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
#pragma endregion 3.��ʃN���A
        //---------------------------

        //�`��R�}���h------------------
#pragma region �`��R�}���h
            //�r���[�|�[�g�̐ݒ�R�}���h-----------------------------
#pragma region �r���[�|�[�g�̐ݒ�R�}���h

        D3D12_VIEWPORT viewport{};

        viewport.Width = dxWindow.window_width;
        viewport.Height = dxWindow.window_height;
        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;

        dx.commandList->RSSetViewports(1, &viewport);

#pragma endregion �r���[�|�[�g�̐ݒ�R�}���h
        //------------------------------

        //�V�U�[��`�̐ݒ�R�}���h-----------------
#pragma region �V�U�[��`�̐ݒ�R�}���h

        D3D12_RECT scissorrect{};

        scissorrect.left = 0;                                       // �؂蔲�����W��
        scissorrect.right = scissorrect.left + dxWindow.window_width;        // �؂蔲�����W�E
        scissorrect.top = 0;                                        // �؂蔲�����W��
        scissorrect.bottom = scissorrect.top + dxWindow.window_height;       // �؂蔲�����W��

        dx.commandList->RSSetScissorRects(1, &scissorrect);

#pragma endregion �V�U�[��`�̐ݒ�R�}���h
        //------------------

        dx.commandList->SetPipelineState(pipelinestate.Get());
        dx.commandList->SetGraphicsRootSignature(rootsignature.Get());
        

        //�v���~�e�B�u�`��̐ݒ�R�}���h�i�O�p�`���X�g�j--------------------------
        dx.commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        
        //�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
        dx.commandList->SetGraphicsRootConstantBufferView(0, objMaterial.constBuffMaterial->GetGPUVirtualAddress());

        //SRV�q�[�v�̐ݒ�R�}���h
        dx.commandList->SetDescriptorHeaps(1, descriptor.srvHeap.GetAddressOf());

        //SRV�q�[�v�̐擪�A�h���X���擾
        D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descriptor.srvHeap->GetGPUDescriptorHandleForHeapStart();

        //SRV�q�[�v�̐擪�ɂ���SRV���p�����[�^1�Ԃɐݒ�
        dx.commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

        for (int i = 0; i < _countof(object3D); i++)
        {
            object3D[i].Draw(dx, vbView, vertex.ibView, _countof(vertex.boxIndices));
        }

        for (int i = 0; i < _countof(Rales); i++)
        {
            Rales[i].Draw(dx,  vbView, vertex.ibView, _countof(vertex.boxIndices));
        }

#pragma endregion �`��R�}���h
        //----------------------

        // �T�D���\�[�X�o���A��߂�--------------
#pragma region 5.���\�[�X�o���A��߂�

        barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // �`��
        barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;   // �\����
        dx.commandList->ResourceBarrier(1, &barrierDesc);

#pragma endregion 5.���\�[�X�o���A��߂�
        //--------------------

        // ���߂̃N���[�Y-----------------------------------
        dx.result = dx.commandList->Close();
        assert(SUCCEEDED(dx.result));
        //------------
        

        // �R�}���h���X�g�̎��s-------------------------------------
#pragma region �R�}���h���X�g���s
        ID3D12CommandList* commandLists[] = { dx.commandList.Get() }; // �R�}���h���X�g�̔z��
        dx.commandQueue->ExecuteCommandLists(1, commandLists);

        // �o�b�t�@���t���b�v�i���\�̓��ւ�)-----------------------
       dx.result =  dx.swapchain->Present(1, 0);
       assert(SUCCEEDED(dx.result));
        //-----------------

#pragma region �R�}���h���s�����҂�
    // �R�}���h���X�g�̎��s������҂�
        dx.commandQueue->Signal(dx.fence.Get(), ++dx.fenceVal);
        if (dx.fence->GetCompletedValue() != dx.fenceVal)
        {
            HANDLE event = CreateEvent(nullptr, false, false, nullptr);
            dx.fence->SetEventOnCompletion(dx.fenceVal, event);
            WaitForSingleObject(event, INFINITE);
            CloseHandle(event);
        }
#pragma endregion �R�}���h���s�����҂�

        //�L���[���N���A
        dx.result = dx.commandAllocator->Reset(); // �L���[���N���A
        assert(SUCCEEDED(dx.result));

        //�ĂуR�}���h���X�g�����߂鏀��
        dx.result = dx.commandList->Reset(dx.commandAllocator.Get(), nullptr);  // �ĂуR�}���h���X�g�𒙂߂鏀��
        assert(SUCCEEDED(dx.result));

#pragma endregion �R�}���h���X�g���s
//------------------

#pragma endregion �`�揈��


    }
#pragma endregion �Q�[�����[�v
    //---------------------------------
	return 0;
}
