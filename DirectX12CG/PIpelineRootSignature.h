#pragma once
#include "Pipeline.h"
#include "RootSignature.h"
#include "Shader.h"
#include "Depth.h"
#include "TexSample.h"
#include "RootParameter.h"

namespace MCB
{

	class PipelineRootSignature
	{
	public:
		Pipeline pipeline;
		RootSignature rootsignature;
		Shader shader;
		TexSample texSample;

		PipelineRootSignature Create3DObjectPipeline(Dx12 &dx12,Depth& depth, RootParameter& rootparam);

		PipelineRootSignature CreateSpritePipeline(Dx12 &dx12,Depth& depth, RootParameter& rootparam);

	};
}

