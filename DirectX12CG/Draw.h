#pragma once
#include "Dx12.h"
#include "Depth.h"
#include "Descriptor.h"
#include "PIpelineRootSignature.h"
#include "DxWindow.h"

namespace MCB
{
	class Draw
	{
	public:
		D3D12_RESOURCE_BARRIER barrierDesc;
		UINT bbIndex;
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;
		D3D12_VIEWPORT viewport;
		D3D12_RECT scissorrect;


		D3D12_RESOURCE_BARRIER GetResouceBarrier();
		UINT GetBbIndex();
		


		void SetBeforeBbIndex(Dx12& dx12);
		void SetBeforeResourceBarrier(Dx12 &dx12);
		void BeforeDraw(Dx12 &dx12,Depth depth, ShaderResource srv,PipelineRootSignature pipeline);
		void SetRenderTargetView(Dx12& dx12,Depth& depth);
		void ClearScreen(Dx12& dx12,const float* clearColor);
		void SetViewPort(Dx12& dx12,DxWindow& dxWindow);
		void SetScissorrect(Dx12& dx12, DxWindow& dxWindow);
		void PreDraw(Dx12& dx12, Depth depth, ShaderResource srv, PipelineRootSignature pipeline, DxWindow& dxWindow, const float* clearColor);


		void SetAfterResourceBarrier(Dx12& dx12);
		void CloseDrawCommandOrder(Dx12& dx12);
		void CommandListExecution(Dx12& dx12);
		void ResetQueAndCommandList(Dx12& dx12);
		void PostDraw(Dx12& dx12);
	};

}

