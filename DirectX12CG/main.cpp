#pragma region 標準.h include

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

#pragma endregion 標準.h include

#pragma region 自作.h include

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

#pragma endregion 自作.h include

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

    //DirectXクラス生成
    Dx12 dx(dxWindow);
    //inputクラス生成
    Input input(dx.result,dxWindow.window,dxWindow.hwnd);

#pragma endregion 


#pragma region 描画初期化処理

    //深度バッファ----
    Depth depth(dxWindow, dx);
    //-------

    //3Dオブジェクトマテリアルの生成-------------------
    ObjectMaterial objMaterial;
    objMaterial.Init(dx);
    //---------------------

    //3Dオブジェクトの生成-------------------
#pragma region 3Dオブジェクトの生成

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

#pragma endregion 3Dオブジェクトの生成
    //----------------------

     //行列-----------------------
#pragma region 行列
        //ビュー変換行列
        View matView;
        matView.CreateMatrixView(XMFLOAT3(0.0f, 0.0f, -100.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));
        //射影変換行列
        Projection matProjection;
         matProjection.CreateMatrixProjection(XMConvertToRadians(45.0f),(float)dxWindow.window_width / dxWindow.window_height, 0.1f, 1000.0f);
#pragma endregion 行列
     //---------------------

#pragma region 画像関係
     //画像ファイル--------------------
     TextureFile textureFile;
     dx.result = textureFile.LoadTexture(L"Resources\\tori.png", WIC_FLAGS_NONE);
     //----------------------------

     //ミップマップの生成-------------------------
     MipMap mipmap;
     dx.result = mipmap.GenerateMipMap(&textureFile, TEX_FILTER_DEFAULT, 0);
     //----------------------------

     //画像イメージデータの作成----------------------
     TexImgData imageData;
     imageData.SetImageDataRGBA(XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
     //------------------------------------

      //テクスチャバッファ設定---------------------------------------
      TextureBuffer texBuff;
      texBuff.SetTexHeapProp(D3D12_HEAP_TYPE_CUSTOM,D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,D3D12_MEMORY_POOL_L0);
      texBuff.SetTexResourceDesc(textureFile, D3D12_RESOURCE_DIMENSION_TEXTURE2D, 1);
      //--------------------------------------


      //テクスチャバッファの生成----------------------
      dx.result = texBuff.CommitResouce(dx, D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr);
      texBuff.TransferMipmatToTexBuff(textureFile, nullptr, dx.result);
      //-----------------------------------
#pragma endregion 画像関係

         //デスクリプタヒープの生成-------------------------
#pragma region デスクリプタヒープの生成

     const size_t kMaxSRVCount = 2056;
     Descriptor descriptor;
     descriptor.SetHeapDesc(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, kMaxSRVCount);
     dx.result = descriptor.SetDescriptorHeap(dx);
     descriptor.SetSrvHeap();

#pragma endregion デスクリプタヒープの生成
    //-------------------------------

     //シェーダーリソースビューの作成------------------------------
#pragma region シェーダーリソースビューの作成

     descriptor.SetSrvDesc(texBuff, D3D12_SRV_DIMENSION_TEXTURE2D);

     descriptor.SetShaderResourceView(dx, texBuff);

#pragma endregion シェーダーリソースビューの作成
     //----------------------------

     //デスクリプタレンジの設定--------------------------------
#pragma region デスクリプタレンジの設定

     descriptor.SetDescriptorRange(1, D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0);

#pragma endregion デスクリプタレンジの設定
     //-----------------------------------------

     //ルートパラメータの設定---------------------------
#pragma region ルートパラメータの設定

     RootParameter rootparams;
     rootparams.SetRootParam(D3D12_ROOT_PARAMETER_TYPE_CBV, 0, 0, D3D12_SHADER_VISIBILITY_ALL,descriptor,0);
     rootparams.SetRootParam(D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE, 0, 0, D3D12_SHADER_VISIBILITY_ALL,descriptor,1);
     rootparams.SetRootParam(D3D12_ROOT_PARAMETER_TYPE_CBV, 1, 0, D3D12_SHADER_VISIBILITY_ALL, descriptor, 0);
#pragma endregion ルートパラメータの設定
     //------------------------

     //頂点データ---------------------------------
#pragma region 頂点データ
   
     Vertex vertex;

#pragma endregion 頂点データ
     //--------------------------
    
     //インデックスバッファの設定-------------------------
#pragma region インデックスの設定
    
     objMaterial.SetIndex(D3D12_RESOURCE_DIMENSION_BUFFER, vertex.sizeIB, 1, 1, 1, 1, D3D12_TEXTURE_LAYOUT_ROW_MAJOR);

#pragma endregion インデックスの設定
     //------------------------

#pragma region インデックスバッファ生成

     vertex.CreateIndexBuffer(dx, objMaterial.HeapProp, D3D12_HEAP_FLAG_NONE,objMaterial.Resdesc, D3D12_RESOURCE_STATE_GENERIC_READ);

#pragma endregion インデックスバッファ生成

     //インデックスバッファへのデータ転送------------------------------
#pragma region インデックスバッファへのデータ転送

     dx.result = vertex.IndexMaping();

#pragma endregion インデックスバッファへのデータ転送
    //-------------------------------------

     //インデックスバッファビューの作成-----------------------------------
#pragma region インデックスバッファビューの作成
     vertex.SetIbView(DXGI_FORMAT_R16_UINT);
#pragma endregion インデックスバッファビューの作成
     //------------------------------------------

     //頂点バッファ---------------
#pragma region 頂点バッファの設定
     objMaterial.SetVertexBuffer(D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_DIMENSION_BUFFER, vertex.sizeVB, 1, 1, 1, 1, D3D12_TEXTURE_LAYOUT_ROW_MAJOR);
#pragma endregion 頂点バッファの設定
     //----------------------------------

     // 頂点バッファの生成----------------------------
#pragma region 頂点バッファの生成

     vertex.CreateVertexBuffer(dx, objMaterial.HeapProp, D3D12_HEAP_FLAG_NONE, objMaterial.Resdesc, D3D12_RESOURCE_STATE_GENERIC_READ);

#pragma endregion 頂点バッファの生成
     //-------------------------

     //法線ベクトル計算---------------------------
#pragma region 法線ベクトル計算
     for (int i = 0; i < _countof(vertex.boxIndices) / 3; i++)
     {
         //三角形1つごとに計算

         //三角形のインデックスを取り出して、一時的な変数に入れる
         unsigned short index0 = vertex.boxIndices[i * 3 + 0];
         unsigned short index1 = vertex.boxIndices[i * 3 + 1];
         unsigned short index2 = vertex.boxIndices[i * 3 + 2];

         //三角形を構成する頂点座標
         XMVECTOR p0 = XMLoadFloat3(&vertex.Box[index0].pos);
         XMVECTOR p1 = XMLoadFloat3(&vertex.Box[index1].pos);
         XMVECTOR p2 = XMLoadFloat3(&vertex.Box[index2].pos);

         //p0->p1ベクトル、p0->p2ベクトルを計算（ベクトルの減算）
         XMVECTOR v1 = XMVectorSubtract(p1, p0);
         XMVECTOR v2 = XMVectorSubtract(p2, p0);

         //外積は両方から垂直なベクトル
         XMVECTOR normal = XMVector3Cross(v1, v2);

         //正規化（長さを一にする)
         normal = XMVector3Normalize(normal);

         //求めた法線を頂点データに代入
         XMStoreFloat3(&vertex.Box[index0].normal, normal);
         XMStoreFloat3(&vertex.Box[index1].normal, normal);
         XMStoreFloat3(&vertex.Box[index2].normal, normal);

     }
#pragma endregion 法線ベクトルを計算
     //-------------------------

     // 頂点バッファへのデータ転送------------
#pragma region 頂点バッファへのデータ転送
     StructVertex* vertMap = nullptr;
     dx.result = vertex.vertBuff->Map(0, nullptr, (void**)&vertMap);
     assert(SUCCEEDED(dx.result));

     // 全頂点に対して
     for (int i = 0; i < _countof(vertex.Box); i++)
     {
         vertMap[i] = vertex.Box[i];   // 座標をコピー
     }

     // マップを解除
     vertex.vertBuff->Unmap(0, nullptr);
#pragma endregion 頂点バッファへのデータ転送
     //--------------------------------------

     // 頂点バッファビューの作成--------------------------
#pragma region 頂点バッファビューの作成
     D3D12_VERTEX_BUFFER_VIEW vbView{};

     vbView.BufferLocation = vertex.vertBuff->GetGPUVirtualAddress();
     vbView.SizeInBytes = vertex.sizeVB;
     vbView.StrideInBytes = sizeof(vertex.Box[0]);
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
         {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},//法線ベクトル
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
     gpipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;  // 背面カリング
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

     depth.SetDepthStencilState(gpipelineDesc);

   //テクスチャサンプラーの設定-----------------------
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
     rootSignatureDesc.pParameters = &rootparams.rootparams.front(); //ルートパラメータの先頭アドレス
     rootSignatureDesc.NumParameters = rootparams.rootparams.size(); //ルートパラメータ数
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

     //ゲームループ用変数--------------------------------
#pragma region ゲームループ用変数
     float angle = 0.0f;
     const int DEFAULT_MOVE_LIMIT = 60;

     XMFLOAT3 targetVec = { 0,0,1 };
     XMFLOAT3 Angle = { 0,0,0 };
     XMFLOAT3 BeforePosition = { 0,0,0 };
     XMINT3 moveLimit = { DEFAULT_MOVE_LIMIT,DEFAULT_MOVE_LIMIT,DEFAULT_MOVE_LIMIT};
     XMINT3 moveTime = {0,0,0};
     XMFLOAT3 trackingPos = { 0,0,0 };
     XMFLOAT3 trackingDistance = { 100,100,100 };

#pragma endregion ゲームループ用変数
     //--------------------------
     
     //ゲームループ-------------------------------------
#pragma region ゲームループ
    while (true)
    {
        input.UpDateInit(dx.result);

        dxWindow.messageUpdate();

        if (input.IsKeyDown(DIK_ESCAPE) || dxWindow.breakFlag)
        {
            break;
        }

#pragma region 更新処理
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

#pragma region 追尾中に追尾先が移動した場合の処理
            //x軸
            if (trackingPos.x != matView.target.x + (targetVec.x * trackingDistance.x))
            {
                trackingPos.x = matView.target.x + (targetVec.x * trackingDistance.x);//追尾先を変更
                BeforePosition.x = object3D[0].position.x;//イージング用移動前座標を現在位置と同期
                moveTime.x = 0;//タイマーリセット
            }
            //y軸
            if (trackingPos.y != matView.target.y + (targetVec.y * trackingDistance.y))
            {
                trackingPos.y = matView.target.y + (targetVec.y * trackingDistance.y);//追尾先を変更
                BeforePosition.y = object3D[0].position.y;//イージング用移動前座標を現在位置と同期
                moveTime.y = 0;//タイマーリセット
            }
            //z軸
            if (trackingPos.z != matView.target.z + (targetVec.z * trackingDistance.z))
            {
                trackingPos.z = matView.target.z + (targetVec.z * trackingDistance.z);//追尾先を変更
                BeforePosition.z = object3D[0].position.z;//イージング用移動前座標を現在位置と同期
                moveTime.z = 0;//タイマーリセット
            }
#pragma endregion 追尾中に追尾先が移動した場合の処理

#pragma region 追尾
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
#pragma endregion 追尾

        }
        for (int i = 0; i < _countof(object3D); i++)
        {
            object3D[i].Updata(matView, matProjection);
        }


        for (int i = 0; i < _countof(Rales); i++)
        {
            Rales[i].Updata(matView, matProjection);
        }

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
        D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = depth.dsvHeap->GetCPUDescriptorHandleForHeapStart();
        dx.commandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

#pragma endregion 2．描画先指定
        //-------------------
        
        //３．画面クリア-------------
#pragma region 3.画面クリア
        dx.commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
        dx.commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
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

        
        //定数バッファビュー(CBV)の設定コマンド
        dx.commandList->SetGraphicsRootConstantBufferView(0, objMaterial.constBuffMaterial->GetGPUVirtualAddress());

        //SRVヒープの設定コマンド
        dx.commandList->SetDescriptorHeaps(1, descriptor.srvHeap.GetAddressOf());

        //SRVヒープの先頭アドレスを取得
        D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descriptor.srvHeap->GetGPUDescriptorHandleForHeapStart();

        //SRVヒープの先頭にあるSRVをパラメータ1番に設定
        dx.commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

        for (int i = 0; i < _countof(object3D); i++)
        {
            object3D[i].Draw(dx, vbView, vertex.ibView, _countof(vertex.boxIndices));
        }

        for (int i = 0; i < _countof(Rales); i++)
        {
            Rales[i].Draw(dx,  vbView, vertex.ibView, _countof(vertex.boxIndices));
        }

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
#pragma endregion ゲームループ
    //---------------------------------
	return 0;
}
