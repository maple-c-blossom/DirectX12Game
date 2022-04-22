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
#include <time.h>

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





//定数バッファ用構造体-----------------------------------
typedef struct ConstBufferDataMaterial
{
    XMFLOAT4 color;
    XMMATRIX mat;
};
//------------------------------------------

//頂点データ構造体-------------------------------------
typedef struct Vertex
{
    XMFLOAT3 pos;//xyz座標
    //XMFLOAT3 normal;//法線ベクトル
    XMFLOAT2 uv;//uv座標
};
//--------------------------------------


// Windowsアプリでのエントリーポイント(main関数) 
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) 
{  

    DxWindow dxWindow;

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

    Dx12 dx(dxWindow);

    Input input(dx.result,dxWindow.w,dxWindow.hwnd);

#pragma endregion 


#pragma region 描画初期化処理

    //定数バッファの生成-------------------
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


     dx.result = dx.device->CreateCommittedResource
     (
         &cdHeapProp,        //ヒープ設定
         D3D12_HEAP_FLAG_NONE,
         &cdResdesc,//リソース設定
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
     //----------------------
     
     //画像イメージデータの作成----------------------
#pragma region 画像イメージデータの作成
     //横方向ピクセル数
     const size_t textureWidth = 256;
     //縦方向ピクセル数
     const size_t textureHeight = 256;
     //配列の要素数
     const size_t imageDataCount = textureWidth * textureHeight;
     //画像イメージデータの配列
     XMFLOAT4* imageData = new XMFLOAT4[imageDataCount];

     srand(time(nullptr));
     for (int i = 0; i < imageDataCount; i++)
     {
         imageData[i].x = (float)(rand() % 255) / 255 ;//R
         imageData[i].y = (float)(rand() % 255) / 255;//G
         imageData[i].z = (float)(rand() % 255) / 255;//B
         imageData[i].w = 1.0f;//A
         
     }

#pragma endregion 画像イメージデータの作成
     //------------------------------------

#pragma region テクスチャバッファ設定

     D3D12_HEAP_PROPERTIES texHeapProp{};

     texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
     texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
     texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

     D3D12_RESOURCE_DESC texresDesc{};
     texresDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
     texresDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
     texresDesc.Width = textureWidth;
     texresDesc.Height = textureHeight;
     texresDesc.DepthOrArraySize = 1;
     texresDesc.MipLevels = 1;
     texresDesc.SampleDesc.Count = 1;


#pragma endregion テクスチャバッファ設定

#pragma region テクスチャバッファの生成
     ComPtr<ID3D12Resource> texbuff = nullptr;
     dx.result = dx.device->CreateCommittedResource(&texHeapProp, D3D12_HEAP_FLAG_NONE, &texresDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&texbuff));
#pragma endregion テクスチャバッファの生成

     dx.result = texbuff->WriteToSubresource(0, nullptr, imageData, sizeof(XMFLOAT4) * textureWidth, sizeof(XMFLOAT4) * imageDataCount);

         //デスクリプタヒープの生成-------------------------
#pragma region デスクリプタヒープの生成

     const size_t kMaxSRVCount = 2056;

    //定数バッファ用のデスクリプタヒープ

     //設定構造体
     D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc{};
     srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
     srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; //シェーダーから見えるように
     srvHeapDesc.NumDescriptors = kMaxSRVCount;//定数バッファの数

     //デスクリプタヒープの生成  
     ComPtr<ID3D12DescriptorHeap> srvHeap = nullptr;
     dx.result = dx.device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));

     D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();
#pragma endregion デスクリプタヒープの生成
    //-------------------------------

     //シェーダーリソースビューの作成------------------------------
#pragma region シェーダーリソースビューの作成
     //シェーダーリソースビュー設定
     D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};

     srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
     srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
     srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
     srvDesc.Texture2D.MipLevels = 1;

     //ヒープの二番目にシェーダーリソースビュー作成
     dx.device->CreateShaderResourceView(texbuff.Get(), &srvDesc, srvHandle);

#pragma endregion シェーダーリソースビューの作成
     //----------------------------

     //デスクリプタレンジの設定--------------------------------
#pragma region デスクリプタレンジの設定
     D3D12_DESCRIPTOR_RANGE descriptorRange{};
     descriptorRange.NumDescriptors = 1;
     descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
     descriptorRange.BaseShaderRegister = 0;
     descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
#pragma endregion デスクリプタレンジの設定
     //-----------------------------------------

     //ルートパラメータの設定---------------------------
#pragma region ルートパラメータの設定

     D3D12_ROOT_PARAMETER rootparams[2] = {};
     //定数バッファ0番
     rootparams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
     rootparams[0].Descriptor.ShaderRegister = 0;
     rootparams[0].Descriptor.RegisterSpace = 0;
     rootparams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
     //テクスチャレジスタ0番
     rootparams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
     rootparams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;
     rootparams[1].DescriptorTable.NumDescriptorRanges = 1;
     rootparams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

#pragma endregion ルートパラメータの設定
     //------------------------

     //頂点データ---------------------------------
#pragma region 頂点データ
     Vertex vertices[] = {
         {{-0.4f,-0.7f,0.0f}, {0.0f,1.0f}},//左下
         {{-0.4f,+0.7f,0.0f}, {0.0f,0.0f}},//左上
         {{+0.4f,-0.7f,0.0f}, {1.0f,1.0f}},//右下
         {{+0.4f,+0.7f,0.0f}, {1.0f,0.0f}},//右上
     };

     UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));
#pragma endregion 頂点データ
     //--------------------------
     
     //頂点インデックス---------------------------
#pragma region 頂点インデックス
     uint16_t indices[] =
     {
         0,1,2,
         1,2,3,
     };
#pragma endregion 頂点インデックス
     //--------------------------

     //インデックスバッファの設定-------------------------
#pragma region インデックスの設定
     //インデックスデータ全体のサイズ
     UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));

     cdResdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
     cdResdesc.Width = sizeIB;
     cdResdesc.Height = 1;
     cdResdesc.DepthOrArraySize = 1;
     cdResdesc.MipLevels = 1;
     cdResdesc.SampleDesc.Count = 1;
     cdResdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

#pragma endregion インデックスの設定
     //------------------------

#pragma region インデックスバッファ生成

     ComPtr<ID3D12Resource> indexBuff = nullptr;
         //インデックスバッファの生成-----------------------------
     dx.result = dx.device->CreateCommittedResource(
         &cdHeapProp,
         D3D12_HEAP_FLAG_NONE,
         &cdResdesc,
         D3D12_RESOURCE_STATE_GENERIC_READ,
         nullptr,
         IID_PPV_ARGS(&indexBuff)
     );

#pragma endregion インデックスバッファ生成

     //インデックスバッファへのデータ転送------------------------------
#pragma region インデックスバッファへのデータ転送

     //GPU上のバッファに対応した仮想メモリを取得----------------------------
     uint16_t* indexMap = nullptr;
     dx.result = indexBuff->Map(0, nullptr, (void**)&indexMap);
     //---------------------------------------

     //全インデックスに対して-------------------------
     for (int i = 0; i < _countof(indices); i++)
     {
         indexMap[i] = indices[i];
     }
     //-----------------------

     //繋がりを解除---------------------
     indexBuff->Unmap(0, nullptr);
     //------------------------

#pragma endregion インデックスバッファへのデータ転送
    //-------------------------------------

     //インデックスバッファビューの作成-----------------------------------
#pragma region インデックスバッファビューの作成
     D3D12_INDEX_BUFFER_VIEW ibView{};
     ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
     ibView.Format = DXGI_FORMAT_R16_UINT;
     ibView.SizeInBytes = sizeIB;
#pragma endregion インデックスバッファビューの作成
     //------------------------------------------

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
     dx.result = dx.device->CreateCommittedResource(
         &cdHeapProp, // ヒープ設定
         D3D12_HEAP_FLAG_NONE,
         &cdResdesc, // リソース設定
         D3D12_RESOURCE_STATE_GENERIC_READ,
         nullptr,
         IID_PPV_ARGS(&vertBuff));

     assert(SUCCEEDED(dx.result));
#pragma endregion 頂点バッファの生成
     //-------------------------

     // 頂点バッファへのデータ転送------------
#pragma region GPU上のバッファに対応した仮想メモリを取得
     Vertex* vertMap = nullptr;
     dx.result = vertBuff->Map(0, nullptr, (void**)&vertMap);
     assert(SUCCEEDED(dx.result));

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
     vbView.StrideInBytes = sizeof(vertices[0]);
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

     dx.result = D3DCompileFromFile(
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

     if (FAILED(dx.result)) {
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

     dx.result = D3DCompileFromFile(
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

     if (FAILED(dx.result)) {
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
         {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}//uv座標
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


//      //テクスチャサンプラーの設定-----------------------
#pragma region テクスチャサンプラーの設定

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

#pragma endregion テクスチャサンプラーの設定
   //----------------------------------

     //ルートシグネチャの生成--------------------------
#pragma region ルートシグネチャの生成

     ComPtr <ID3D12RootSignature> rootsignature;

     D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
     rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
     rootSignatureDesc.pParameters = rootparams; //ルートパラメータの先頭アドレス
     rootSignatureDesc.NumParameters = _countof(rootparams); //ルートパラメータ数
     rootSignatureDesc.pStaticSamplers = &samplerDesc;
     rootSignatureDesc.NumStaticSamplers = 1;


     ComPtr<ID3DBlob> rootSigBlob = nullptr;
     dx.result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
     dx.result = dx.device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));

     assert(SUCCEEDED(dx.result));

     // パイプラインにルートシグネチャをセット
     gpipelineDesc.pRootSignature = rootsignature.Get();

#pragma endregion ルートシグネチャの生成
     //--------------------------------

    //パイプラインステートの生成-------------------------
#pragma region パイプラインステートの生成

     ComPtr<ID3D12PipelineState> pipelinestate = nullptr;
     dx.result = dx.device->CreateGraphicsPipelineState(&gpipelineDesc, IID_PPV_ARGS(&pipelinestate));
     assert(SUCCEEDED(dx.result));
#pragma endregion パイプラインステートの生成
     //-----------------------------

     float clearColor[] = { 0.1f,0.25f, 0.5f,0.0f }; // 青っぽい色

#pragma endregion
    while (true)
    {
        input.UpDateInit(dx.result);

        dxWindow.messageUpdate();

        if (input.IsKeyDown(DIK_ESCAPE) || dxWindow.breakFlag)
        {
            break;
        }

#pragma region 更新処理


#pragma endregion 更新処理

#pragma region 描画処理
        //バックバッファの番号を取得（2つなので0番か1番）--------------------------
        UINT bbIndex = dx.swapchain->GetCurrentBackBufferIndex();
        //-----------------------------------

        // １．リソースバリアで書き込み可能に変更----
#pragma region １．リソースバリアで書き込み可能に変更

        D3D12_RESOURCE_BARRIER barrierDesc{};
        barrierDesc.Transition.pResource = dx.backBuffers[bbIndex].Get(); // バックバッファを指定
        barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT; // 表示から
        barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画
        dx.commandList->ResourceBarrier(1, &barrierDesc);

#pragma endregion 1．リソースバリアで書き込み可能に変更
        //--------------------------

        // ２．描画先指定----------------
#pragma region ２．描画先指定

// レンダーターゲットビュー用ディスクリプタヒープのハンドルを取得
        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = dx.rtvHeaps->GetCPUDescriptorHandleForHeapStart();
        rtvHandle.ptr += bbIndex * dx.device->GetDescriptorHandleIncrementSize(dx.rtvHeapDesc.Type);
        dx.commandList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);
        dx.commandList->IASetIndexBuffer(&ibView);

#pragma endregion 2．描画先指定
        //-------------------
        
        //３．画面クリア-------------
#pragma region 3.画面クリア
        dx.commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
#pragma endregion 3.画面クリア
        //---------------------------

        //描画コマンド------------------
#pragma region 描画コマンド
            //ビューポートの設定コマンド-----------------------------
#pragma region ビューポートの設定コマンド

        D3D12_VIEWPORT viewport{};

        viewport.Width = dxWindow.window_width;
        viewport.Height = dxWindow.window_height;
        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;

        dx.commandList->RSSetViewports(1, &viewport);

#pragma endregion ビューポートの設定コマンド
        //------------------------------

        //シザー矩形の設定コマンド-----------------
#pragma region シザー矩形の設定コマンド

        D3D12_RECT scissorrect{};

        scissorrect.left = 0;                                       // 切り抜き座標左
        scissorrect.right = scissorrect.left + dxWindow.window_width;        // 切り抜き座標右
        scissorrect.top = 0;                                        // 切り抜き座標上
        scissorrect.bottom = scissorrect.top + dxWindow.window_height;       // 切り抜き座標下

        dx.commandList->RSSetScissorRects(1, &scissorrect);

#pragma endregion シザー矩形の設定コマンド
        //------------------

        dx.commandList->SetPipelineState(pipelinestate.Get());
        dx.commandList->SetGraphicsRootSignature(rootsignature.Get());
        

        //プリミティブ形状の設定コマンド（三角形リスト）--------------------------
        dx.commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        dx.commandList->IASetVertexBuffers(0, 1, &vbView);

        
        //定数バッファビュー(CBV)の設定コマンド
        dx.commandList->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());

        //SRVヒープの設定コマンド
        dx.commandList->SetDescriptorHeaps(1, srvHeap.GetAddressOf());

        //SRVヒープの先頭アドレスを取得
        D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();

        //SRVヒープの先頭にあるSRVをパラメータ1番に設定
        dx.commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

        //描画コマンド
        dx.commandList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);

#pragma endregion 描画コマンド
        //----------------------

        // ５．リソースバリアを戻す--------------
#pragma region 5.リソースバリアを戻す

        barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画
        barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;   // 表示に
        dx.commandList->ResourceBarrier(1, &barrierDesc);

#pragma endregion 5.リソースバリアを戻す
        //--------------------

        // 命令のクローズ-----------------------------------
        dx.result = dx.commandList->Close();
        assert(SUCCEEDED(dx.result));
        //------------
        

        // コマンドリストの実行-------------------------------------
#pragma region コマンドリスト実行
        ID3D12CommandList* commandLists[] = { dx.commandList.Get() }; // コマンドリストの配列
        dx.commandQueue->ExecuteCommandLists(1, commandLists);

        // バッファをフリップ（裏表の入替え)-----------------------
       dx.result =  dx.swapchain->Present(1, 0);
       assert(SUCCEEDED(dx.result));
        //-----------------

#pragma region コマンド実行完了待ち
    // コマンドリストの実行完了を待つ
        dx.commandQueue->Signal(dx.fence.Get(), ++dx.fenceVal);
        if (dx.fence->GetCompletedValue() != dx.fenceVal)
        {
            HANDLE event = CreateEvent(nullptr, false, false, nullptr);
            dx.fence->SetEventOnCompletion(dx.fenceVal, event);
            WaitForSingleObject(event, INFINITE);
            CloseHandle(event);
        }
#pragma endregion コマンド実行完了待ち

        //キューをクリア
        dx.result = dx.commandAllocator->Reset(); // キューをクリア
        assert(SUCCEEDED(dx.result));

        //再びコマンドリストをためる準備
        dx.result = dx.commandList->Reset(dx.commandAllocator.Get(), nullptr);  // 再びコマンドリストを貯める準備
        assert(SUCCEEDED(dx.result));

#pragma endregion コマンドリスト実行
//------------------

#pragma endregion 描画処理


    }

	return 0;
}
