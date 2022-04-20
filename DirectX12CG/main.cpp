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

// ウィンドウプロシージャ 
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {  // メッセージに応じてゲーム固有の処理を行う 
	switch (msg) {
		// ウィンドウが破棄された 
	case WM_DESTROY:
		// OSに対して、アプリの終了を伝える 
		PostQuitMessage(0);
		return 0;
	}
	// 標準のメッセージ処理を行う 
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

//キーボード初期化-------------------------------
void KeyInit(BYTE* key, BYTE* oldkey, int ArraySize)
{
    for (int i = 0; i < ArraySize; i++)
    {
        oldkey[i] = key[i];
    }



}
//---------------------

//入力関数-----------------------------------
#pragma region 入力関数
//押している状態
bool IsKeyDown(int keyNum, BYTE* key)
{
    if (key[keyNum])
    {
        return true;
    }
    return false;
};
//押していない状態
bool IsKeyNDown(int keyNum, BYTE* key)
{
    if (!key[keyNum])
    {
        return true;
    }
    return false;
};
//長押し状態
bool IsKeyPress(int keyNum, BYTE* key, BYTE* oldkey)
{
    if (key[keyNum] && oldkey[keyNum])
    {
        return true;
    }
    return false;
};
//押した瞬間
bool IsKeyTrigger(int keyNum, BYTE* key, BYTE* oldkey)
{
    if (key[keyNum] && !oldkey[keyNum])
    {
        return true;
    }
    return false;
};
//離した瞬間
bool IsKeyRelease(int keyNum, BYTE* key, BYTE* oldkey)
{
    if (!key[keyNum] && oldkey[keyNum])
    {
        return true;
    }
    return false;
};
#pragma endregion 入力関数
//--------------------


//定数バッファ用構造体-----------------------------------
typedef struct ConstBufferDataMaterial
{
    XMFLOAT4 color;
    XMMATRIX mat;
};
//------------------------------------------


// Windowsアプリでのエントリーポイント(main関数) 
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) 
{  
	

  // ウィンドウの生成（前処理-----------------------
#pragma region ウィンドウ生成
    // ウィンドウサイズ
    const int window_width = 1280;  // 横幅
    const int window_height = 720;  // 縦幅

    WNDCLASSEX w{}; // ウィンドウクラスの設定
    w.cbSize = sizeof(WNDCLASSEX);
    w.lpfnWndProc = (WNDPROC)WindowProc; // ウィンドウプロシージャを設定
    w.lpszClassName = L"DirectXGame"; // ウィンドウクラス名
    w.hInstance = GetModuleHandle(nullptr); // ウィンドウハンドル
    w.hCursor = LoadCursor(NULL, IDC_ARROW); // カーソル指定

    // ウィンドウクラスをOSに登録
    RegisterClassEx(&w);
    // ウィンドウサイズ{ X座標 Y座標 横幅 縦幅 }
    RECT wrc = { 0, 0, window_width, window_height };
    AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false); // 自動でサイズ補正


    // ウィンドウオブジェクトの生成
    HWND hwnd = CreateWindow(w.lpszClassName, // クラス名
        L"DirectX12Game",         // タイトルバーの文字
        WS_OVERLAPPEDWINDOW,        // 標準的なウィンドウスタイル
        CW_USEDEFAULT,              // 表示X座標（OSに任せる）
        CW_USEDEFAULT,              // 表示Y座標（OSに任せる）
        wrc.right - wrc.left,       // ウィンドウ横幅
        wrc.bottom - wrc.top,   // ウィンドウ縦幅
        nullptr,                // 親ウィンドウハンドル
        nullptr,                // メニューハンドル
        w.hInstance,            // 呼び出しアプリケーションハンドル
        nullptr);               // オプション

    // ウィンドウ表示
    ShowWindow(hwnd, SW_SHOW);

#pragma endregion ウィンドウ生成
    //-------------

    MSG msg{}; //メッセージここから

#pragma region DirectX初期化


        //デバック時のみ----------
#pragma region デバック時のみ

#ifdef _DEBUG
//デバックレイヤーをオンに
    ComPtr<ID3D12Debug> debugController;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
    {
        debugController->EnableDebugLayer();
    }

#endif

#pragma endregion デバック時のみ
    //-------------


    //基本変数初期化-----
#pragma region 基本変数初期化

    HRESULT result;
    ComPtr<ID3D12Device> device = nullptr;
    ComPtr<IDXGIFactory6> dxgiFactory = nullptr;
    ComPtr<IDXGISwapChain4> swapchain = nullptr;
    ComPtr<ID3D12CommandAllocator> commandAllocator = nullptr;
    ComPtr<ID3D12GraphicsCommandList> commandList = nullptr;
    ComPtr<ID3D12CommandQueue> commandQueue = nullptr;
    ComPtr<ID3D12DescriptorHeap> rtvHeaps = nullptr;

#pragma endregion 基本変数初期化
    //--------------

       //DXGIファクトリー生成----------------
#pragma region DXGIファクトリー生成

//DXGIファクトリーの生成
    result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
    assert(SUCCEEDED(result));

#pragma endregion DXGIファクトリー生成
    //-------------------

    //アダプター設定ここから--------------------------------------------------------------------------------------------
#pragma region アダプター設定

    //アダプターの列挙用
    vector<ComPtr<IDXGIAdapter4>> adapters;
    //ここに特定の名前を持つアダプターオブジェクトが入る
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

#pragma endregion アダプター設定
    //アダプター設定ここまで-----------------------------------------------------------------------

    //デバイス生成ここから----------------------------------------------------------------------------------------------------------
#pragma region デバイス生成

    //対応レベルの配列
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
        //採用したアダプターでデバイスを生成
        result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(&device));
        if (result == S_OK)
        {
            featureLevel = levels[i];
            break;
        }
    }

#pragma endregion デバイス生成
    //デバイス生成ここまで----------------------------------------------------------------------

     //コマンドリストここから--------------------------------------------------------------------------------------------------------
#pragma region コマンドリスト

    //コマンドアロケータを生成
    result = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
    assert(SUCCEEDED(result));

    //コマンドリストを生成
    result = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList));
    assert(SUCCEEDED(result));


    //コマンドキューの生成
    //標準設定でコマンドキューを生成
    D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
    result = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
    assert(SUCCEEDED(result));

#pragma endregion コマンドリスト
    //コマンドリストここまで------------------------------------------------------------------------------------------


    //スワップチェーンの生成ここから------------------------------------------------------------------
#pragma region スワップチェーンの生成
    ComPtr<IDXGISwapChain1> swapchain1;

    //各種設定
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



#pragma endregion スワップチェーンの生成
    //スワップチェーンの生成ここまで--------------------------------------------------------------

     //レンダーターゲットビュー生成関係ここから---------------------------------------------------------------------------------------------------
#pragma region レンダーターゲットビュー生成関係

    //デスクリプタヒープを生成ここから--------------
#pragma region デスクリプタヒープ生成

//各種設定
     D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
     rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
     rtvHeapDesc.NumDescriptors = swapChainDesc.BufferCount;
     device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeaps));

#pragma endregion デスクリプタヒープ生成
     //デスクリプタヒープを生成ここまで------------

     //ダブルバッファ----------------------
#pragma region ダブルバッファ

     vector<ComPtr<ID3D12Resource>> backBuffers(2);

     backBuffers.resize(swapChainDesc.BufferCount);

     for (size_t i = 0; i < backBuffers.size(); i++)
     {
         //スワップチェーンからバッファを取得
         swapchain->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers[i]));
         //でスクリプタヒープハンドルを取得
         D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
         //裏か表かでアドレスがずれる
         rtvHandle.ptr += i * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
         //レンダーターゲットビューの設定
         D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
         //シェーダーの計算結果をSRGBに変換して書き込む
         rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
         rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
         //レンダーターゲットビューの生成
         device->CreateRenderTargetView(backBuffers[i].Get(), &rtvDesc, rtvHandle);

     }

#pragma endregion ダブルバッファ
     //------------------

#pragma endregion レンダーターゲットビュー生成関係



         //フェンスの生成---------
#pragma region フェンスの生成

     ComPtr<ID3D12Fence> fence = nullptr;
     UINT64 fenceVal = 0;

     result = device->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

#pragma endregion フェンスの生成
     //------------

         //入力系初期化--------------
#pragma region 入力系初期化

     IDirectInput8* dinput = nullptr;
     result = DirectInput8Create(w.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);
     assert(SUCCEEDED(result));
     //キーボードデバイスの生成-----------------
#pragma region キーボードデバイスの生成

     IDirectInputDevice8* devkeyboard = nullptr;
     result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
     assert(SUCCEEDED(result));

#pragma endregion キーボードデバイスの生成
     //--------------------------

     //入力データ形式セット--------------------------------
     result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);
     assert(SUCCEEDED(result));
     //---------------------------------

     //排他レベル制御-------------------------------------------------------------
     result = devkeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
     assert(SUCCEEDED(result));

     //---------------------------------------------------

     //キー初期化----------------------
     BYTE key[256] = {};
     BYTE oldkey[256] = {};
     //-----------------------

#pragma endregion 入力系初期化
//----------------

#pragma endregion 


#pragma region 描画初期化処理


#pragma region 定数バッファの生成

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


     result = device->CreateCommittedResource
     (
         &cdHeapProp,        //ヒープ設定
         D3D12_HEAP_FLAG_NONE,
         &cdResdesc,//リソース設定
         D3D12_RESOURCE_STATE_GENERIC_READ,
         nullptr,
         IID_PPV_ARGS(&constBuffMaterial)
     );
     assert(SUCCEEDED(result));


     ConstBufferDataMaterial* constMapMaterial = nullptr;

     result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);

     assert(SUCCEEDED(result));

     constMapMaterial->color = XMFLOAT4(1, 0, 0, 0.5f);

#pragma endregion

     //ルートパラメータの設定---------------------------
#pragma region ルートパラメータの設定

     D3D12_ROOT_PARAMETER rootparam = {};
     rootparam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
     rootparam.Descriptor.ShaderRegister = 0;
     rootparam.Descriptor.RegisterSpace = 0;
     rootparam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

#pragma endregion ルートパラメータの設定
     //------------------------




     //頂点データ---------------------------------
#pragma region 頂点データ
     XMFLOAT3 vertices[] = {
         {-0.5f,-0.5f,0.0f},
         {-0.5f,+0.5f,0.0f},
         {+ 0.5f,-0.5f,0.0f},
     };

     UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * _countof(vertices));
#pragma endregion 頂点データ
     //--------------------------
     
     //頂点バッファ---------------
#pragma region 頂点バッファの設定
    D3D12_HEAP_PROPERTIES heapprop{};   // ヒープ設定
    cdHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用

    D3D12_RESOURCE_DESC resdesc{};  // リソース設定
    cdResdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    cdResdesc.Width = sizeVB; // 頂点データ全体のサイズ
    cdResdesc.Height = 1;
    cdResdesc.DepthOrArraySize = 1;
    cdResdesc.MipLevels = 1;
    cdResdesc.SampleDesc.Count = 1;
    cdResdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
#pragma endregion 頂点バッファの設定
     //----------------------------------

     // 頂点バッファの生成----------------------------
#pragma region 頂点バッファの生成
     ComPtr<ID3D12Resource> vertBuff = nullptr;
     result = device->CreateCommittedResource(
         &cdHeapProp, // ヒープ設定
         D3D12_HEAP_FLAG_NONE,
         &cdResdesc, // リソース設定
         D3D12_RESOURCE_STATE_GENERIC_READ,
         nullptr,
         IID_PPV_ARGS(&vertBuff));

     assert(SUCCEEDED(result));
#pragma endregion 頂点バッファの生成
     //-------------------------

     // 頂点バッファへのデータ転送------------
#pragma region GPU上のバッファに対応した仮想メモリを取得
     XMFLOAT3* vertMap = nullptr;
     result = vertBuff->Map(0, nullptr, (void**)&vertMap);
     assert(SUCCEEDED(result));

     // 全頂点に対して
     for (int i = 0; i < _countof(vertices); i++)
     {
         vertMap[i] = vertices[i];   // 座標をコピー
     }

     // マップを解除
     vertBuff->Unmap(0, nullptr);
#pragma endregion GPU上のバッファに対応した仮想メモリを取得
     //--------------------------------------

     // 頂点バッファビューの作成--------------------------
#pragma region 頂点バッファビューの作成
     D3D12_VERTEX_BUFFER_VIEW vbView{};

     vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
     vbView.SizeInBytes = sizeVB;
     vbView.StrideInBytes = sizeof(XMFLOAT3);
#pragma endregion 頂点バッファビューの作成
     //-----------------------------------

       //シェーダーオブジェクト宣言-------------------------------------------
#pragma region シェーダーオブジェクト宣言
     ComPtr<ID3DBlob> vsBlob = nullptr; // 頂点シェーダオブジェクト
     ComPtr<ID3DBlob> psBlob = nullptr; // ピクセルシェーダオブジェクト
     ComPtr<ID3DBlob> errorBlob = nullptr; // エラーオブジェクト
#pragma endregion シェーダーオブジェクト宣言
//---------------------------------

// 頂点シェーダの読み込みとコンパイル--------------------------------
#pragma region 頂点シェーダの読み込みとコンパイル

     result = D3DCompileFromFile(
         L"BasicVS.hlsl",  // シェーダファイル名
         nullptr,
         D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
         "main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
         D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
         0,
         &vsBlob, &errorBlob);

#pragma endregion 頂点シェーダの読み込みとコンパイル
     //------------------------------------------


     //  シェーダーのエラーに関する出力部分-----------------
#pragma region シェーダーのエラーに関する出力部分

     if (FAILED(result)) {
         // errorBlobからエラー内容をstring型にコピー
         string error;
         error.resize(errorBlob->GetBufferSize());

         copy_n((char*)errorBlob->GetBufferPointer(),
             errorBlob->GetBufferSize(),
             error.begin());
         error += "\n";
         // エラー内容を出力ウィンドウに表示
         OutputDebugStringA(error.c_str());
         assert(0);
     }

#pragma endregion シェーダーのエラーに関する出力部分
     //-----------------------------------

     // ピクセルシェーダの読み込みとコンパイル-------------------------------
#pragma region ピクセルシェーダの読み込みとコンパイル

     result = D3DCompileFromFile(
         L"BasicPS.hlsl",   // シェーダファイル名
         nullptr,
         D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
         "main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
         D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
         0,
         &psBlob, &errorBlob);

#pragma endregion ピクセルシェーダの読み込みとコンパイル
     //--------------------------------

     //  シェーダーのエラーに関する出力部分-----------------
#pragma region シェーダーのエラーに関する出力部分

     if (FAILED(result)) {
         // errorBlobからエラー内容をstring型にコピー
         string error;
         error.resize(errorBlob->GetBufferSize());

         copy_n((char*)errorBlob->GetBufferPointer(),
             errorBlob->GetBufferSize(),
             error.begin());
         error += "\n";
         // エラー内容を出力ウィンドウに表示
         OutputDebugStringA(error.c_str());
         assert(0);
     }

#pragma endregion シェーダーのエラーに関する出力部分
     //-----------------------------------


      // 頂点レイアウト------------------
#pragma region 頂点レイアウト

     D3D12_INPUT_ELEMENT_DESC inputLayout[] =
     {
         {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}, // (1行で書いたほうが見やすい)
         //{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},//法線ベクトル
         //{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
     };

#pragma endregion 頂点レイアウト
     //--------------------

     // グラフィックスパイプライン設定-----------
     D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipelineDesc{};
     //-------------------------

     //頂点シェーダ、ピクセルシェーダをパイプラインに設定-----------------------------
#pragma region 頂点シェーダとピクセルシェーダをパイプラインに設定

     gpipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
     gpipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
     gpipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
     gpipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

#pragma endregion 頂点シェーダとピクセルシェーダをパイプラインに設定
     //-----------------------------------

     //サンプルマスクとラスタライザステートの設定------------------------------------
#pragma region サンプルマスクとラスタライザステートの設定

     gpipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
     gpipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;  // 背面カリング
     gpipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
     gpipelineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

#pragma endregion サンプルマスクとラスタライザステートの設定
//------------------------------------


      //ブレンドステートの設定-------------------------------
#pragma region ブレンドステートの設定
//gpipeline.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;  // RBGA全てのチャンネルを描画
     D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = gpipelineDesc.BlendState.RenderTarget[0];
     blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//標準設定

     //共通設定
     blenddesc.BlendEnable = true;
     blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
     blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
     blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

     //半透明合成
     blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
     blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
     blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

     //加算合成
     //#pragma region 加算合成
     //blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
     //blenddesc.SrcBlend = D3D12_BLEND_ONE;
     //blenddesc.DestBlend = D3D12_BLEND_ONE;
     //#pragma endregion

     //減算合成
   /*  #pragma region 減算合成
     blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
     blenddesc.SrcBlend = D3D12_BLEND_ONE;
     blenddesc.DestBlend = D3D12_BLEND_ONE;
     #pragma endregion*/

     ////色反転
     //blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
     //blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
     //blenddesc.DestBlend = D3D12_BLEND_ZERO;


#pragma endregion ブレンドステートの設定
     //--------------------------

     //頂点レイアウトの設定------------------
#pragma region 頂点レイアウトの設定

     gpipelineDesc.InputLayout.pInputElementDescs = inputLayout;
     gpipelineDesc.InputLayout.NumElements = _countof(inputLayout);

#pragma endregion 頂点レイアウトの設定
     //----------------------------

     //図形の形状を三角形に設定-------------------------
     gpipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
     //------------------

     //その他の設定----------------
#pragma region その他の設定

     gpipelineDesc.NumRenderTargets = 1; // 描画対象は1つ
     gpipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
     gpipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

#pragma endregion その他の設定
//----------------

     //ルートシグネチャの生成--------------------------
#pragma region ルートシグネチャの生成

     ComPtr <ID3D12RootSignature> rootsignature;

     D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
     rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
     rootSignatureDesc.pParameters = &rootparam; //ルートパラメータの先頭アドレス
     rootSignatureDesc.NumParameters = 1; //ルートパラメータ数
     //rootSignatureDesc.pStaticSamplers = &samplerDesc;
     //rootSignatureDesc.NumStaticSamplers = 1;


     ComPtr<ID3DBlob> rootSigBlob = nullptr;
     result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
     result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));

     assert(SUCCEEDED(result));

     // パイプラインにルートシグネチャをセット
     gpipelineDesc.pRootSignature = rootsignature.Get();

#pragma endregion ルートシグネチャの生成
     //--------------------------------

    //パイプラインステートの生成-------------------------
#pragma region パイプラインステートの生成

     ComPtr<ID3D12PipelineState> pipelinestate = nullptr;
     result = device->CreateGraphicsPipelineState(&gpipelineDesc, IID_PPV_ARGS(&pipelinestate));
     assert(SUCCEEDED(result));
#pragma endregion パイプラインステートの生成
     //-----------------------------

     float clearColor[] = { 0.1f,0.25f, 0.5f,0.0f }; // 青っぽい色

     XMFLOAT4 objColor = { 1, 0, 0, 0.5f };
#pragma endregion
    while (true)
    {

        //メッセージ関係------------------
#pragma region メッセージ関係

// メッセージがある？
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) 
        {
            TranslateMessage(&msg); // キー入力メッセージの処理
            DispatchMessage(&msg); // プロシージャにメッセージを送る
        }

        // xボタンで終了メッセージが来たらゲームループを抜ける
        if (msg.message == WM_QUIT || key[DIK_ESCAPE])
        {
            break;
        }

#pragma endregion メッセージ関係
        //--------------------


#pragma region 更新処理
        //キーボード初期化-------------------------------------
#pragma region キーボード初期化

        //キーボード情報の取得開始-----------------
        devkeyboard->Acquire();
        //----------------------------

        //全キーの入力状態を取得する---------------------------
        KeyInit(key, oldkey, sizeof(key) / sizeof(key[0]));
        result = devkeyboard->GetDeviceState(sizeof(key), key);
        //----------------------------

#pragma endregion キーボード初期化
        //----------------------------------------


        //result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);

        //assert(SUCCEEDED(result));

        if (objColor.y <= 1)
        {
            objColor.y += 0.01f;
        }

        constMapMaterial->color = objColor;


#pragma endregion 更新処理

#pragma region 描画処理
                //バックバッファの番号を取得（2つなので0番か1番）--------------------------
        UINT bbIndex = swapchain->GetCurrentBackBufferIndex();
        //-----------------------------------

        // １．リソースバリアで書き込み可能に変更----
#pragma region １．リソースバリアで書き込み可能に変更

        D3D12_RESOURCE_BARRIER barrierDesc{};
        barrierDesc.Transition.pResource = backBuffers[bbIndex].Get(); // バックバッファを指定
        barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT; // 表示から
        barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画
        commandList->ResourceBarrier(1, &barrierDesc);

#pragma endregion 1．リソースバリアで書き込み可能に変更
        //--------------------------

        // ２．描画先指定----------------
#pragma region ２．描画先指定

// レンダーターゲットビュー用ディスクリプタヒープのハンドルを取得
        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
        rtvHandle.ptr += bbIndex * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
        commandList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);

#pragma endregion 2．描画先指定
        //-------------------
        
        //３．画面クリア-------------
#pragma region 3.画面クリア

        
        commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
#pragma endregion 3.画面クリア
        //---------------------------

        //描画コマンド------------------
#pragma region 描画コマンド
            //ビューポートの設定コマンド-----------------------------
#pragma region ビューポートの設定コマンド

        D3D12_VIEWPORT viewport{};

        viewport.Width = window_width;
        viewport.Height = window_height;
        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;

        commandList->RSSetViewports(1, &viewport);

#pragma endregion ビューポートの設定コマンド
        //------------------------------

        //シザー矩形の設定コマンド-----------------
#pragma region シザー矩形の設定コマンド

        D3D12_RECT scissorrect{};

        scissorrect.left = 0;                                       // 切り抜き座標左
        scissorrect.right = scissorrect.left + window_width;        // 切り抜き座標右
        scissorrect.top = 0;                                        // 切り抜き座標上
        scissorrect.bottom = scissorrect.top + window_height;       // 切り抜き座標下

        commandList->RSSetScissorRects(1, &scissorrect);

#pragma endregion シザー矩形の設定コマンド
        //------------------

        commandList->SetPipelineState(pipelinestate.Get());
        commandList->SetGraphicsRootSignature(rootsignature.Get());
        

        //プリミティブ形状の設定コマンド（三角形リスト）--------------------------
        commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        commandList->IASetVertexBuffers(0, 1, &vbView);



        
        //定数バッファビュー(CBV)の設定コマンド
        commandList->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());

        //描画コマンド
        commandList->DrawInstanced(_countof(vertices), 1, 0, 0);

#pragma endregion 描画コマンド
        //----------------------

        // ５．リソースバリアを戻す--------------
#pragma region 5.リソースバリアを戻す

        barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画
        barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;   // 表示に
        commandList->ResourceBarrier(1, &barrierDesc);

#pragma endregion 5.リソースバリアを戻す
        //--------------------

        // 命令のクローズ-----------------------------------
        result = commandList->Close();
        assert(SUCCEEDED(result));
        //------------
        

        // コマンドリストの実行-------------------------------------
#pragma region コマンドリスト実行
         ID3D12CommandList* commandLists[] = { commandList.Get() }; // コマンドリストの配列
        commandQueue->ExecuteCommandLists(1, commandLists);

        // バッファをフリップ（裏表の入替え)-----------------------
       result =  swapchain->Present(1, 0);
       assert(SUCCEEDED(result));
        //-----------------

#pragma region コマンド実行完了待ち
    // コマンドリストの実行完了を待つ
        commandQueue->Signal(fence.Get(), ++fenceVal);
        if (fence->GetCompletedValue() != fenceVal)
        {
            HANDLE event = CreateEvent(nullptr, false, false, nullptr);
            fence->SetEventOnCompletion(fenceVal, event);
            WaitForSingleObject(event, INFINITE);
            CloseHandle(event);
        }
#pragma endregion コマンド実行完了待ち

        //キューをクリア
        result = commandAllocator->Reset(); // キューをクリア
        assert(SUCCEEDED(result));

        //再びコマンドリストをためる準備
        result = commandList->Reset(commandAllocator.Get(), nullptr);  // 再びコマンドリストを貯める準備
        assert(SUCCEEDED(result));

#pragma endregion コマンドリスト実行
//------------------

#pragma endregion 描画処理


    }



    UnregisterClass(w.lpszClassName, w.hInstance);

	return 0;
}
