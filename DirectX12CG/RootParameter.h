#pragma once
#include <d3d12.h>
#include <vector>
#include "Descriptor.h"

namespace MCB
{
	class RootParameter
	{
		public:
			std::vector<D3D12_ROOT_PARAMETER> rootparams;
		~RootParameter();

		void SetRootParam(D3D12_ROOT_PARAMETER_TYPE paramType, int ShaderRegister,int RegisterSpace,
			D3D12_SHADER_VISIBILITY shaderVisibility,ShaderResource &descriptor, int NumDescriptorRanges);

		void SetRootParam(D3D12_ROOT_PARAMETER_TYPE paramType, ShaderResource &descriptor, int ShaderRegister = 0);

	};

}

