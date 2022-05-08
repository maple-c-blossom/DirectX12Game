#include "RootParameter.h"

void MCB::RootParameter::SetRootParam(D3D12_ROOT_PARAMETER_TYPE paramType, int ShaderRegister, int RegisterSpace, D3D12_SHADER_VISIBILITY shaderVisibility, Descriptor &descriptor,int NumDescriptorRanges)
{
    D3D12_ROOT_PARAMETER rootparam{};
    rootparam.ParameterType = paramType;//種類
    rootparam.ShaderVisibility = shaderVisibility;//全てのシェーダーから見える

    if (paramType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
    {
        rootparam.DescriptorTable.pDescriptorRanges = &descriptor.descriptorRange;
        rootparam.DescriptorTable.NumDescriptorRanges = NumDescriptorRanges;
    }
    else if (paramType == D3D12_ROOT_PARAMETER_TYPE_CBV)
    {
        rootparam.Descriptor.ShaderRegister = ShaderRegister;//定数バッファ番号
        rootparam.Descriptor.RegisterSpace = RegisterSpace;//デフォルト値
    }
    else
    {
        return;
    }

    rootparams.push_back(rootparam);

}
