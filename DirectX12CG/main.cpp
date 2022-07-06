#define _CRTDBG_MAP_ALLOC 
#include <crtdbg.h>
#include <cstdlib>

#define _USE_MATH_DEFINES
#include <cmath>


#pragma region �W��.h include

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


#pragma endregion �W��.h include

#pragma region ����.h include

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
#include "Collider.h"

#pragma endregion ����.h include

#pragma region �Q�[���n.h include

#include "RayObject.h"

#pragma endregion �Q�[���n.h include

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

// Windows�A�v���ł̃G���g���[�|�C���g(main�֐�) 
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    {
    //_CrtSetBreakAlloc(1030);

    //int* hoge = new int(4);
    DxWindow* dxWindow = new DxWindow;

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

    //DirectX�N���X����
    Dx12* dx = new Dx12(*dxWindow);
    //input�N���X����
    Input* input = new Input(dx->result, dxWindow->window, dxWindow->hwnd);


#pragma endregion 


#pragma region �`�揉��������

    //�[�x�o�b�t�@----
    Depth depth(*dxWindow, *dx);
    //-------
    ShaderResource descriptor;
    descriptor.Init(*dx);

    Draw draw;


     //�s��-----------------------
#pragma region �s��
        //�r���[�ϊ��s��
    View matView;
    matView.CreateMatrixView(XMFLOAT3(0.0f, 40.0f, -100.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));
    //�ˉe�ϊ��s��
    Projection matProjection;
    matProjection.CreateMatrixProjection(XMConvertToRadians(45.0f), (float)dxWindow->window_width / dxWindow->window_height, 0.1f, 4000.0f);
#pragma endregion �s��
    //---------------------


    //���[�g�p�����[�^�̐ݒ�---------------------------
#pragma region ���[�g�p�����[�^�̐ݒ�

    RootParameter rootparams;
    rootparams.SetRootParam(D3D12_ROOT_PARAMETER_TYPE_CBV, 0, 0, D3D12_SHADER_VISIBILITY_ALL, descriptor, 0);
    rootparams.SetRootParam(D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE, 0, 0, D3D12_SHADER_VISIBILITY_ALL, descriptor, 1);
    rootparams.SetRootParam(D3D12_ROOT_PARAMETER_TYPE_CBV, 1, 0, D3D12_SHADER_VISIBILITY_ALL, descriptor, 0);
#pragma endregion ���[�g�p�����[�^�̐ݒ�
    //------------------------

    PipelineRootSignature obj3dPipeline = obj3dPipeline.Create3DObjectPipeline(*dx,depth,rootparams);

    PipelineRootSignature spritePipeline = spritePipeline.CreateSpritePipeline(*dx, depth, rootparams);
     


    //�e�N�X�`���ǂݍ���
    Texture testTex;
    testTex.CreateTexture(*dx, L"Resources\\reimu.png", &descriptor);
    Texture debugTextTexture;
    debugTextTexture.CreateTexture(*dx, L"Resources\\debugfont.png", &descriptor);

    //3D���f���ǂݍ���
    Model* BoxModel = new Model(*dx, "Box", &descriptor);
    
    Model* groundModel = new Model(*dx, "ground", &descriptor);

    Model* skydomeModel = new Model(*dx, "skydome", &descriptor);



    //3D�I�u�W�F�N�g�̐���-------------------
#pragma region 3D�I�u�W�F�N�g�̐���
    //Object3d* Box = new Object3d(*dx);
    std::array<Object3d, 20> Box;
    std::array<Object3d, 40> Box2;

    Object3d ground;
    ground.Init(*dx);
    ground.model = groundModel;
    ground.scale = { 4,4,4 };
    ground.position = { 0,-15,0 };
    ;
    Object3d Skydorm;
    Skydorm.Init(*dx);
    Skydorm.model = skydomeModel;
    Skydorm.scale = { 4,4,4 };

    RayObject ray;
    ray.Init(*dx);
    ray.model = BoxModel;
    ray.scale = { 1,1,30 };


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

    


#pragma endregion 3D�I�u�W�F�N�g�̐���
    //----------------------

    //�X�v���C�g�̐���---------------
    Sprite sprite;
    sprite.InitMatProje(*dxWindow);
    sprite = sprite.CreateSprite(*dx, *dxWindow);


    DebugText debugText;
    debugText.Init(*dx, *dxWindow,&debugTextTexture);

    //-----------------------

    float clearColor[] = { 0.0f,0.25f, 0.5f,0.0f }; // ���ۂ��F


#pragma endregion

#pragma region �T�E���h
    //�T�E���h������
    SoundManager soundManager;
    //�T�E���h�n�ǂݍ���
    int testSound = soundManager.LoadWaveSound("Resources\\cat1.wav");

    //soundManager.PlaySoundWave(testSound);
#pragma endregion �T�E���h

     //�Q�[�����[�v�p�ϐ�--------------------------------
#pragma region �Q�[�����[�v�p�ϐ�

    XMFLOAT3 targetVec = { 0,0,1 };
    XMFLOAT3 Angle = { 0,0,0 };


    bool SelectVio = true;
    int count = 0;

    int distance = 20;
#pragma endregion �Q�[�����[�v�p�ϐ�
    //--------------------------

    //�Q�[�����[�v-------------------------------------
#pragma region �Q�[�����[�v
    while (true)
    {

        input->UpDateInit(dx->result);

        dxWindow->messageUpdate();

        if (input->IsKeyDown(DIK_ESCAPE) || dxWindow->breakFlag)
        {
            break;
        }

#pragma region �X�V����

        if (input->IsKeyDown(DIK_D))
        {
            matView.eye.x++;
        }

        if (input->IsKeyDown(DIK_A))
        {
            matView.eye.x--;
        }

        if (input->IsKeyDown(DIK_W))
        {
            matView.eye.z++;
        }

        if (input->IsKeyDown(DIK_S))
        {
            matView.eye.z--;
        }

        matView.target.x = ray.position.x;
        matView.target.y = ray.position.y;
        matView.target.z = ray.position.z;

        matView.UpDateMatrixView();


        for (int i = 0; i < 9; i++)
        {
            Box[i].Updata(matView, matProjection);
        }

        for (int i = 0; i < Box2.size(); i++)
        {
            Box2[i].Updata(matView, matProjection);
        }

        
        Skydorm.Updata(matView, matProjection);
        ground.Updata(matView, matProjection);

        ray.Updata(matView, matProjection);

#pragma endregion �X�V����

#pragma region �`�揈��

        draw.PreDraw(*dx, depth, descriptor, obj3dPipeline, *dxWindow, clearColor);

        Skydorm.Draw(*dx, descriptor);
        ground.Draw(*dx, descriptor);

        //for (int i = 0; i < Box.size(); i++)
        //{
        //    Box[i].Draw(*dx, descriptor);
        //}

        //for (int i = 0; i < Box2.size(); i++)
        //{
        //    Box2[i].Draw(*dx, descriptor,0);
        //}

        ray.Draw(*dx, descriptor);

        sprite.SpriteCommonBeginDraw(*dx, spritePipeline, descriptor);

        //sprite.SpriteFlipDraw(sprite, *dx, descriptor, testTex, (float)dxWindow->window_width / 2, (float)dxWindow->window_height / 2);
        //debugText.Print(0, 600, 1, "hogehogehogehoge",Box[0].position.x, Box[0].position.y, Box[0].position.z);

        //sprite.SpriteDraw(sprite, *dx, descriptor, ground.model->texture);

        debugText.AllDraw(descriptor);

#pragma endregion �`��R�}���h
        //----------------------


        draw.PostDraw(*dx);

#pragma endregion �R�}���h���X�g���s
        //------------------

#pragma endregion �`�揈��


    }
#pragma endregion �Q�[�����[�v

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
