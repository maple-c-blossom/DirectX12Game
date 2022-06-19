#define _CRTDBG_MAP_ALLOC 
#include <crtdbg.h>
#include <cstdlib>

#define _USE_MATH_DEFINES
#include <cmath>


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
#include "RootParameter.h"
#include "Vertex.h"
#include "MCBMatrix.h"
#include "Util.h"
#include "Particle.h"
#include "Quaternion.h"
#include <array>
#include "Texture.h"
#include "PIpelineRootSignature.h"
#include "Draw.h"
#include "Sprite.h"

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
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    {
    //_CrtSetBreakAlloc(1030);

    //int* hoge = new int(4);
    DxWindow* dxWindow = new DxWindow;

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
    Dx12* dx = new Dx12(*dxWindow);
    //inputクラス生成
    Input* input = new Input(dx->result, dxWindow->window, dxWindow->hwnd);

#pragma endregion 


#pragma region 描画初期化処理

    //深度バッファ----
    Depth depth(*dxWindow, *dx);
    //-------
    ShaderResource descriptor;
    descriptor.Init(*dx);

    Texture testTex;
    testTex.CreateTexture(*dx, L"Resources\\reimu.png", &descriptor);

    Draw draw;

    Model* BoxModel = new Model(*dx, "hoge");

    //Box->Init(*dx);
    //Box->model->CreateModel("Box");
    //BoxModel->Init(*dx, "Box");

    Box->model = BoxModel;
    Box2->model = BoxModel;

    Box->scale = { 1,1,1 };
    Box2->scale = { 20,20,20 };
    //Particle particle(*dx);
    //particle.vert.material.Init(*dx);
    //particle.Init(*dx);

#pragma endregion 3Dオブジェクトの生成
    //----------------------

     //行列-----------------------
#pragma region 行列
        //ビュー変換行列
    View matView;
    matView.CreateMatrixView(XMFLOAT3(0.0f, 0.0f, -100.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));
    //射影変換行列
    Projection matProjection;
    matProjection.CreateMatrixProjection(XMConvertToRadians(45.0f), (float)dxWindow->window_width / dxWindow->window_height, 0.1f, 1000.0f);
#pragma endregion 行列
    //---------------------


    //ルートパラメータの設定---------------------------
#pragma region ルートパラメータの設定

    RootParameter rootparams;
    rootparams.SetRootParam(D3D12_ROOT_PARAMETER_TYPE_CBV, 0, 0, D3D12_SHADER_VISIBILITY_ALL, descriptor, 0);
    rootparams.SetRootParam(D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE, 0, 0, D3D12_SHADER_VISIBILITY_ALL, descriptor, 1);
    rootparams.SetRootParam(D3D12_ROOT_PARAMETER_TYPE_CBV, 1, 0, D3D12_SHADER_VISIBILITY_ALL, descriptor, 0);
#pragma endregion ルートパラメータの設定
    //------------------------

    PipelineRootSignature obj3dPipeline = obj3dPipeline.Create3DObjectPipeline(*dx,depth,rootparams);

    PipelineRootSignature spritePipeline = spritePipeline.CreateSpritePipeline(*dx, depth, rootparams);
     
    
        
    //3Dオブジェクトの生成-------------------
#pragma region 3Dオブジェクトの生成
    //Object3d* Box = new Object3d(*dx);
    std::array<Object3d, 20> Box;
    std::array<Object3d, 40> Box2;

    Model* BoxModel = new Model(*dx, "Box", &descriptor);

    Box.begin()->model = BoxModel;

    Box.begin()->scale = { 5,5,5 };

    int distance = 20;


    for (int i = 0; i < Box.size(); i++)
    {
        Box[i].Init(*dx);
        Box[i].model = BoxModel;
        if (i > 0)
        {
            Box[i].parent = &Box[i - 1];
            Box[i].scale = { 0.9f,0.9f,0.9f };
            Box[i].rotasion = { 0,0,0.2 };
            Box[i].position = { 0,0,1 };
        }
    }

    for (int i = 0; i < Box2.size(); i++)
    {
        Box2[i].Init(*dx);
        Box2[i].model = BoxModel;
        Box2[i].position.y = -10;
        Box2[i].scale = { 5,5,5 };
        if (i > 0)
        {
              Box2[i].position.z = Box2[i - 1].position.z + 20;
        }
    }




#pragma endregion 3Dオブジェクトの生成
    //----------------------




    Sprite sprite;
    sprite.InitMatProje(*dxWindow);


    sprite = sprite.CreateSprite(*dx, *dxWindow);



    float clearColor[] = { 0.0f,0.25f, 0.5f,0.0f }; // 青っぽい色


#pragma endregion

     //ゲームループ用変数--------------------------------
#pragma region ゲームループ用変数

    XMFLOAT3 targetVec = { 0,0,1 };
    XMFLOAT3 Angle = { 0,0,0 };


    bool SelectVio = true;
    int count = 0;

#pragma endregion ゲームループ用変数
    //--------------------------

    //ゲームループ-------------------------------------
#pragma region ゲームループ
    while (true)
    {

        input->UpDateInit(dx->result);

        dxWindow->messageUpdate();

        if (input->IsKeyDown(DIK_ESCAPE) || dxWindow->breakFlag)
        {
            break;
        }

#pragma region 更新処理

        if (input->IsKeyTrigger(DIK_Q))
        {
            SelectVio = !SelectVio;
        }


        if (input->IsKeyDown(DIK_RIGHT) || input->IsKeyDown(DIK_LEFT) || input->IsKeyDown(DIK_UP) || input->IsKeyDown(DIK_DOWN))
        {
            XMFLOAT3 move = { 0.0f,0.0f,0.0f };
            if (input->IsKeyDown(DIK_RIGHT)) { move.x += 1.0f; }
            else if (input->IsKeyDown(DIK_LEFT)) { move.x -= 1.0f; }

            if (input->IsKeyDown(DIK_UP)) { move.z += 1.0f; }
            else if (input->IsKeyDown(DIK_DOWN)) { move.z -= 1.0f; }

            matView.eye.x += move.x;
            matView.eye.y += move.y;
            matView.eye.z += move.z;


        }


        matView.target.x = matView.eye.x + targetVec.x;
        matView.target.y = matView.eye.y + targetVec.y;
        matView.target.z = matView.eye.z + targetVec.z;

        matView.UpDateMatrixView();

        //angle_test += 0.001f;

        XMMATRIX matrot = XMMatrixIdentity();
        matrot = XMMatrixRotationX(600.0f);
        MCBMatrix matRot;
        MCBMatrix matRot2;
        Quaternion q;
        Quaternion q1;
        Quaternion q2;
        Vector3D vec{ 1,0,0 };
        Vector3D vec1{ 0,1,1 };
        Box->rotasion = { angle_test,0,0 };
        q.SetRota(vec, angle_test);
        q1.SetRota(vec1, angle_test1);

        if (time < MaxTime)
        {
            time++;
        }

        q2 = q2.Slerp(q, q1, time, MaxTime);

        //q = q.SetRotationQuaternion(vec, position, 0.5f);
        matRot = q2.GetQuaternionRotaMat(q2);

        //Vector3D vec1;
        //vec1.vec.x = matRot._21;
        //vec1.vec.y = matRot._22;
        //vec1.vec.z = matRot._23;
        //float len = vec1.V3Len();


        WorldMatrix mat;
        mat.SetMatScale(2, 2, 2);
        mat.SetMatTrans(Box->position.x , Box->position.y, Box->position.z);
        mat.matWorld = XMMatrixIdentity();
        mat.matWorld = matRot.MatrixConvertXMMatrix(q2.GetQuaternionRotaMat(q2) ) * mat.matScale;
/*        mat.matWorld = mat.matWorld * matRot.MatrixConvertXMMatrix(q.GetQuaternionRotaMat(q.GetReciprocal(q)))*/;
        //mat.matWorld = mat.matScale * matRot.MatrixConvertXMMatrix(q.GetQuaternionRotaMat(q));
        mat.matWorld = mat.matWorld * mat.matTransform;

                if (input->IsKeyDown(DIK_D)) { Box[0].rotasion.y += 0.05f; };
                if (input->IsKeyDown(DIK_A)) { Box[0].rotasion.y -= 0.05f; };

                Box[0].nowFrontVec.vec.x = sinf(Box[0].rotasion.y);
                Box[0].nowFrontVec.vec.z = cosf(Box[0].rotasion.y);

                if (input->IsKeyDown(DIK_W)) { tempmove.z += 1.0f; };
                if (input->IsKeyDown(DIK_S)) { tempmove.z -= 1.0f; };

                move.x = Box[0].nowFrontVec.vec.x * tempmove.z;
                move.y = Box[0].nowFrontVec.vec.y * tempmove.z;
                move.z = Box[0].nowFrontVec.vec.z * tempmove.z;


#pragma endregion 1．リソースバリアで書き込み可能に変更
        //--------------------------

        // ２．描画先指定----------------
#pragma region ２．描画先指定

// レンダーターゲットビュー用ディスクリプタヒープのハンドルを取得
        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = dx->rtvHeaps->GetCPUDescriptorHandleForHeapStart();
        rtvHandle.ptr += bbIndex * dx->device->GetDescriptorHandleIncrementSize(dx->rtvHeapDesc.Type);
        D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = depth.dsvHeap->GetCPUDescriptorHandleForHeapStart();
        dx->commandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

#pragma endregion 2．描画先指定
        //-------------------
        
        //３．画面クリア-------------
#pragma region 3.画面クリア
        dx->commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
        dx->commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
#pragma endregion 3.画面クリア
        //---------------------------

        }





        for (int i = 0; i < Box.size(); i++)
        {
            Box[i].Updata(matView, matProjection);
        }

        for (int i = 0; i < Box2.size(); i++)
        {
            Box2[i].Updata(matView, matProjection);
        }


        //sprite.position = { (float)dxWindow->window_width / 2,(float)dxWindow->window_height / 2,0 };
        //sprite.SpriteUpdate(sprite);
        //


#pragma endregion 更新処理

#pragma region 描画処理

        draw.PreDraw(*dx, depth, descriptor, obj3dPipeline, *dxWindow, clearColor);

        

        for (int i = 0; i < Box.size(); i++)
        {
            Box[i].Draw(*dx, descriptor);
        }

        for (int i = 0; i < Box2.size(); i++)
        {
            Box2[i].Draw(*dx, descriptor,0);
        }

        sprite.SpriteCommonBeginDraw(*dx, spritePipeline, descriptor);

        sprite.SpriteDraw(sprite, *dx, descriptor, testTex, (float)dxWindow->window_width / 2, (float)dxWindow->window_height / 2);

#pragma endregion 描画コマンド
        //----------------------


        draw.PostDraw(*dx);

#pragma endregion コマンドリスト実行
        //------------------

#pragma endregion 描画処理


    }
#pragma endregion ゲームループ
    //---------------------------------
    delete dxWindow;
    delete dx;
    delete input;
    delete BoxModel;
    //delete testTex;
}
    _CrtDumpMemoryLeaks();

	return 0;
}
