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
#include "DebugText.h"
#include "Sound.h"

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

    Draw draw;


     //行列-----------------------
#pragma region 行列
        //ビュー変換行列
    View matView;
    matView.CreateMatrixView(XMFLOAT3(0.0f, 0.0f, -100.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));
    //射影変換行列
    Projection matProjection;
    matProjection.CreateMatrixProjection(XMConvertToRadians(45.0f), (float)dxWindow->window_width / dxWindow->window_height, 0.1f, 4000.0f);
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
     


    //テクスチャ読み込み
    Texture testTex;
    testTex.CreateTexture(*dx, L"Resources\\reimu.png", &descriptor);
    Texture debugTextTexture;
    debugTextTexture.CreateTexture(*dx, L"Resources\\debugfont.png", &descriptor);

    //3Dモデル読み込み
    Model* BoxModel = new Model(*dx, "Box", &descriptor);
    
    Model* groundModel = new Model(*dx, "ground", &descriptor);

    Model* skydomeModel = new Model(*dx, "skydome", &descriptor);



    //3Dオブジェクトの生成-------------------
#pragma region 3Dオブジェクトの生成
    //Object3d* Box = new Object3d(*dx);
    std::array<Object3d, 20> Box;
    std::array<Object3d, 40> Box2;

    Object3d ground;
    ground.Init(*dx);
    ground.model = groundModel;
    ground.scale = { 4,4,4 };
    ground.position = { 0,-15,0 };

    Object3d Skydorm;
    Skydorm.Init(*dx);
    Skydorm.model = skydomeModel;
    Skydorm.scale = { 4,4,4 };


    Box.begin()->model = BoxModel;

    Box.begin()->scale = { 5,5,5 };



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

    //スプライトの生成---------------
    Sprite sprite;
    sprite.InitMatProje(*dxWindow);
    sprite = sprite.CreateSprite(*dx, *dxWindow);


    DebugText debugText;
    debugText.Init(*dx, *dxWindow,&debugTextTexture);

    //-----------------------

    float clearColor[] = { 0.0f,0.25f, 0.5f,0.0f }; // 青っぽい色


#pragma endregion

#pragma region サウンド
    //サウンド初期化
    SoundManager soundManager;
    //サウンド系読み込み
    int testSound = soundManager.LoadWaveSound("Resources\\cat1.wav");

    //soundManager.PlaySoundWave(testSound);
#pragma endregion サウンド

     //ゲームループ用変数--------------------------------
#pragma region ゲームループ用変数

    XMFLOAT3 targetVec = { 0,0,1 };
    XMFLOAT3 Angle = { 0,0,0 };


    bool SelectVio = true;
    int count = 0;
    int distance = 20;

    Quaternion BoxRotationQ = {1,0,0,0};
    BoxRotationQ.SetRota({ 0,1,0 }, 0);
    Float3 rollPitchYawAngle = { 0,0,0 };



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





       

        if (input->IsKeyDown(DIK_RIGHT) || input->IsKeyDown(DIK_LEFT) || input->IsKeyDown(DIK_UP) || input->IsKeyDown(DIK_DOWN))
        {
            XMFLOAT3 move = { 0.0f,0.0f,0.0f };
            if (input->IsKeyDown(DIK_RIGHT)) { move.x += 1.0f; }
            else if (input->IsKeyDown(DIK_LEFT)) { move.x -= 1.0f; }
            if (input->IsKeyDown(DIK_UP)) { move.z += 1.0f; }
            else if (input->IsKeyDown(DIK_DOWN)) { move.z -= 1.0f; }

            matView.eye.x += targetVec.x * move.z;
            matView.eye.y += targetVec.y * move.z;
            matView.eye.z += targetVec.z * move.z;
            matView.eye.x += move.x;
            matView.eye.y += move.y;
            matView.eye.z += move.z;
        }

        matView.target.x = matView.eye.x + targetVec.x;
        matView.target.y = matView.eye.y + targetVec.y;
        matView.target.z = matView.eye.z + targetVec.z;

        matView.UpDateMatrixView();


        if (input->IsKeyDown(DIK_S) || input->IsKeyDown(DIK_W) || input->IsKeyDown(DIK_A)|| input->IsKeyDown(DIK_D) || input->IsKeyDown(DIK_Q) || input->IsKeyDown(DIK_E))
        {
            Vector3D objectRightAxis = { 0,0,1 };
            objectRightAxis = objectRightAxis.GetRightVec(Box.begin()->nowFrontVec);

            Vector3D objectUpAxis = { 0,1,0 };
            objectUpAxis = objectUpAxis.GetUpVec(Box.begin()->nowFrontVec);

            Quaternion PitchQ;
            PitchQ.SetRota(objectRightAxis, rollPitchYawAngle.z);

            Quaternion YawQ;
            YawQ.SetRota(objectUpAxis, rollPitchYawAngle.y);

            Quaternion RollQ;
            RollQ.SetRota(Box.begin()->nowFrontVec, rollPitchYawAngle.x);

            if (input->IsKeyDown(DIK_S) || input->IsKeyDown(DIK_W))
            {

                if (input->IsKeyDown(DIK_S))
                {
                    rollPitchYawAngle.z -= 0.05f;
                }
                else
                {
                    rollPitchYawAngle.z += 0.05f;
                }

                PitchQ.SetRota(objectRightAxis, rollPitchYawAngle.z);

                Quaternion tempVec = { 0,0,0,0 };
                tempVec = PitchQ.SetRotationQuaternion(objectRightAxis, Box.begin()->nowFrontVec, rollPitchYawAngle.z);

                Box.begin()->nowFrontVec.vec.x = tempVec.x;
                Box.begin()->nowFrontVec.vec.y = tempVec.y;
                Box.begin()->nowFrontVec.vec.z = tempVec.z;

                objectUpAxis = objectUpAxis.GetUpVec(Box.begin()->nowFrontVec);
            }

            if (input->IsKeyDown(DIK_Q) || input->IsKeyDown(DIK_E))
            {

                if (input->IsKeyDown(DIK_E))
                {
                    rollPitchYawAngle.y -= 0.05f;
                }
                else
                {
                    rollPitchYawAngle.y += 0.05f;
                }

                YawQ.SetRota(objectUpAxis, rollPitchYawAngle.y);

                Quaternion tempVec = { 0,0,0,0 };
                tempVec = YawQ.SetRotationQuaternion(objectUpAxis, Box.begin()->nowFrontVec, rollPitchYawAngle.z);

                Box.begin()->nowFrontVec.vec.x = tempVec.x;
                Box.begin()->nowFrontVec.vec.y = tempVec.y;
                Box.begin()->nowFrontVec.vec.z = tempVec.z;


            }


            if (input->IsKeyDown(DIK_A) || input->IsKeyDown(DIK_D))
            {

                if (input->IsKeyDown(DIK_A))
                {
                    rollPitchYawAngle.x -= 0.05f;
                }
                else
                {
                    rollPitchYawAngle.x += 0.05f;
                }

                RollQ.SetRota(Box.begin()->nowFrontVec, rollPitchYawAngle.x);
            }


            BoxRotationQ = BoxRotationQ.GetCartesianProduct(PitchQ, YawQ);
            BoxRotationQ = BoxRotationQ.GetCartesianProduct(BoxRotationQ, RollQ);

        }


        for (int i = 0; i < 9; i++)
        {
            Box[i].Updata(matView, matProjection,BoxRotationQ);
        }

        for (int i = 0; i < Box2.size(); i++)
        {
            Box2[i].Updata(matView, matProjection);
        }

        
        Skydorm.Updata(matView, matProjection);
        ground.Updata(matView, matProjection);



#pragma endregion 更新処理

#pragma region 描画処理

        draw.PreDraw(*dx, depth, descriptor, obj3dPipeline, *dxWindow, clearColor);

        Skydorm.Draw(*dx, descriptor);
        ground.Draw(*dx, descriptor);

        //for (int i = 0; i < Box.size(); i++)
        //{
        //    Box[i].Draw(*dx, descriptor);
        //}


        for (int i = 0; i < 1; i++)
        {
            Box[i].Draw(*dx, descriptor);
        }

        for (int i = 0; i < Box2.size(); i++)
        {
            Box2[i].Draw(*dx, descriptor,0);
        }

        sprite.SpriteCommonBeginDraw(*dx, spritePipeline, descriptor);

        //sprite.SpriteFlipDraw(sprite, *dx, descriptor, testTex, (float)dxWindow->window_width / 2, (float)dxWindow->window_height / 2);
        //debugText.Print(0, 600, 1, "hogehogehogehoge",Box[0].position.x, Box[0].position.y, Box[0].position.z);

        //sprite.SpriteDraw(sprite, *dx, descriptor, ground.model->texture);

        debugText.AllDraw(descriptor);



#pragma endregion 描画コマンド
        //----------------------


        draw.PostDraw(*dx);

#pragma endregion コマンドリスト実行
        //------------------

#pragma endregion 描画処理


    }
#pragma endregion ゲームループ

    soundManager.ReleasexAudio2();
    soundManager.AllDeleteSound();
    //testSound.DeleteSound();

    //---------------------------------
    delete dxWindow;
    delete dx;
    delete input;
    delete BoxModel;
    delete skydomeModel;
    delete groundModel;

}   
    _CrtDumpMemoryLeaks();

	return 0;
}
