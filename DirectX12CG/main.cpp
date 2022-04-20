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





//�萔�o�b�t�@�p�\����-----------------------------------
typedef struct ConstBufferDataMaterial
{
    XMFLOAT4 color;
    XMMATRIX mat;
};
//------------------------------------------


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

     constMapMaterial->color = XMFLOAT4(1, 0, 0, 0.5f);

#pragma endregion

     //���[�g�p�����[�^�̐ݒ�---------------------------
#pragma region ���[�g�p�����[�^�̐ݒ�

     D3D12_ROOT_PARAMETER rootparam = {};
     rootparam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
     rootparam.Descriptor.ShaderRegister = 0;
     rootparam.Descriptor.RegisterSpace = 0;
     rootparam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

#pragma endregion ���[�g�p�����[�^�̐ݒ�
     //------------------------




     //���_�f�[�^---------------------------------
#pragma region ���_�f�[�^
     XMFLOAT3 vertices[] = {
         {-0.5f,-0.5f,0.0f},
         {-0.5f,+0.5f,0.0f},
         {+ 0.5f,-0.5f,0.0f},
     };

     UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * _countof(vertices));
#pragma endregion ���_�f�[�^
     //--------------------------
     
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

     // ���_�o�b�t�@�ւ̃f�[�^�]��------------
#pragma region GPU��̃o�b�t�@�ɑΉ��������z���������擾
     XMFLOAT3* vertMap = nullptr;
     dx.result = vertBuff->Map(0, nullptr, (void**)&vertMap);
     assert(SUCCEEDED(dx.result));

     // �S���_�ɑ΂���
     for (int i = 0; i < _countof(vertices); i++)
     {
         vertMap[i] = vertices[i];   // ���W���R�s�[
     }

     // �}�b�v������
     vertBuff->Unmap(0, nullptr);
#pragma endregion GPU��̃o�b�t�@�ɑΉ��������z���������擾
     //--------------------------------------

     // ���_�o�b�t�@�r���[�̍쐬--------------------------
#pragma region ���_�o�b�t�@�r���[�̍쐬
     D3D12_VERTEX_BUFFER_VIEW vbView{};

     vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
     vbView.SizeInBytes = sizeVB;
     vbView.StrideInBytes = sizeof(XMFLOAT3);
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
         //{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},//�@���x�N�g��
         //{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
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
     gpipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;  // �w�ʃJ�����O
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

     //���[�g�V�O�l�`���̐���--------------------------
#pragma region ���[�g�V�O�l�`���̐���

     ComPtr <ID3D12RootSignature> rootsignature;

     D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
     rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
     rootSignatureDesc.pParameters = &rootparam; //���[�g�p�����[�^�̐擪�A�h���X
     rootSignatureDesc.NumParameters = 1; //���[�g�p�����[�^��
     //rootSignatureDesc.pStaticSamplers = &samplerDesc;
     //rootSignatureDesc.NumStaticSamplers = 1;


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
    while (true)
    {



        input.UpDateInit(dx.result);

        dxWindow.messageUpdate();

        if (input.IsKeyDown(DIK_ESCAPE) || dxWindow.breakFlag)
        {
            break;
        }

#pragma region �X�V����


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
        dx.commandList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);

#pragma endregion 2�D�`���w��
        //-------------------
        
        //�R�D��ʃN���A-------------
#pragma region 3.��ʃN���A

        
        dx.commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
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

        //�`��R�}���h
        dx.commandList->DrawInstanced(_countof(vertices), 1, 0, 0);

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

	return 0;
}
