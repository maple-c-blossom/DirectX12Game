#pragma region include

#include <d3dcompiler.h>
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <string>
#include <DirectXMath.h>
#include <wrl.h>
#include <cassert>
#include "Input.h"
#include "DxWindow.h"
#include "Dx12.h"
#include <memory>
#include <DirectXTex.h>
#include "View.h"
#include "Projection.h"
#include "WorldMatrix.h"
#include "Depth.h"

#pragma endregion include

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

#pragma endregion using namespace





//�萔�o�b�t�@�p�\����(�}�e���A��)-----------------------------------
typedef struct ConstBufferDataMaterial
{
    XMFLOAT4 color;
};
//------------------------------------------

//�萔�o�b�t�@�p�\����(���W)------------------------
typedef struct ConstBufferDataTransform
{
    XMMATRIX mat;
};
//---------------------------------

//���_�f�[�^�\����-------------------------------------
typedef struct Vertex
{
    XMFLOAT3 pos;//xyz���W
    XMFLOAT3 normal;//�@���x�N�g��
    XMFLOAT2 uv;//uv���W
};
//--------------------------------------


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

    Dx12 dx(dxWindow);

    Input input(dx.result,dxWindow.w,dxWindow.hwnd);

#pragma endregion 


#pragma region �`�揉��������

    //�[�x�o�b�t�@----
    Depth depth(dxWindow, dx);
    //-------

    //�萔�o�b�t�@�̐���-------------------
#pragma region �萔�o�b�t�@�̐���

     D3D12_HEAP_PROPERTIES cdHeapProp{};
     cdHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

     D3D12_RESOURCE_DESC cdResdesc{};
     cdResdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
     cdResdesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;
     cdResdesc.Height = 1;
     cdResdesc.DepthOrArraySize = 1;
     cdResdesc.MipLevels = 1;
     cdResdesc.SampleDesc.Count = 1;
     cdResdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

     ComPtr<ID3D12Resource> constBuffMaterial = nullptr;


     dx.result = dx.device->CreateCommittedResource
     (
         &cdHeapProp,        //�q�[�v�ݒ�
         D3D12_HEAP_FLAG_NONE,
         &cdResdesc,//���\�[�X�ݒ�
         D3D12_RESOURCE_STATE_GENERIC_READ,
         nullptr,
         IID_PPV_ARGS(&constBuffMaterial)
     );
     assert(SUCCEEDED(dx.result));


     ConstBufferDataMaterial* constMapMaterial = nullptr;

     dx.result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);

     assert(SUCCEEDED(dx.result));

     constMapMaterial->color = XMFLOAT4(1, 1, 1, 1.0f);

#pragma endregion
     //----------------------

     //�萔�o�b�t�@�̐���-------------------
#pragma region �萔�o�b�t�@�̐���
    ComPtr<ID3D12Resource> constBuffTranceform0 = nullptr;

    ConstBufferDataTransform* constMapTranceform0 = nullptr;
     
        D3D12_HEAP_PROPERTIES cbHeapProp{};
        cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

        D3D12_RESOURCE_DESC cbResdesc{};
        cbResdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        cbResdesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
        cbResdesc.Height = 1;
        cbResdesc.DepthOrArraySize = 1;
        cbResdesc.MipLevels = 1;
        cbResdesc.SampleDesc.Count = 1;
        cbResdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

        dx.result = dx.device->CreateCommittedResource
        (
            &cbHeapProp,        //�q�[�v�ݒ�
            D3D12_HEAP_FLAG_NONE,
            &cbResdesc,//���\�[�X�ݒ�
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&constBuffTranceform0)
        );
        assert(SUCCEEDED(dx.result));

        dx.result = constBuffTranceform0->Map(0, nullptr, (void**)&constMapTranceform0);

        assert(SUCCEEDED(dx.result));
     
#pragma endregion
     //----------------------

             //�萔�o�b�t�@�̐���-------------------
#pragma region �萔�o�b�t�@�̐���
        ComPtr<ID3D12Resource> constBuffTranceform1 = nullptr;

        ConstBufferDataTransform* constMapTranceform1 = nullptr;

        dx.result = dx.device->CreateCommittedResource
        (
            &cbHeapProp,        //�q�[�v�ݒ�
            D3D12_HEAP_FLAG_NONE,
            &cbResdesc,//���\�[�X�ݒ�
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&constBuffTranceform1)
        );
        assert(SUCCEEDED(dx.result));

        dx.result = constBuffTranceform1->Map(0, nullptr, (void**)&constMapTranceform1);

        assert(SUCCEEDED(dx.result));

#pragma endregion
        //----------------------


     //�s��-----------------------
#pragma region �s��
        //���[���h�s��
        WorldMatrix matWorld;
        matWorld.CreateMatrixWorld(XMMatrixScaling(1.0f, 0.5f, 1.0f), matWorld.ReturnMatRot(matWorld.matRot,15.0f,30.0f,0.0f), XMMatrixTranslation(-50.0f, 0.0f, 0.0f));
        //�r���[�ϊ��s��
        View matView;
        matView.CreateMatrixView(XMFLOAT3(0.0f, 0.0f, -100.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));
        //�ˉe�ϊ��s��
        Projection matProjection;
         matProjection.CreateMatrixProjection(XMConvertToRadians(45.0f),(float)dxWindow.window_width / dxWindow.window_height, 0.1f, 1000.0f);

        constMapTranceform0->mat = matWorld.matWorld * matView.mat * matProjection.mat;

        //���[���h�s��
        WorldMatrix matWorld1;
        matWorld1.CreateMatrixWorld(XMMatrixScaling(1.0f, 0.5f, 1.0f), matWorld1.ReturnMatRot(matWorld1.matRot, 15.0f, 30.0f, 0.0f), XMMatrixTranslation(-50.0f, 0.0f, 0.0f));

        constMapTranceform1->mat = matWorld1.matWorld * matView.mat * matProjection.mat;



#pragma endregion �s��
     //---------------------


     //�摜�t�@�C��--------------------
#pragma region �摜�t�@�C��
     TexMetadata metadata{};
     ScratchImage scratchImg{};

     dx.result = LoadFromWICFile(L"Resources\\tori.png", WIC_FLAGS_NONE, &metadata, scratchImg);
#pragma endregion �摜�t�@�C��
     //----------------------------

     //�~�b�v�}�b�v�̐���-------------
#pragma region �~�b�v�}�b�v�̐���

     ScratchImage mipChain{};
     //�~�b�v�}�b�v����
     dx.result = GenerateMipMaps(scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(), TEX_FILTER_DEFAULT, 0, mipChain);
     if (SUCCEEDED(dx.result))
     {
         scratchImg = std::move(mipChain);
         metadata = scratchImg.GetMetadata();
     }
     metadata.format = MakeSRGB(metadata.format);
#pragma endregion �~�b�v�}�b�v�̐���
     //---------------

     //�摜�C���[�W�f�[�^�̍쐬----------------------
#pragma region �摜�C���[�W�f�[�^�̍쐬
     //�������s�N�Z����
     const size_t textureWidth = 256;
     //�c�����s�N�Z����
     const size_t textureHeight = 256;
     //�z��̗v�f��
     const size_t imageDataCount = textureWidth * textureHeight;
     //�摜�C���[�W�f�[�^�̔z��
     XMFLOAT4* imageData = new XMFLOAT4[imageDataCount];

     for (int i = 0; i < imageDataCount; i++)
     {
         imageData[i].x = 1.0f;//R
         imageData[i].y = 0.0f;//G
         imageData[i].z = 0.0f;//B
         imageData[i].w = 1.0f;//A
     }

#pragma endregion �摜�C���[�W�f�[�^�̍쐬
     //------------------------------------

#pragma region �e�N�X�`���o�b�t�@�ݒ�

     D3D12_HEAP_PROPERTIES texHeapProp{};

     texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
     texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
     texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

     D3D12_RESOURCE_DESC texresDesc{};
     texresDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
     texresDesc.Format = metadata.format;
     texresDesc.Width = metadata.width;
     texresDesc.Height = (UINT)metadata.height;
     texresDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
     texresDesc.MipLevels = (UINT16)metadata.mipLevels;
     texresDesc.SampleDesc.Count = 1;


#pragma endregion �e�N�X�`���o�b�t�@�ݒ�



#pragma region �e�N�X�`���o�b�t�@�̐���
     ComPtr<ID3D12Resource> texbuff = nullptr;
     dx.result = dx.device->CreateCommittedResource(&texHeapProp, D3D12_HEAP_FLAG_NONE, &texresDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&texbuff));
#pragma endregion �e�N�X�`���o�b�t�@�̐���

     for (size_t i = 0; i < metadata.mipLevels; i++)
     {
         //�~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
         const Image* img = scratchImg.GetImage(i, 0, 0);
         //�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
         dx.result = texbuff->WriteToSubresource((UINT) i, nullptr,img->pixels, (UINT)img->rowPitch,(UINT)img->slicePitch);
         assert(SUCCEEDED(dx.result));
     }

         //�f�X�N���v�^�q�[�v�̐���-------------------------
#pragma region �f�X�N���v�^�q�[�v�̐���

     const size_t kMaxSRVCount = 2056;

    //�萔�o�b�t�@�p�̃f�X�N���v�^�q�[�v

     //�ݒ�\����
     D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc{};
     srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
     srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; //�V�F�[�_�[���猩����悤��
     srvHeapDesc.NumDescriptors = kMaxSRVCount;//�萔�o�b�t�@�̐�

     //�f�X�N���v�^�q�[�v�̐���  
     ComPtr<ID3D12DescriptorHeap> srvHeap = nullptr;
     dx.result = dx.device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));

     D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();
#pragma endregion �f�X�N���v�^�q�[�v�̐���
    //-------------------------------

     //�V�F�[�_�[���\�[�X�r���[�̍쐬------------------------------
#pragma region �V�F�[�_�[���\�[�X�r���[�̍쐬
     //�V�F�[�_�[���\�[�X�r���[�ݒ�
     D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};

     srvDesc.Format = texresDesc.Format;
     srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
     srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
     srvDesc.Texture2D.MipLevels = texresDesc.MipLevels;

     //�q�[�v�̓�ԖڂɃV�F�[�_�[���\�[�X�r���[�쐬
     dx.device->CreateShaderResourceView(texbuff.Get(), &srvDesc, srvHandle);

#pragma endregion �V�F�[�_�[���\�[�X�r���[�̍쐬
     //----------------------------

     //�f�X�N���v�^�����W�̐ݒ�--------------------------------
#pragma region �f�X�N���v�^�����W�̐ݒ�
     D3D12_DESCRIPTOR_RANGE descriptorRange{};
     descriptorRange.NumDescriptors = 1;
     descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
     descriptorRange.BaseShaderRegister = 0;
     descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
#pragma endregion �f�X�N���v�^�����W�̐ݒ�
     //-----------------------------------------

     //���[�g�p�����[�^�̐ݒ�---------------------------
#pragma region ���[�g�p�����[�^�̐ݒ�

     D3D12_ROOT_PARAMETER rootparams[3] = {};
     //�萔�o�b�t�@0��
     rootparams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//���
     rootparams[0].Descriptor.ShaderRegister = 0;//�萔�o�b�t�@�ԍ�
     rootparams[0].Descriptor.RegisterSpace = 0;//�f�t�H���g�l
     rootparams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//�S�ẴV�F�[�_�[���猩����
     //�e�N�X�`�����W�X�^0��
     rootparams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
     rootparams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;
     rootparams[1].DescriptorTable.NumDescriptorRanges = 1;
     rootparams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
     //�萔�o�b�t�@1��
     rootparams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
     rootparams[2].Descriptor.ShaderRegister = 1;
     rootparams[2].Descriptor.RegisterSpace = 0;
     rootparams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
#pragma endregion ���[�g�p�����[�^�̐ݒ�
     //------------------------

     //���_�f�[�^---------------------------------
#pragma region ���_�f�[�^
    Vertex vertices[] =
    {
        //�O
        {{-5.0f,-5.0f,-5.0f} ,{} ,{0.0f,1.0f}},// ����(x,y,z,u,v)
        {{-5.0f,5.0f,-5.0f}  ,{} ,{0.0f,0.0f}},// ����
        {{5.0f,-5.0f,-5.0f}  ,{} ,{1.0f,1.0f}},// �E��
        {{5.0f,5.0f,-5.0f}   ,{} ,{1.0f,0.0f}},// �E��
        //���
        {{-5.0f,5.0f,5.0f}  ,{} ,{0.0f,0.0f}},// ����
        {{-5.0f,-5.0f,5.0f} ,{} ,{0.0f,1.0f}},// ����(x,y,z,u,v)
        {{5.0f,5.0f,5.0f}   ,{} ,{1.0f,0.0f}},// �E��
        {{5.0f,-5.0f,5.0f}  ,{} ,{1.0f,1.0f}},// �E��
        //��
        {{-5.0f,-5.0f,-5.0f} ,{} ,{0.0f,1.0f}},// ����(x,y,z,u,v)
        {{-5.0f,-5.0f,5.0f}  ,{} ,{0.0f,0.0f}},// ����
        {{-5.0f,5.0f,-5.0f}  ,{} ,{1.0f,1.0f}},// �E��
        {{-5.0f,5.0f,5.0f}   ,{} ,{1.0f,0.0f}},// �E��
        //�E
        {{5.0f,-5.0f,5.0f}  ,{} ,{0.0f,0.0f}},// ����
        {{5.0f,-5.0f,-5.0f} ,{} ,{0.0f,1.0f}},// ����(x,y,z,u,v)
        {{5.0f,5.0f,5.0f}   ,{} ,{1.0f,0.0f}},// �E��
        {{5.0f,5.0f,-5.0f}  ,{} ,{1.0f,1.0f}},// �E��
        //��
        {{-5.0f,-5.0f,-5.0f} ,{} ,{0.0f,1.0f}},// ����(x,y,z,u,v)
        {{5.0f,-5.0f,-5.0f}  ,{} ,{0.0f,0.0f}},// ����
        {{-5.0f,-5.0f,5.0f}  ,{} ,{1.0f,1.0f}},// �E��
        {{5.0f,-5.0f,5.0f}   ,{} ,{1.0f,0.0f}},// �E��
        //��
        {{5.0f,5.0f,-5.0f}  ,{} ,{0.0f,0.0f}},// ����
        {{-5.0f,5.0f,-5.0f} ,{} ,{0.0f,1.0f}},// ����(x,y,z,u,v)
        {{5.0f,5.0f,5.0f}   ,{} ,{1.0f,0.0f}},// �E��
        {{-5.0f,5.0f,5.0f}  ,{} ,{1.0f,1.0f}},// �E��
    };





     UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));
#pragma endregion ���_�f�[�^
     //--------------------------
     
     //���_�C���f�b�N�X---------------------------
#pragma region ���_�C���f�b�N�X
      unsigned short indices[]
      {
          //�O
          0,1,2,
          2,1,3,
          //���
          4,5,6,
          6,5,7,
          //��
          8,9,10,
          10,9,11,
          //�E
          12,13,14,
          14,13,15,
          //��
          16,17,18,
          18,17,19,
          //��
          20,21,22,
          22,21,23,
      };
#pragma endregion ���_�C���f�b�N�X
     //--------------------------

     //�C���f�b�N�X�o�b�t�@�̐ݒ�-------------------------
#pragma region �C���f�b�N�X�̐ݒ�
     //�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
     UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));

     cdResdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
     cdResdesc.Width = sizeIB;
     cdResdesc.Height = 1;
     cdResdesc.DepthOrArraySize = 1;
     cdResdesc.MipLevels = 1;
     cdResdesc.SampleDesc.Count = 1;
     cdResdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

#pragma endregion �C���f�b�N�X�̐ݒ�
     //------------------------

#pragma region �C���f�b�N�X�o�b�t�@����

     ComPtr<ID3D12Resource> indexBuff = nullptr;
         //�C���f�b�N�X�o�b�t�@�̐���-----------------------------
     dx.result = dx.device->CreateCommittedResource(
         &cdHeapProp,
         D3D12_HEAP_FLAG_NONE,
         &cdResdesc,
         D3D12_RESOURCE_STATE_GENERIC_READ,
         nullptr,
         IID_PPV_ARGS(&indexBuff)
     );

#pragma endregion �C���f�b�N�X�o�b�t�@����

     //�C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��------------------------------
#pragma region �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��

     //GPU��̃o�b�t�@�ɑΉ��������z���������擾----------------------------
     uint16_t* indexMap = nullptr;
     dx.result = indexBuff->Map(0, nullptr, (void**)&indexMap);
     //---------------------------------------

     //�S�C���f�b�N�X�ɑ΂���-------------------------
     for (int i = 0; i < _countof(indices); i++)
     {
         indexMap[i] = indices[i];
     }
     //-----------------------

     //�q���������---------------------
     indexBuff->Unmap(0, nullptr);
     //------------------------

#pragma endregion �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
    //-------------------------------------

     //�C���f�b�N�X�o�b�t�@�r���[�̍쐬-----------------------------------
#pragma region �C���f�b�N�X�o�b�t�@�r���[�̍쐬
     D3D12_INDEX_BUFFER_VIEW ibView{};
     ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
     ibView.Format = DXGI_FORMAT_R16_UINT;
     ibView.SizeInBytes = sizeIB;
#pragma endregion �C���f�b�N�X�o�b�t�@�r���[�̍쐬
     //------------------------------------------

     //���_�o�b�t�@---------------
#pragma region ���_�o�b�t�@�̐ݒ�
    D3D12_HEAP_PROPERTIES heapprop{};   // �q�[�v�ݒ�
    cdHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p

    D3D12_RESOURCE_DESC resdesc{};  // ���\�[�X�ݒ�
    cdResdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    cdResdesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
    cdResdesc.Height = 1;
    cdResdesc.DepthOrArraySize = 1;
    cdResdesc.MipLevels = 1;
    cdResdesc.SampleDesc.Count = 1;
    cdResdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
#pragma endregion ���_�o�b�t�@�̐ݒ�
     //----------------------------------

     // ���_�o�b�t�@�̐���----------------------------
#pragma region ���_�o�b�t�@�̐���
     ComPtr<ID3D12Resource> vertBuff = nullptr;
     dx.result = dx.device->CreateCommittedResource(
         &cdHeapProp, // �q�[�v�ݒ�
         D3D12_HEAP_FLAG_NONE,
         &cdResdesc, // ���\�[�X�ݒ�
         D3D12_RESOURCE_STATE_GENERIC_READ,
         nullptr,
         IID_PPV_ARGS(&vertBuff));

     assert(SUCCEEDED(dx.result));
#pragma endregion ���_�o�b�t�@�̐���
     //-------------------------

     //�@���x�N�g���v�Z---------------------------
#pragma region �@���x�N�g���v�Z
     for (int i = 0; i < _countof(indices) / 3; i++)
     {
         //�O�p�`1���ƂɌv�Z

         //�O�p�`�̃C���f�b�N�X�����o���āA�ꎞ�I�ȕϐ��ɓ����
         unsigned short index0 = indices[i * 3 + 0];
         unsigned short index1 = indices[i * 3 + 1];
         unsigned short index2 = indices[i * 3 + 2];

         //�O�p�`���\�����钸�_���W
         XMVECTOR p0 = XMLoadFloat3(&vertices[index0].pos);
         XMVECTOR p1 = XMLoadFloat3(&vertices[index1].pos);
         XMVECTOR p2 = XMLoadFloat3(&vertices[index2].pos);

         //p0->p1�x�N�g���Ap0->p2�x�N�g�����v�Z�i�x�N�g���̌��Z�j
         XMVECTOR v1 = XMVectorSubtract(p1, p0);
         XMVECTOR v2 = XMVectorSubtract(p2, p0);

         //�O�ς͗������琂���ȃx�N�g��
         XMVECTOR normal = XMVector3Cross(v1, v2);

         //���K���i��������ɂ���)
         normal = XMVector3Normalize(normal);

         //���߂��@���𒸓_�f�[�^�ɑ��
         XMStoreFloat3(&vertices[index0].normal, normal);
         XMStoreFloat3(&vertices[index1].normal, normal);
         XMStoreFloat3(&vertices[index2].normal, normal);

     }
#pragma endregion �@���x�N�g�����v�Z
     //-------------------------

     // ���_�o�b�t�@�ւ̃f�[�^�]��------------
#pragma region ���_�o�b�t�@�ւ̃f�[�^�]��
     Vertex* vertMap = nullptr;
     dx.result = vertBuff->Map(0, nullptr, (void**)&vertMap);
     assert(SUCCEEDED(dx.result));

     // �S���_�ɑ΂���
     for (int i = 0; i < _countof(vertices); i++)
     {
         vertMap[i] = vertices[i];   // ���W���R�s�[
     }

     // �}�b�v������
     vertBuff->Unmap(0, nullptr);
#pragma endregion ���_�o�b�t�@�ւ̃f�[�^�]��
     //--------------------------------------

     // ���_�o�b�t�@�r���[�̍쐬--------------------------
#pragma region ���_�o�b�t�@�r���[�̍쐬
     D3D12_VERTEX_BUFFER_VIEW vbView{};

     vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
     vbView.SizeInBytes = sizeVB;
     vbView.StrideInBytes = sizeof(vertices[0]);
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
     rootSignatureDesc.pParameters = rootparams; //���[�g�p�����[�^�̐擪�A�h���X
     rootSignatureDesc.NumParameters = _countof(rootparams); //���[�g�p�����[�^��
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

     XMFLOAT3 scale = { 1.0f,1.0f,1.0f };
     XMFLOAT3 rotasion = {0.0f,0.0f,0.0f};
     XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };

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
        if (input.IsKeyDown(DIK_D) || input.IsKeyDown(DIK_A))
        {
            if (input.IsKeyDown(DIK_D)) { angle += XMConvertToRadians(1.0f); }
            else if (input.IsKeyDown(DIK_A)) { angle -= XMConvertToRadians(1.0f); }

            matView.eye.x = -100.0f * sinf(angle);
            matView.eye.z = -100.0f * cosf(angle);
            matView.UpDateMatrixView();

        }

        if (input.IsKeyDown(DIK_UP) || input.IsKeyDown(DIK_DOWN) || input.IsKeyDown(DIK_LEFT) || input.IsKeyDown(DIK_RIGHT))
        {
            if (input.IsKeyDown(DIK_UP)) { position.z += 1.0f; }
            else if (input.IsKeyDown(DIK_DOWN)) { position.z -= 1.0f; }

            if (input.IsKeyDown(DIK_RIGHT)) { position.x += 1.0f; }
            else if (input.IsKeyDown(DIK_LEFT)) { position.x -= 1.0f; }
        }

        matWorld.SetMatScale(scale.x, scale.y, scale.z);

        matWorld.SetMatRot(rotasion.x, rotasion.y, rotasion.z, false);

        matWorld.SetMatTrans(position.x, position.y, position.z);

        matWorld.UpdataMatrixWorld();

        matWorld1.SetMatScale(1.0f, 1.0f, 1.0f);

        matWorld1.matRot = XMMatrixRotationY(XM_PI / 4.0f);

        matWorld1.SetMatTrans(-position.x, -position.y, -position.z);

        matWorld1.UpdataMatrixWorld();

        constMapTranceform0->mat = matWorld.matWorld * matView.mat * matProjection.mat;

        constMapTranceform1->mat = matWorld1.matWorld * matView.mat * matProjection.mat;
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
        dx.commandList->IASetIndexBuffer(&ibView);

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
        dx.commandList->IASetVertexBuffers(0, 1, &vbView);

        
        //�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
        dx.commandList->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());

        //SRV�q�[�v�̐ݒ�R�}���h
        dx.commandList->SetDescriptorHeaps(1, srvHeap.GetAddressOf());

        //SRV�q�[�v�̐擪�A�h���X���擾
        D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();

        //SRV�q�[�v�̐擪�ɂ���SRV���p�����[�^1�Ԃɐݒ�
        dx.commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

        //�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
        dx.commandList->SetGraphicsRootConstantBufferView(2, constBuffTranceform0->GetGPUVirtualAddress());

        //�`��R�}���h
        dx.commandList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);


        //�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
        dx.commandList->SetGraphicsRootConstantBufferView(2, constBuffTranceform1->GetGPUVirtualAddress());

        //�`��R�}���h
        dx.commandList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);

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
