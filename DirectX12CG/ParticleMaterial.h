#pragma once
#include <d3dcompiler.h>
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <wrl.h>
#include "WorldMatrix.h"
#include "Dx12.h"
#include "View.h"
#include "Vertex.h"
#include "MCBFloat.h"

namespace MCB
{
	class ParticleMaterial
	{
	public:
		////定数バッファ用構造体(マテリアル)-----------------------------------
		//typedef struct ConstBufferDataMaterial
		//{
		//	DirectX::XMFLOAT4 color = { 0,0,0,1 };
		//};
		////------------------------------------------

		typedef struct ConstBufferDataMaterialB1
		{

			Float3 ambient;
			float pad1;
			Float3 diffuse;
			float pad2;
			Float3 specular;
			float alpha;
		};


		typedef struct Material
		{
			std::string name;
			Float3 ambient;
			Float3 diffuse;
			Float3 specular;
			float alpha;
			std::string textureFileName;

			Material()
			{
				ambient = { 0.3f,0.3f,0.3f };
				diffuse = { 0.0f,0.0f,0.0f };
				specular = { 0.0f,0.0f,0.0f };
				alpha = 1.0f;
			}
		};



		Material material;
		D3D12_HEAP_PROPERTIES HeapProp{};
		D3D12_RESOURCE_DESC Resdesc{};
		//Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial = nullptr;
		Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterialB1 = nullptr;
		ConstBufferDataMaterialB1* constMapMaterial = nullptr;

		~ParticleMaterial();

		void Init(Dx12& dx12);

		void SetIndex(D3D12_RESOURCE_DIMENSION dimension, UINT sizeIB, int height, int DepthOrArraySize, int MipLevels, int SampleDescCount, D3D12_TEXTURE_LAYOUT layout);

		void SetVertexBuffer(D3D12_HEAP_TYPE heaptype, D3D12_RESOURCE_DIMENSION dimension, UINT sizeVB, int height, int DepthOrArraySize, int MipLevels, int SampleDescCount, D3D12_TEXTURE_LAYOUT layout);

	};

}

