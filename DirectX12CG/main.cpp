#pragma region include

#include <d3dcompiler.h>
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <string>
#include <DirectXMath.h>
#include <dinput.h>
#include <wrl.h>

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

// �E�B���h�E�v���V�[�W�� 
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {  // ���b�Z�[�W�ɉ����ăQ�[���ŗL�̏������s�� 
	switch (msg) {
		// �E�B���h�E���j�����ꂽ 
	case WM_DESTROY:
		// OS�ɑ΂��āA�A�v���̏I����`���� 
		PostQuitMessage(0);
		return 0;
	}
	// �W���̃��b�Z�[�W�������s�� 
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

//�L�[�{�[�h������-------------------------------
void KeyInit(BYTE* key, BYTE* oldkey, int ArraySize)
{
    for (int i = 0; i < ArraySize; i++)
    {
        oldkey[i] = key[i];
    }



}
//---------------------

//���͊֐�-----------------------------------
#pragma region ���͊֐�
//�����Ă�����
bool IsKeyDown(int keyNum, BYTE* key)
{
    if (key[keyNum])
    {
        return true;
    }
    return false;
};
//�����Ă��Ȃ����
bool IsKeyNDown(int keyNum, BYTE* key)
{
    if (!key[keyNum])
    {
        return true;
    }
    return false;
};
//���������
bool IsKeyPress(int keyNum, BYTE* key, BYTE* oldkey)
{
    if (key[keyNum] && oldkey[keyNum])
    {
        return true;
    }
    return false;
};
//�������u��
bool IsKeyTrigger(int keyNum, BYTE* key, BYTE* oldkey)
{
    if (key[keyNum] && !oldkey[keyNum])
    {
        return true;
    }
    return false;
};
//�������u��
bool IsKeyRelease(int keyNum, BYTE* key, BYTE* oldkey)
{
    if (!key[keyNum] && oldkey[keyNum])
    {
        return true;
    }
    return false;
};
#pragma endregion ���͊֐�
//--------------------


// Windows�A�v���ł̃G���g���[�|�C���g(main�֐�) 
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) 
{  
	

  // �E�B���h�E�̐����i�O����-----------------------
#pragma region �E�B���h�E����
    // �E�B���h�E�T�C�Y
    const int window_width = 1280;  // ����
    const int window_height = 720;  // �c��

    WNDCLASSEX w{}; // �E�B���h�E�N���X�̐ݒ�
    w.cbSize = sizeof(WNDCLASSEX);
    w.lpfnWndProc = (WNDPROC)WindowProc; // �E�B���h�E�v���V�[�W����ݒ�
    w.lpszClassName = L"DirectXGame"; // �E�B���h�E�N���X��
    w.hInstance = GetModuleHandle(nullptr); // �E�B���h�E�n���h��
    w.hCursor = LoadCursor(NULL, IDC_ARROW); // �J�[�\���w��

    // �E�B���h�E�N���X��OS�ɓo�^
    RegisterClassEx(&w);
    // �E�B���h�E�T�C�Y{ X���W Y���W ���� �c�� }
    RECT wrc = { 0, 0, window_width, window_height };
    AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false); // �����ŃT�C�Y�␳


    // �E�B���h�E�I�u�W�F�N�g�̐���
    HWND hwnd = CreateWindow(w.lpszClassName, // �N���X��
        L"DirectX12Game",         // �^�C�g���o�[�̕���
        WS_OVERLAPPEDWINDOW,        // �W���I�ȃE�B���h�E�X�^�C��
        CW_USEDEFAULT,              // �\��X���W�iOS�ɔC����j
        CW_USEDEFAULT,              // �\��Y���W�iOS�ɔC����j
        wrc.right - wrc.left,       // �E�B���h�E����
        wrc.bottom - wrc.top,   // �E�B���h�E�c��
        nullptr,                // �e�E�B���h�E�n���h��
        nullptr,                // ���j���[�n���h��
        w.hInstance,            // �Ăяo���A�v���P�[�V�����n���h��
        nullptr);               // �I�v�V����

    // �E�B���h�E�\��
    ShowWindow(hwnd, SW_SHOW);

#pragma endregion �E�B���h�E����
    //-------------

    MSG msg{}; //���b�Z�[�W��������

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


    //��{�ϐ�������-----
#pragma region ��{�ϐ�������

    HRESULT result;
    ComPtr<ID3D12Device> device = nullptr;
    ComPtr<IDXGIFactory6> dxgiFactory = nullptr;
    ComPtr<IDXGISwapChain4> swapchain = nullptr;
    ComPtr<ID3D12CommandAllocator> commandAllocator = nullptr;
    ComPtr<ID3D12GraphicsCommandList> commandList = nullptr;
    ComPtr<ID3D12CommandQueue> commandQueue = nullptr;
    ComPtr<ID3D12DescriptorHeap> rtvHeaps = nullptr;

#pragma endregion ��{�ϐ�������
    //--------------

       //DXGI�t�@�N�g���[����----------------
#pragma region DXGI�t�@�N�g���[����

//DXGI�t�@�N�g���[�̐���
    result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
    assert(SUCCEEDED(result));

#pragma endregion DXGI�t�@�N�g���[����
    //-------------------

    //�A�_�v�^�[�ݒ肱������--------------------------------------------------------------------------------------------
#pragma region �A�_�v�^�[�ݒ�

    //�A�_�v�^�[�̗񋓗p
    vector<ComPtr<IDXGIAdapter4>> adapters;
    //�����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������
    ComPtr <IDXGIAdapter4> tmpAdapter = nullptr;

    for (UINT i = 0; dxgiFactory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND; i++)
    {
        adapters.push_back(tmpAdapter);
    }

    for (size_t i = 0; i < adapters.size(); i++)
    {
        DXGI_ADAPTER_DESC3 adapterDesc;
        adapters[i]->GetDesc3(&adapterDesc);

        if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE))
        {
            tmpAdapter = adapters[i];
            break;
        }
    }

#pragma endregion �A�_�v�^�[�ݒ�
    //�A�_�v�^�[�ݒ肱���܂�-----------------------------------------------------------------------

    //�f�o�C�X������������----------------------------------------------------------------------------------------------------------
#pragma region �f�o�C�X����

    //�Ή����x���̔z��
    D3D_FEATURE_LEVEL levels[] =
    {
        D3D_FEATURE_LEVEL_12_1,
        D3D_FEATURE_LEVEL_12_0,
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };

    D3D_FEATURE_LEVEL featureLevel;

    for (int i = 0; i < _countof(levels); i++)
    {
        //�̗p�����A�_�v�^�[�Ńf�o�C�X�𐶐�
        result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(&device));
        if (result == S_OK)
        {
            featureLevel = levels[i];
            break;
        }
    }

#pragma endregion �f�o�C�X����
    //�f�o�C�X���������܂�----------------------------------------------------------------------

     //�R�}���h���X�g��������--------------------------------------------------------------------------------------------------------
#pragma region �R�}���h���X�g

    //�R�}���h�A���P�[�^�𐶐�
    result = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
    assert(SUCCEEDED(result));

    //�R�}���h���X�g�𐶐�
    result = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList));
    assert(SUCCEEDED(result));


    //�R�}���h�L���[�̐���
    //�W���ݒ�ŃR�}���h�L���[�𐶐�
    D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
    result = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
    assert(SUCCEEDED(result));

#pragma endregion �R�}���h���X�g
    //�R�}���h���X�g�����܂�------------------------------------------------------------------------------------------


    //�X���b�v�`�F�[���̐�����������------------------------------------------------------------------
#pragma region �X���b�v�`�F�[���̐���
    ComPtr<IDXGISwapChain1> swapchain1;

    //�e��ݒ�
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
    swapChainDesc.Width = window_width;
    swapChainDesc.Height = window_height;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
    swapChainDesc.BufferCount = 2;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

     result = dxgiFactory->CreateSwapChainForHwnd(commandQueue.Get(), hwnd, &swapChainDesc, nullptr, nullptr, &swapchain1);

     assert(SUCCEEDED(result));

    result = swapchain1.As(&swapchain);
     assert(SUCCEEDED(result));



#pragma endregion �X���b�v�`�F�[���̐���
    //�X���b�v�`�F�[���̐��������܂�--------------------------------------------------------------

     //�����_�[�^�[�Q�b�g�r���[�����֌W��������---------------------------------------------------------------------------------------------------
#pragma region �����_�[�^�[�Q�b�g�r���[�����֌W

    //�f�X�N���v�^�q�[�v�𐶐���������--------------
#pragma region �f�X�N���v�^�q�[�v����

//�e��ݒ�
     D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
     rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
     rtvHeapDesc.NumDescriptors = swapChainDesc.BufferCount;
     device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeaps));

#pragma endregion �f�X�N���v�^�q�[�v����
     //�f�X�N���v�^�q�[�v�𐶐������܂�------------

     //�_�u���o�b�t�@----------------------
#pragma region �_�u���o�b�t�@

     vector<ComPtr<ID3D12Resource>> backBuffers(2);

     backBuffers.resize(swapChainDesc.BufferCount);

     for (size_t i = 0; i < backBuffers.size(); i++)
     {
         //�X���b�v�`�F�[������o�b�t�@���擾
         swapchain->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers[i]));
         //�ŃX�N���v�^�q�[�v�n���h�����擾
         D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
         //�����\���ŃA�h���X�������
         rtvHandle.ptr += i * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
         //�����_�[�^�[�Q�b�g�r���[�̐ݒ�
         D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
         //�V�F�[�_�[�̌v�Z���ʂ�SRGB�ɕϊ����ď�������
         rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
         rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
         //�����_�[�^�[�Q�b�g�r���[�̐���
         device->CreateRenderTargetView(backBuffers[i].Get(), &rtvDesc, rtvHandle);

     }

#pragma endregion �_�u���o�b�t�@
     //------------------

#pragma endregion �����_�[�^�[�Q�b�g�r���[�����֌W



         //�t�F���X�̐���---------
#pragma region �t�F���X�̐���

     ComPtr<ID3D12Fence> fence = nullptr;
     UINT64 fenceVal = 0;

     result = device->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

#pragma endregion �t�F���X�̐���
     //------------

         //���͌n������--------------
#pragma region ���͌n������

     IDirectInput8* dinput = nullptr;
     result = DirectInput8Create(w.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);
     assert(SUCCEEDED(result));
     //�L�[�{�[�h�f�o�C�X�̐���-----------------
#pragma region �L�[�{�[�h�f�o�C�X�̐���

     IDirectInputDevice8* devkeyboard = nullptr;
     result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
     assert(SUCCEEDED(result));

#pragma endregion �L�[�{�[�h�f�o�C�X�̐���
     //--------------------------

     //���̓f�[�^�`���Z�b�g--------------------------------
     result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);
     assert(SUCCEEDED(result));
     //---------------------------------

     //�r�����x������-------------------------------------------------------------
     result = devkeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
     assert(SUCCEEDED(result));

     //---------------------------------------------------

     //�L�[������----------------------
     BYTE key[256] = {};
     BYTE oldkey[256] = {};
     //-----------------------

#pragma endregion ���͌n������
//----------------

#pragma endregion 


#pragma region �`�揉��������

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
    heapprop.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p

    D3D12_RESOURCE_DESC resdesc{};  // ���\�[�X�ݒ�
    resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resdesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
    resdesc.Height = 1;
    resdesc.DepthOrArraySize = 1;
    resdesc.MipLevels = 1;
    resdesc.SampleDesc.Count = 1;
    resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
#pragma endregion ���_�o�b�t�@�̐ݒ�
     //----------------------------------

     // ���_�o�b�t�@�̐���----------------------------
#pragma region ���_�o�b�t�@�̐���
     ComPtr<ID3D12Resource> vertBuff = nullptr;
     result = device->CreateCommittedResource(
         &heapprop, // �q�[�v�ݒ�
         D3D12_HEAP_FLAG_NONE,
         &resdesc, // ���\�[�X�ݒ�
         D3D12_RESOURCE_STATE_GENERIC_READ,
         nullptr,
         IID_PPV_ARGS(&vertBuff));

     assert(SUCCEEDED(result));
#pragma endregion ���_�o�b�t�@�̐���
     //-------------------------

     // ���_�o�b�t�@�ւ̃f�[�^�]��------------
#pragma region GPU��̃o�b�t�@�ɑΉ��������z���������擾
     XMFLOAT3* vertMap = nullptr;
     result = vertBuff->Map(0, nullptr, (void**)&vertMap);
     assert(SUCCEEDED(result));

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

     result = D3DCompileFromFile(
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

     if (FAILED(result)) {
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

     result = D3DCompileFromFile(
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

     if (FAILED(result)) {
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

     ////���ʐݒ�
     //blenddesc.BlendEnable = true;
     //blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
     //blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
     //blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

     ////���Z����
     //blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
     //blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
     //blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
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
     //rootSignatureDesc.pParameters = rootparams; //���[�g�p�����[�^�̐擪�A�h���X
     //rootSignatureDesc.NumParameters = _countof(rootparams); //���[�g�p�����[�^��
     //rootSignatureDesc.pStaticSamplers = &samplerDesc;
     //rootSignatureDesc.NumStaticSamplers = 1;


     ComPtr<ID3DBlob> rootSigBlob = nullptr;
     result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
     result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));

     assert(SUCCEEDED(result));

     // �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
     gpipelineDesc.pRootSignature = rootsignature.Get();

#pragma endregion ���[�g�V�O�l�`���̐���
     //--------------------------------

    //�p�C�v���C���X�e�[�g�̐���-------------------------
#pragma region �p�C�v���C���X�e�[�g�̐���

     ComPtr<ID3D12PipelineState> pipelinestate = nullptr;
     result = device->CreateGraphicsPipelineState(&gpipelineDesc, IID_PPV_ARGS(&pipelinestate));
     assert(SUCCEEDED(result));
#pragma endregion �p�C�v���C���X�e�[�g�̐���
     //-----------------------------

     float clearColor[] = { 0.1f,0.25f, 0.5f,0.0f }; // ���ۂ��F

#pragma endregion
    while (true)
    {

        //���b�Z�[�W�֌W------------------
#pragma region ���b�Z�[�W�֌W

// ���b�Z�[�W������H
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) 
        {
            TranslateMessage(&msg); // �L�[���̓��b�Z�[�W�̏���
            DispatchMessage(&msg); // �v���V�[�W���Ƀ��b�Z�[�W�𑗂�
        }

        // x�{�^���ŏI�����b�Z�[�W��������Q�[�����[�v�𔲂���
        if (msg.message == WM_QUIT || key[DIK_ESCAPE])
        {
            break;
        }

#pragma endregion ���b�Z�[�W�֌W
        //--------------------


#pragma region �X�V����
        //�L�[�{�[�h������-------------------------------------
#pragma region �L�[�{�[�h������

        //�L�[�{�[�h���̎擾�J�n-----------------
        devkeyboard->Acquire();
        //----------------------------

        //�S�L�[�̓��͏�Ԃ��擾����---------------------------
        KeyInit(key, oldkey, sizeof(key) / sizeof(key[0]));
        result = devkeyboard->GetDeviceState(sizeof(key), key);
        //----------------------------

#pragma endregion �L�[�{�[�h������
        //----------------------------------------

        if (key[DIK_SPACE])
        {
            clearColor[0] = {1.0f}; // ���ۂ��F
        }
        else
        {
            clearColor[0] = { 0.1f };
        }


#pragma endregion �X�V����

#pragma region �`�揈��
                //�o�b�N�o�b�t�@�̔ԍ����擾�i2�Ȃ̂�0�Ԃ�1�ԁj--------------------------
        UINT bbIndex = swapchain->GetCurrentBackBufferIndex();
        //-----------------------------------

        // �P�D���\�[�X�o���A�ŏ������݉\�ɕύX----
#pragma region �P�D���\�[�X�o���A�ŏ������݉\�ɕύX

        D3D12_RESOURCE_BARRIER barrierDesc{};
        barrierDesc.Transition.pResource = backBuffers[bbIndex].Get(); // �o�b�N�o�b�t�@���w��
        barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT; // �\������
        barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // �`��
        commandList->ResourceBarrier(1, &barrierDesc);

#pragma endregion 1�D���\�[�X�o���A�ŏ������݉\�ɕύX
        //--------------------------

        // �Q�D�`���w��----------------
#pragma region �Q�D�`���w��

// �����_�[�^�[�Q�b�g�r���[�p�f�B�X�N���v�^�q�[�v�̃n���h�����擾
        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
        rtvHandle.ptr += bbIndex * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
        commandList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);

#pragma endregion 2�D�`���w��
        //-------------------
        
        //�R�D��ʃN���A-------------
#pragma region 3.��ʃN���A

        
        commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
#pragma endregion 3.��ʃN���A
        //---------------------------

        //�`��R�}���h------------------
#pragma region �`��R�}���h
            //�r���[�|�[�g�̐ݒ�R�}���h-----------------------------
#pragma region �r���[�|�[�g�̐ݒ�R�}���h

        D3D12_VIEWPORT viewport[4]{};

        viewport[0].Width = 200;
        viewport[0].Height = 200;
        viewport[0].TopLeftX = 0;
        viewport[0].TopLeftY = 0;
        viewport[0].MinDepth = 0.0f;
        viewport[0].MaxDepth = 1.0f;

        viewport[1].Width = 200;
        viewport[1].Height = 200;
        viewport[1].TopLeftX = viewport[0].Width;
        viewport[1].TopLeftY = viewport[0].Height;
        viewport[1].MinDepth = 0.0f;
        viewport[1].MaxDepth = 1.0f;



        commandList->RSSetViewports(4, viewport);

#pragma endregion �r���[�|�[�g�̐ݒ�R�}���h
        //------------------------------

        //�V�U�[��`�̐ݒ�R�}���h-----------------
#pragma region �V�U�[��`�̐ݒ�R�}���h

        D3D12_RECT scissorrect{};

        scissorrect.left = 0;                                       // �؂蔲�����W��
        scissorrect.right = scissorrect.left + window_width;        // �؂蔲�����W�E
        scissorrect.top = 0;                                        // �؂蔲�����W��
        scissorrect.bottom = scissorrect.top + window_height;       // �؂蔲�����W��

        commandList->RSSetScissorRects(1, &scissorrect);

#pragma endregion �V�U�[��`�̐ݒ�R�}���h
        //------------------

        commandList->SetPipelineState(pipelinestate.Get());
        commandList->SetGraphicsRootSignature(rootsignature.Get());
        

        //�v���~�e�B�u�`��̐ݒ�R�}���h�i�O�p�`���X�g�j--------------------------
        commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        commandList->IASetVertexBuffers(0, 1, &vbView);

        //�`��R�}���h
        commandList->DrawInstanced(_countof(vertices), 1, 0, 0);

#pragma endregion �`��R�}���h
        //----------------------

        // �T�D���\�[�X�o���A��߂�--------------
#pragma region 5.���\�[�X�o���A��߂�

        barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // �`��
        barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;   // �\����
        commandList->ResourceBarrier(1, &barrierDesc);

#pragma endregion 5.���\�[�X�o���A��߂�
        //--------------------

        // ���߂̃N���[�Y-----------------------------------
        result = commandList->Close();
        assert(SUCCEEDED(result));
        //------------
        

        // �R�}���h���X�g�̎��s-------------------------------------
#pragma region �R�}���h���X�g���s
         ID3D12CommandList* commandLists[] = { commandList.Get() }; // �R�}���h���X�g�̔z��
        commandQueue->ExecuteCommandLists(1, commandLists);

        // �o�b�t�@���t���b�v�i���\�̓��ւ�)-----------------------
       result =  swapchain->Present(1, 0);
       assert(SUCCEEDED(result));
        //-----------------

#pragma region �R�}���h���s�����҂�
    // �R�}���h���X�g�̎��s������҂�
        commandQueue->Signal(fence.Get(), ++fenceVal);
        if (fence->GetCompletedValue() != fenceVal)
        {
            HANDLE event = CreateEvent(nullptr, false, false, nullptr);
            fence->SetEventOnCompletion(fenceVal, event);
            WaitForSingleObject(event, INFINITE);
            CloseHandle(event);
        }
#pragma endregion �R�}���h���s�����҂�

        //�L���[���N���A
        result = commandAllocator->Reset(); // �L���[���N���A
        assert(SUCCEEDED(result));

        //�ĂуR�}���h���X�g�����߂鏀��
        result = commandList->Reset(commandAllocator.Get(), nullptr);  // �ĂуR�}���h���X�g�𒙂߂鏀��
        assert(SUCCEEDED(result));

#pragma endregion �R�}���h���X�g���s
//------------------

#pragma endregion �`�揈��


    }



    UnregisterClass(w.lpszClassName, w.hInstance);

	return 0;
}
