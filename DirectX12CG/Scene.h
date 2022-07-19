#pragma once
#include "IScene.h"
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
#include <array>


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
#include "Texture.h"
#include "PIpelineRootSignature.h"
#include "Draw.h"
#include "Sprite.h"
#include "DebugText.h"
#include "Sound.h"
#include "Collider.h"

#include "Human.h"
#pragma endregion 自作.h include

#pragma region ゲーム系.h include

#include "RayObject.h"
#include "SphereObj.h"
#include "Player.h"

#pragma endregion ゲーム系.h include

#pragma region pragma comment

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#pragma endregion pragma comment

namespace MCB
{
	class Scene :public IScene
	{
	private:
		#pragma region DirectX基礎機能群
		DxWindow* dxWindow = DxWindow::GetInstance();
		//DirectXクラス生成
		Dx12* dx = Dx12::GetInstance();
		//inputクラス生成
		Input* input = Input::GetInstance();

		ShaderResource* descriptor = ShaderResource::GetInstance();

		Depth* depth;
		//ルートパラメータ
		RootParameter* rootparamsPtr;

		PipelineRootSignature* obj3dPipelinePtr;

		PipelineRootSignature* spritePipelinePtr;

		//描画前処理と描画後処理
		MCB::Draw draw;

		//サウンドマネージャー
		SoundManager soundManager;
		//クリアカラー
		float clearColor[4] = { 0.0f,0.25f, 0.5f,0.0f }; // 青っぽい色
#pragma endregion DirectX基礎機能群

		#pragma region 変換行列
		//変換行列
		View matView;
		Projection matProjection;
#pragma endregion 変換行列
		
		#pragma region 各種リソース
		//3Dモデル
		#pragma region 3Dモデル
		Model* BoxModel;
		Model* groundModel;
		Model* skydomeModel;
#pragma endregion 3Dモデル

		//テクスチャ
		#pragma region テクスチャ
		Texture testTex;
		Texture debugTextTexture;
		Texture zoomTex;
		Texture scopeTex;
#pragma endregion テクスチャ

		//サウンド
		#pragma region サウンド
		int testSound;
#pragma endregion サウンド

#pragma endregion 各種リソース

		#pragma region 3Dオブジェクト
		SimpleFigure triangle;
		Human human;
		Object3d ground;
		Object3d Skydorm;
		Player* player;
#pragma endregion 3Dオブジェクト

		#pragma region スプライト
		Sprite sprite;

		Sprite zoomSprite;

		Sprite scopeSprite;

		DebugText debugText;

#pragma endregion スプライト

		#pragma region 通常変数
		float angle = 0;
		float anglemove = 0.1f;
		bool isJump = false;
		float jumpSpeed = 5;
#pragma endregion 通常変数

	public:
		Scene(RootParameter* root,Depth* depth, PipelineRootSignature* pipeline, PipelineRootSignature* pipeline1);
		~Scene();
		void Initialize() override;

		//各初期化系関数群--------------------
		void LoadModel()  override;
		void LoadTexture()  override;
		void LoadSound()  override;
		void Object3DInit()  override;
		void SpriteInit()  override;
		//---------------

		void MatrixUpdate();
		void Update() override;
		void Draw() override;

		//各描画系関数群---------------
		void DrawSprite() override;
		void Object3DDraw() override;
		//-------------------
	};

}

