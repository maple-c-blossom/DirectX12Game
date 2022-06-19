#pragma once
#include <d3dcompiler.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include "DxWindow.h"
#include "Dx12.h"
#include <wrl.h>

namespace MCB
{
	class Depth
	{
	public:
		D3D12_RESOURCE_DESC depthResDesc{};
		D3D12_HEAP_PROPERTIES depthHeapProp{};
		D3D12_CLEAR_VALUE depthClearValue{};
		Microsoft::WRL::ComPtr<ID3D12Resource> depthBuffer = nullptr;
		D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap = nullptr;
		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};

		Depth(DxWindow dxWindow, Dx12& dx12);


		void InitDepthResDesc(DxWindow dxWindow);

		void InitDepthHeapProp();

		void InitDepthClearValue();

		void InitDepthBuffer(Dx12 &dx12);

		void InitDepthDescriptorHeap(Dx12 &dx12);

		void InitDepthStencilView(Dx12& dx12);

		void SetDepthStencilState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc);
		void SetSpriteDepthStencilState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc);
	};
}

