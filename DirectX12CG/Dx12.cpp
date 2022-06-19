#include "Dx12.h"

using namespace MCB;
using namespace std;


Dx12::Dx12(DxWindow dxWindow)
{
    SetDXFactory();

    SetAdapter();

    SetCommandListAndQueue();

    SetSwapChain(dxWindow);

    SetDesctiptor();

    SetBackBuffer();

    SetFence();
}

void Dx12::SetAdapter()
{
    //アダプター設定ここから--------------------------------------------------------------------------------------------
#pragma region アダプター設定

    //アダプターの列挙用------------
    vector<IDXGIAdapter4*> adapters;

    //ここに特定の名前を持つアダプターオブジェクトが入る
    IDXGIAdapter4* tmpAdapter = nullptr;
    //---------------------

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

    SetDevice(tmpAdapter);
    
    tmpAdapter->Release();

#pragma endregion アダプター設定
    //アダプター設定ここまで-----------------------------------------------------------------------
}

void Dx12::SetDevice(IDXGIAdapter4* tmpAdapter)
{
    //デバイス生成ここから----------------------------------------------------------------------------------------------------------
#pragma region デバイス生成

    for (int i = 0; i < _countof(levels); i++)
    {
        //採用したアダプターでデバイスを生成
        result = D3D12CreateDevice(tmpAdapter, levels[i], IID_PPV_ARGS(&device));
        if (result == S_OK)
        {
            featureLevel = levels[i];
            break;
        }
    }

#pragma endregion デバイス生成
    //デバイス生成ここまで----------------------------------------------------------------------

}

void Dx12::SetCommandListAndQueue()
{
    //コマンドリストここから--------------------------------------------------------------------------------------------------------
#pragma region コマンドリスト

    //コマンドアロケータを生成
    result = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
    assert(SUCCEEDED(result));

    //コマンドリストを生成
    result = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList));
    assert(SUCCEEDED(result));


    //コマンドキューの生成

    result = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
    assert(SUCCEEDED(result));

#pragma endregion コマンドリスト
    //コマンドリストここまで------------------------------------------------------------------------------------------
}

void Dx12::SetDXFactory()
{
    //DXGIファクトリー生成----------------
#pragma region DXGIファクトリー生成

//DXGIファクトリーの生成
    result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
    assert(SUCCEEDED(result));

#pragma endregion DXGIファクトリー生成
    //-------------------
}

void Dx12::SetSwapChain(DxWindow dxWindow)
{
    //スワップチェーンの生成ここから------------------------------------------------------------------
#pragma region スワップチェーンの生成

    swapChainDesc.Width = dxWindow.window_width;
    swapChainDesc.Height = dxWindow.window_height;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
    swapChainDesc.BufferCount = 2;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    result = dxgiFactory->CreateSwapChainForHwnd(commandQueue.Get(), dxWindow.hwnd, &swapChainDesc, nullptr, nullptr, &swapchain1);

    assert(SUCCEEDED(result));

    result = swapchain1.As(&swapchain);
    assert(SUCCEEDED(result));



#pragma endregion スワップチェーンの生成
    //スワップチェーンの生成ここまで--------------------------------------------------------------
}

void Dx12::SetDesctiptor()
{
    //デスクリプタヒープを生成ここから--------------
#pragma region デスクリプタヒープ生成

//各種設定

    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.NumDescriptors = swapChainDesc.BufferCount;
    device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeaps));

#pragma endregion デスクリプタヒープ生成
    //デスクリプタヒープを生成ここまで------------

}

void Dx12::SetBackBuffer()
{
    //ダブルバッファ----------------------
#pragma region ダブルバッファ

    

    backBuffers.resize(swapChainDesc.BufferCount);

    for (size_t i = 0; i < backBuffers.size(); i++)
    {
        //スワップチェーンからバッファを取得
        swapchain->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers[i]));
        //でスクリプタヒープハンドルを取得
        rtvHandle = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
        //裏か表かでアドレスがずれる
        rtvHandle.ptr += i * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);

        //シェーダーの計算結果をSRGBに変換して書き込む
        rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
        rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
        //レンダーターゲットビューの生成
        device->CreateRenderTargetView(backBuffers[i].Get(), &rtvDesc, rtvHandle);

    }

#pragma endregion ダブルバッファ
    //------------------
}

void Dx12::SetFence()
{
    //フェンスの生成---------
#pragma region フェンスの生成
    result = device->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
#pragma endregion フェンスの生成
    //------------
}
