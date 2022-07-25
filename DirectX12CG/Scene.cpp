#include "Scene.h"

using namespace MCB;
using namespace DirectX;

MCB::Scene::~Scene()
{
    soundManager.ReleasexAudio2();
    soundManager.AllDeleteSound();

    delete player;
    delete BoxModel;
    delete skydomeModel;
    delete groundModel;
}

#pragma region �ʏ�ϐ��̏�������3D�I�u�W�F�N�g�̏�����
void MCB::Scene::Initialize()
{
    matView.CreateMatrixView(XMFLOAT3(0.0f, 0.0f, -100.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));
    matProjection.CreateMatrixProjection(XMConvertToRadians(45.0f), (float)dxWindow->window_width / dxWindow->window_height, 0.1f, 4000.0f);
    LoadTexture();
    LoadModel();
    LoadSound();
    Object3DInit();
    SpriteInit();

    angle = 0;
    anglemove = 0.1f;

    isJump = false;
    jumpSpeed = 5;
}

void MCB::Scene::Object3DInit()
{
    human;
    human.Init(BoxModel);

    ground;
    ground.Init();
    ground.model = groundModel;
    ground.scale = { 4,4,4 };
    ground.position = { 0,-15,0 };
    ;
    Skydorm;
    Skydorm.Init();
    Skydorm.model = skydomeModel;
    Skydorm.scale = { 4,4,4 };

    player = new Player;
    player->Initialize(BoxModel);
}

#pragma endregion �ʏ�ϐ��̏�����

#pragma region �e�탊�\�[�X�̓ǂݍ��݂Ə�����
void MCB::Scene::LoadModel()
{
	BoxModel = new Model("Box");

	groundModel = new Model("ground");

	skydomeModel = new Model("skydome");
}

void MCB::Scene::LoadTexture()
{
	testTex.CreateTexture(L"Resources\\reimu.png");
	debugTextTexture.CreateTexture(L"Resources\\debugfont.png");
    zoomTex.CreateTexture(L"Resources\\reticle.png");
    scopeTex.CreateTexture(L"Resources\\scope.png");

}

void MCB::Scene::LoadSound()
{
	testSound = soundManager.LoadWaveSound("Resources\\cat1.wav");
}

void MCB::Scene::SpriteInit()
{
    sprite.InitMatProje();
    sprite = sprite.CreateSprite();
    zoomSprite.InitMatProje();
    zoomSprite = zoomSprite.CreateSprite();
    scopeSprite.InitMatProje();
    scopeSprite = scopeSprite.CreateSprite();
    debugText.Init(&debugTextTexture);

}
#pragma endregion �e�탊�\�[�X�̓ǂݍ��݂Ə�����

void MCB::Scene::Update()
{
    player->Update();
    //�s��ϊ�
    MatrixUpdate();
}

void MCB::Scene::Object3DDraw()
{
    Skydorm.Draw();
    player->Draw();


}

void MCB::Scene::DrawSprite()
{
;
debugText.Print(20, 20, 1, "position:%f,%f,%f", player->playerObj.position.x, player->playerObj.position.y, player->playerObj.position.z);
    debugText.AllDraw();
}

void MCB::Scene::Draw()
{
    draw.PreDraw(*depth, *obj3dPipelinePtr, clearColor);
    //3D�I�u�W�F�N�g
    Object3DDraw();
    //�X�v���C�g
    sprite.SpriteCommonBeginDraw(*spritePipelinePtr);
    DrawSprite();
    draw.PostDraw();
}

void MCB::Scene::MatrixUpdate()
{
    matProjection.UpdataMatrixProjection();
    matView.UpDateMatrixView();
    human.UpDate(matView, matProjection);
    Skydorm.MatrixUpdata(matView, matProjection);
    ground.MatrixUpdata(matView, matProjection);
    player->MatrixUpdata(matView, matProjection);
}


MCB::Scene::Scene(RootParameter* root, Depth* depthptr, PipelineRootSignature* pipeline, PipelineRootSignature* pipeline1)
{
	rootparamsPtr = root;
	obj3dPipelinePtr = pipeline;
	spritePipelinePtr = pipeline1;
    depth = depthptr;
}

