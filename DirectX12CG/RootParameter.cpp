#include "RootParameter.h"

void MCB::RootParameter::SetRootParam(D3D12_ROOT_PARAMETER_TYPE paramType, int ShaderRegister, int RegisterSpace, D3D12_SHADER_VISIBILITY shaderVisibility, Descriptor &descriptor,int NumDescriptorRanges)
{
	D3D12_ROOT_PARAMETER rootparam;
    rootparam.ParameterType = paramType;//���
    rootparam.ShaderVisibility = shaderVisibility;//�S�ẴV�F�[�_�[���猩����

    if (paramType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
    {
        rootparam.DescriptorTable.pDescriptorRanges = &descriptor.descriptorRange;
        rootparam.DescriptorTable.NumDescriptorRanges = NumDescriptorRanges;
    }
    else if (paramType == D3D12_ROOT_PARAMETER_TYPE_CBV)
    {
        rootparam.Descriptor.ShaderRegister = ShaderRegister;//�萔�o�b�t�@�ԍ�
        rootparam.Descriptor.RegisterSpace = RegisterSpace;//�f�t�H���g�l
    }
    else
    {
        return;
    }

    rootparams.push_back(rootparam);

}
