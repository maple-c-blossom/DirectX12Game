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
        key[i] = 0;
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
    ComPtr<ID3D12GraphicsCommandList> cmdList = nullptr;
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
    result = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&cmdList));
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
        if (msg.message == WM_QUIT) 
        {
            break;
        }

#pragma endregion ���b�Z�[�W�֌W
        //--------------------
    }



    UnregisterClass(w.lpszClassName, w.hInstance);

	return 0;
}
