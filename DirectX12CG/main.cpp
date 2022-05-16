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
#include "Shader.h"
#include "Pipeline.h"
#include "TexSample.h"
#include "RootSignature.h"

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
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine,
	_In_ int nCmdShow) 
{  

    DxWindow* dxWindow = new DxWindow;

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
    Dx12* dx = new Dx12(*dxWindow);
    //input�N���X����
    Input* input = new Input(dx->result,dxWindow->window,dxWindow->hwnd);

#pragma endregion 


#pragma region �`�揉��������

    //�[�x�o�b�t�@----
    Depth depth(*dxWindow, *dx);
    //-------

    //3D�I�u�W�F�N�g�}�e���A���̐���-------------------
    ObjectMaterial objMaterial;
    objMaterial.Init(*dx);
    //---------------------

    //3D�I�u�W�F�N�g�̐���-------------------
#pragma region 3D�I�u�W�F�N�g�̐���
    Object3d triangle;
    triangle.Init(*dx);
    triangle.vertex.CreateModel("Resources\\triangle.obj");
    triangle.scale = { 20,20,20 };
#pragma endregion 3D�I�u�W�F�N�g�̐���
    //----------------------

     //�s��-----------------------
#pragma region �s��
        //�r���[�ϊ��s��
        View matView;
        matView.CreateMatrixView(XMFLOAT3(0.0f, 0.0f, -100.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));
        //�ˉe�ϊ��s��
        Projection matProjection;
         matProjection.CreateMatrixProjection(XMConvertToRadians(45.0f),(float)dxWindow->window_width / dxWindow->window_height, 0.1f, 1000.0f);
#pragma endregion �s��
     //---------------------

#pragma region �摜�֌W
     //�摜�t�@�C��--------------------
     TextureFile* textureFile = new TextureFile;
     dx->result = textureFile->LoadTexture(L"Resources\\tori.png", WIC_FLAGS_NONE);
     //----------------------------

     //�~�b�v�}�b�v�̐���-------------------------
     MipMap* mipmap = new MipMap;
     dx->result = mipmap->GenerateMipMap(textureFile, TEX_FILTER_DEFAULT, 0);
     //----------------------------

     //�摜�C���[�W�f�[�^�̍쐬----------------------
     TexImgData* imageData = new TexImgData;
     imageData->SetImageDataRGBA(XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
     //------------------------------------

      //�e�N�X�`���o�b�t�@�ݒ�---------------------------------------
      TextureBuffer texBuff;
      texBuff.SetTexHeapProp(D3D12_HEAP_TYPE_CUSTOM,D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,D3D12_MEMORY_POOL_L0);
      texBuff.SetTexResourceDesc(*textureFile, D3D12_RESOURCE_DIMENSION_TEXTURE2D, 1);
      //--------------------------------------


      //�e�N�X�`���o�b�t�@�̐���----------------------
      dx->result = texBuff.CommitResouce(*dx, D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr);
      texBuff.TransferMipmatToTexBuff(*textureFile, nullptr, dx->result);
      //-----------------------------------
#pragma endregion �摜�֌W

         //�f�X�N���v�^�q�[�v�̐���-------------------------
#pragma region �f�X�N���v�^�q�[�v�̐���

     const size_t kMaxSRVCount = 2056;
     Descriptor descriptor;
     descriptor.SetHeapDesc(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, kMaxSRVCount);
     dx->result = descriptor.SetDescriptorHeap(*dx);
     descriptor.SetSrvHeap();

#pragma endregion �f�X�N���v�^�q�[�v�̐���
    //-------------------------------

     //�V�F�[�_�[���\�[�X�r���[�̍쐬------------------------------
#pragma region �V�F�[�_�[���\�[�X�r���[�̍쐬

     descriptor.SetSrvDesc(texBuff, D3D12_SRV_DIMENSION_TEXTURE2D);

     descriptor.SetShaderResourceView(*dx, texBuff);

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
   
#pragma endregion ���_�f�[�^
     //--------------------------
    
     //�C���f�b�N�X�o�b�t�@�̐ݒ�-------------------------
#pragma region �C���f�b�N�X�̐ݒ�
     triangle.vertex.SetSizeIB();

     objMaterial.SetIndex(D3D12_RESOURCE_DIMENSION_BUFFER, triangle.vertex.sizeIB, 1, 1, 1, 1, D3D12_TEXTURE_LAYOUT_ROW_MAJOR);

#pragma endregion �C���f�b�N�X�̐ݒ�
     //------------------------

#pragma region �C���f�b�N�X�o�b�t�@����

     triangle.vertex.CreateIndexBuffer(*dx, objMaterial.HeapProp, D3D12_HEAP_FLAG_NONE,objMaterial.Resdesc, D3D12_RESOURCE_STATE_GENERIC_READ);

#pragma endregion �C���f�b�N�X�o�b�t�@����

     //�C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��------------------------------
#pragma region �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��

     dx->result = triangle.vertex.IndexMaping();

#pragma endregion �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
    //-------------------------------------

     //�C���f�b�N�X�o�b�t�@�r���[�̍쐬-----------------------------------
#pragma region �C���f�b�N�X�o�b�t�@�r���[�̍쐬
     triangle.vertex.SetIbView(DXGI_FORMAT_R16_UINT);
#pragma endregion �C���f�b�N�X�o�b�t�@�r���[�̍쐬
     //------------------------------------------

     //���_�o�b�t�@---------------
#pragma region ���_�o�b�t�@�̐ݒ�
     triangle.vertex.SetSizeVB();
     objMaterial.SetVertexBuffer(D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_DIMENSION_BUFFER, triangle.vertex.sizeVB, 1, 1, 1, 1, D3D12_TEXTURE_LAYOUT_ROW_MAJOR);
#pragma endregion ���_�o�b�t�@�̐ݒ�
     //----------------------------------

     // ���_�o�b�t�@�̐���----------------------------
#pragma region ���_�o�b�t�@�̐���

     triangle.vertex.CreateVertexBuffer(*dx, objMaterial.HeapProp, D3D12_HEAP_FLAG_NONE, objMaterial.Resdesc, D3D12_RESOURCE_STATE_GENERIC_READ);

#pragma endregion ���_�o�b�t�@�̐���
     //-------------------------

     //�@���x�N�g���v�Z---------------------------
#pragma region �@���x�N�g���v�Z
     //triangle.vertex.CalculationNormalVec();
#pragma endregion �@���x�N�g�����v�Z
     //-------------------------

     // ���_�o�b�t�@�ւ̃f�[�^�]��------------
#pragma region ���_�o�b�t�@�ւ̃f�[�^�]��
     triangle.vertex.VertexMaping();
#pragma endregion ���_�o�b�t�@�ւ̃f�[�^�]��
     //--------------------------------------

     // ���_�o�b�t�@�r���[�̍쐬--------------------------
#pragma region ���_�o�b�t�@�r���[�̍쐬
     triangle.vertex.SetVbView();
#pragma endregion ���_�o�b�t�@�r���[�̍쐬
     //-----------------------------------

    //�V�F�[�_�[�I�u�W�F�N�g�錾-------------------------------------------
#pragma region �V�F�[�_�[�I�u�W�F�N�g�錾
     Shader shader;
#pragma endregion �V�F�[�_�[�I�u�W�F�N�g�錾
    //---------------------------------

    // ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��--------------------------------
#pragma region ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��

     shader.ShaderCompile(L"BasicVS.hlsl", "main", VS);

#pragma endregion ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    //------------------------------------------

     // �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��-------------------------------
#pragma region �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
     
     shader.ShaderCompile(L"BasicPS.hlsl", "main", PS);

#pragma endregion �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
     //--------------------------------

     Pipeline pipleline;

     //���_�V�F�[�_�A�s�N�Z���V�F�[�_���p�C�v���C���ɐݒ�-----------------------------
#pragma region ���_�V�F�[�_�ƃs�N�Z���V�F�[�_���p�C�v���C���ɐݒ�

     pipleline.SetGpipleneDescAll(&shader);

#pragma endregion ���_�V�F�[�_�ƃs�N�Z���V�F�[�_���p�C�v���C���ɐݒ�
     //-----------------------------------

     //�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�------------------------------------
#pragma region �T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
     pipleline.SetSampleMask();

     pipleline.SetAllAddRasterizerState();
#pragma endregion �T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
     //------------------------------------


      //�u�����h�X�e�[�g�̐ݒ�-------------------------------
#pragma region �u�����h�X�e�[�g�̐ݒ�

     pipleline.SetRenderTaegetBlendDesc(pipleline.pipelineDesc.BlendState.RenderTarget[0]);

     pipleline.SetRenderTargetWriteMask();

     pipleline.SetNormalBlendDesc();

     pipleline.SetAlphaBlend();


#pragma endregion �u�����h�X�e�[�g�̐ݒ�
     //--------------------------

     //���_���C�A�E�g�̐ݒ�------------------
#pragma region ���_���C�A�E�g�̐ݒ�

     pipleline.pipelineDesc.InputLayout.pInputElementDescs = shader.inputLayout;
     pipleline.pipelineDesc.InputLayout.NumElements = _countof(shader.inputLayout);

#pragma endregion ���_���C�A�E�g�̐ݒ�
     //----------------------------

     //�}�`�̌`����O�p�`�ɐݒ�-------------------------
     pipleline.SetPrimitiveTopologyType();
     //------------------

     //���̑��̐ݒ�----------------
#pragma region ���̑��̐ݒ�

     pipleline.SetNumRenderTargets();
     pipleline.SetRTVFormats();
     pipleline.SetSampleDescCount();

#pragma endregion ���̑��̐ݒ�
    //----------------

     depth.SetDepthStencilState(pipleline.pipelineDesc);

   //�e�N�X�`���T���v���[�̐ݒ�-----------------------
#pragma region �e�N�X�`���T���v���[�̐ݒ�

     TexSample sample;
     sample.Init();

#pragma endregion �e�N�X�`���T���v���[�̐ݒ�
   //----------------------------------

     //���[�g�V�O�l�`���̐���--------------------------
#pragma region ���[�g�V�O�l�`���̐���

     RootSignature rootsignature;

     rootsignature.InitRootSignatureDesc(rootparams, sample);

     rootsignature.SetSerializeRootSignature(shader,*dx);

     rootsignature.CreateRootSignature(dx);

     // �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g

     pipleline.SetRootSignature(rootsignature);

#pragma endregion ���[�g�V�O�l�`���̐���
     //--------------------------------

    //�p�C�v���C���X�e�[�g�̐���-------------------------
#pragma region �p�C�v���C���X�e�[�g�̐���

     pipleline.CreateGraphicsPipelineState(dx);

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


#pragma endregion �Q�[�����[�v�p�ϐ�
     //--------------------------
     
     //�Q�[�����[�v-------------------------------------
#pragma region �Q�[�����[�v
    while (true)
    {
        input->UpDateInit(dx->result);

        dxWindow->messageUpdate();

        if (input->IsKeyDown(DIK_ESCAPE) || dxWindow->breakFlag)
        {
            break;
        }

#pragma region �X�V����
        if (input->IsKeyDown(DIK_RIGHT) || input->IsKeyDown(DIK_LEFT) || input->IsKeyDown(DIK_UP) || input->IsKeyDown(DIK_DOWN))
        {

            if (input->IsKeyDown(DIK_RIGHT)) { Angle.y += 0.05f; };
            if (input->IsKeyDown(DIK_LEFT)) { Angle.y -= 0.05f; };

            if (input->IsKeyDown(DIK_UP)) { Angle.x += 0.05f; };
            if (input->IsKeyDown(DIK_DOWN)) { Angle.x -= 0.05f; };

            targetVec.x = sinf(Angle.y);
            targetVec.y = sinf(Angle.x);
            targetVec.z = cosf(Angle.y + Angle.x);


        }

        if (input->IsKeyDown(DIK_D) || input->IsKeyDown(DIK_A) || input->IsKeyDown(DIK_W) || input->IsKeyDown(DIK_S))
        {
            XMFLOAT3 move = { 0.0f,0.0f,0.0f };
            if (input->IsKeyDown(DIK_W)) { move.z += 1.0f; }
            else if (input->IsKeyDown(DIK_S)) { move.z -= 1.0f; }

            //if (input.IsKeyDown(DIK_D)) { move.x += 1.0f; }
            //else if (input.IsKeyDown(DIK_A)) { move.x-= 1.0f; }

            matView.eye.x += targetVec.x * move.z;
            matView.eye.y += targetVec.y * move.z;
            matView.eye.z += targetVec.z * move.z;



        }

        matView.target.x = matView.eye.x + targetVec.x;
        matView.target.y = matView.eye.y + targetVec.y;
        matView.target.z = matView.eye.z + targetVec.z;


        matView.UpDateMatrixView();

        //for (int i = 0; i < _countof(object3D); i++)
        //{
        //    object3D[i].Updata(matView, matProjection,true);
        //}

        //for (int i = 0; i < _countof(Rales); i++)
        //{
        //    Rales[i].Updata(matView, matProjection);
        //}

        triangle.Updata(matView, matProjection, true);

#pragma endregion �X�V����

#pragma region �`�揈��
        //�o�b�N�o�b�t�@�̔ԍ����擾�i2�Ȃ̂�0�Ԃ�1�ԁj--------------------------
        UINT bbIndex = dx->swapchain->GetCurrentBackBufferIndex();
        //-----------------------------------

        // �P�D���\�[�X�o���A�ŏ������݉\�ɕύX----
#pragma region �P�D���\�[�X�o���A�ŏ������݉\�ɕύX

        D3D12_RESOURCE_BARRIER barrierDesc{};
        barrierDesc.Transition.pResource = dx->backBuffers[bbIndex].Get(); // �o�b�N�o�b�t�@���w��
        barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT; // �\������
        barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // �`��
        dx->commandList->ResourceBarrier(1, &barrierDesc);

#pragma endregion 1�D���\�[�X�o���A�ŏ������݉\�ɕύX
        //--------------------------

        // �Q�D�`���w��----------------
#pragma region �Q�D�`���w��

// �����_�[�^�[�Q�b�g�r���[�p�f�B�X�N���v�^�q�[�v�̃n���h�����擾
        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = dx->rtvHeaps->GetCPUDescriptorHandleForHeapStart();
        rtvHandle.ptr += bbIndex * dx->device->GetDescriptorHandleIncrementSize(dx->rtvHeapDesc.Type);
        D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = depth.dsvHeap->GetCPUDescriptorHandleForHeapStart();
        dx->commandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

#pragma endregion 2�D�`���w��
        //-------------------
        
        //�R�D��ʃN���A-------------
#pragma region 3.��ʃN���A
        dx->commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
        dx->commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
#pragma endregion 3.��ʃN���A
        //---------------------------

        //�`��R�}���h------------------
#pragma region �`��R�}���h
            //�r���[�|�[�g�̐ݒ�R�}���h-----------------------------
#pragma region �r���[�|�[�g�̐ݒ�R�}���h

        D3D12_VIEWPORT viewport{};

        viewport.Width = dxWindow->window_width;
        viewport.Height = dxWindow->window_height;
        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;

        dx->commandList->RSSetViewports(1, &viewport);

#pragma endregion �r���[�|�[�g�̐ݒ�R�}���h
        //------------------------------

        //�V�U�[��`�̐ݒ�R�}���h-----------------
#pragma region �V�U�[��`�̐ݒ�R�}���h

        D3D12_RECT scissorrect{};

        scissorrect.left = 0;                                       // �؂蔲�����W��
        scissorrect.right = scissorrect.left + dxWindow->window_width;        // �؂蔲�����W�E
        scissorrect.top = 0;                                        // �؂蔲�����W��
        scissorrect.bottom = scissorrect.top + dxWindow->window_height;       // �؂蔲�����W��

        dx->commandList->RSSetScissorRects(1, &scissorrect);

#pragma endregion �V�U�[��`�̐ݒ�R�}���h
        //------------------

        dx->commandList->SetPipelineState(pipleline.pipelinestate.Get());
        dx->commandList->SetGraphicsRootSignature(rootsignature.rootsignature.Get());
        

        //�v���~�e�B�u�`��̐ݒ�R�}���h�i�O�p�`���X�g�j--------------------------
        dx->commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        
        //�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
        dx->commandList->SetGraphicsRootConstantBufferView(0, objMaterial.constBuffMaterial->GetGPUVirtualAddress());

        //SRV�q�[�v�̐ݒ�R�}���h
        dx->commandList->SetDescriptorHeaps(1, descriptor.srvHeap.GetAddressOf());

        //SRV�q�[�v�̐擪�A�h���X���擾
        D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descriptor.srvHeap->GetGPUDescriptorHandleForHeapStart();

        //SRV�q�[�v�̐擪�ɂ���SRV���p�����[�^1�Ԃɐݒ�
        dx->commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

        //for (int i = 0; i < _countof(object3D); i++)
        //{
        //    object3D[i].Draw(*dx);
        //}

        //for (int i = 0; i < _countof(Rales); i++)
        //{
        //    Rales[i].Draw(*dx);
        //}

        triangle.Draw(*dx);

#pragma endregion �`��R�}���h
        //----------------------

        // �T�D���\�[�X�o���A��߂�--------------
#pragma region 5.���\�[�X�o���A��߂�

        barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // �`��
        barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;   // �\����
        dx->commandList->ResourceBarrier(1, &barrierDesc);

#pragma endregion 5.���\�[�X�o���A��߂�
        //--------------------

        // ���߂̃N���[�Y-----------------------------------
        dx->result = dx->commandList->Close();
        assert(SUCCEEDED(dx->result) && "���߃N���[�Y�i�K�ł̃G���[");
        //------------
        

        // �R�}���h���X�g�̎��s-------------------------------------
#pragma region �R�}���h���X�g���s
        ID3D12CommandList* commandLists[] = { dx->commandList.Get() }; // �R�}���h���X�g�̔z��
        dx->commandQueue->ExecuteCommandLists(1, commandLists);

        // �o�b�t�@���t���b�v�i���\�̓��ւ�)-----------------------
       dx->result =  dx->swapchain->Present(1, 0);
       assert(SUCCEEDED(dx->result) && "�o�b�t�@�t���b�v�i�K�ł̃G���[");
        //-----------------

#pragma region �R�}���h���s�����҂�
    // �R�}���h���X�g�̎��s������҂�
        dx->commandQueue->Signal(dx->fence.Get(), ++dx->fenceVal);
        if (dx->fence->GetCompletedValue() != dx->fenceVal)
        {
            HANDLE event = CreateEvent(nullptr, false, false, nullptr);
            dx->fence->SetEventOnCompletion(dx->fenceVal, event);
            WaitForSingleObject(event, INFINITE);
            CloseHandle(event);
        }
#pragma endregion �R�}���h���s�����҂�

        //�L���[���N���A
        dx->result = dx->commandAllocator->Reset(); // �L���[���N���A
        assert(SUCCEEDED(dx->result) && "�L���[�N���A�i�K�ł̃G���[");

        //�ĂуR�}���h���X�g�����߂鏀��
        dx->result = dx->commandList->Reset(dx->commandAllocator.Get(), nullptr);  // �ĂуR�}���h���X�g�𒙂߂鏀��
        assert(SUCCEEDED(dx->result) && "�R�}���h���X�g�Ē��~�����i�K�ł̃G���[");

#pragma endregion �R�}���h���X�g���s
//------------------

#pragma endregion �`�揈��


    }
#pragma endregion �Q�[�����[�v
    //---------------------------------
    delete dxWindow;
    delete dx;
    delete input;
    delete textureFile;
    delete mipmap;
    delete imageData;
	return 0;
}
