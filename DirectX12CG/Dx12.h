#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <wrl.h>
#include <vector>
#include <string>
#include "DxWindow.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")



namespace MCB
{

    class Dx12
    {
    public:

        //基本変数初期化-----
    #pragma region 基本変数初期化

        HRESULT result;
        Microsoft::WRL::ComPtr<ID3D12Device> device = nullptr;
        Microsoft::WRL::ComPtr<IDXGIFactory6> dxgiFactory = nullptr;
        Microsoft::WRL::ComPtr<IDXGISwapChain4> swapchain = nullptr;
        Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator = nullptr;
        Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList = nullptr;
        Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue = nullptr;
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeaps = nullptr;

    #pragma endregion 基本変数初期化
        //--------------

        //デバイス変数-----------------------
        //対応レベルの配列
        D3D_FEATURE_LEVEL levels[4] =
        {
            D3D_FEATURE_LEVEL_12_1,
            D3D_FEATURE_LEVEL_12_0,
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
        };

        D3D_FEATURE_LEVEL featureLevel;
        //---------------------------

        //コマンドキュー
        D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};

        //スワップチェーン
        Microsoft::WRL::ComPtr<IDXGISwapChain1> swapchain1;

        //各種設定
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

        // デスクリプタ変数
        D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};

        //バックバッファ変数
        std::vector< Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers = std::vector< Microsoft::WRL::ComPtr<ID3D12Resource>>(2);

        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;

        //レンダーターゲットビューの設定
        D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};

        //フェンス変数
        Microsoft::WRL::ComPtr<ID3D12Fence> fence = nullptr;
        UINT64 fenceVal = 0;

        Dx12(DxWindow dxWindow);

        void SetDXFactory();

        void SetAdapter();

        void SetDevice(IDXGIAdapter4* tmpAdapter);

        void SetCommandListAndQueue();

        void SetSwapChain(DxWindow dxWindow);

        void SetDesctiptor();

        void SetBackBuffer();

        void SetFence();

    };

}
