#pragma once
#include "MCBFloat.h"
#include <d3d12.h>
#include <wrl.h>
#include "Dx12.h"
#include "DxWindow.h"
#include "Object3d.h"
#include "PIpelineRootSignature.h"
#include "Descriptor.h"
#include "Texture.h"
namespace MCB
{


	class Sprite
	{
	public:
		//定数バッファ用構造体(行列)------------------------
		typedef struct SpriteConstBufferDataTransform
		{
			Float4 color;
			DirectX::XMMATRIX mat;
		};
		//---------------------------------

		typedef struct SpriteVertex
		{
			Float3 pos;
			Float2 uv;
		};


		Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;
		D3D12_VERTEX_BUFFER_VIEW vbView{};
		Microsoft::WRL::ComPtr<ID3D12Resource> constBuff = nullptr;
		D3D12_HEAP_PROPERTIES HeapProp{};
		D3D12_HEAP_PROPERTIES constHeapProp{};
		D3D12_RESOURCE_DESC Resdesc{};
		D3D12_RESOURCE_DESC constResdesc{};

		float rotation = 0.0f;
		Float3 position = { 0,0,0 };
		DirectX::XMMATRIX matWorld;
		Float4 color = { 1.0f,1.0f,1.0f,1.0f };
		static DirectX::XMMATRIX matProje;
		Float2 size = { 100,100 };
		Float2 anchorPoint = { 0.5f,0.5f };
		Float2 texLeftTop = { 0,0 };
		Float2 cuttingSize = { 0,0 };
		bool isFlipX = false;
		bool isFlipY = false;


		void SpriteTransferVertexBuffer(const Sprite& sprite, Texture* tex = nullptr);
		void SpriteUpdate(Sprite& sprite);
		void InitMatProje(DxWindow& dxWindow);
		Sprite CreateSprite(Dx12& dx12, DxWindow& dxWindow);
		void SpriteCommonBeginDraw(Dx12& dx12, const PipelineRootSignature& pipeline, ShaderResource& descHeap);

		void SpriteDraw(Sprite& sprite,Dx12& dx12, ShaderResource descriptor, Texture& tex, float positionX = 0,float positionY = 0,
						float size_x = 0, float size_y = 0);

		void SpriteFlipDraw(Sprite& sprite, Dx12& dx12, ShaderResource descriptor, Texture& tex, float positionX = 0, float positionY = 0, 
							bool isflipX = false, bool isflipY = false);

		void SpriteCuttingDraw(Sprite& sprite, Dx12& dx12, ShaderResource descriptor, Texture& tex, float positionX = 0, float positionY = 0,
			Float2 cuttingsize = {0,0},Float2 CuttingLeftTop = {0,0});



	};

}

