#pragma once

#include <d3d12.h>
#include <wrl.h>
#include "Dx12.h"
#include "TextureBuffer.h"

namespace MCB
{

	class Descriptor
	{
		public:
            //�萔�o�b�t�@�p�̃f�X�N���v�^�q�[�v

            //�ݒ�\����
            D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc{};

            //�f�X�N���v�^�q�[�v�̐���  
            Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap = nullptr;
            
            D3D12_CPU_DESCRIPTOR_HANDLE srvHandle; 

            //�V�F�[�_�[���\�[�X�r���[�ݒ�
            D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};

            //�f�X�N���v�^�����W�̐ݒ�
            D3D12_DESCRIPTOR_RANGE descriptorRange{};

            void SetHeapDesc(D3D12_DESCRIPTOR_HEAP_TYPE type, D3D12_DESCRIPTOR_HEAP_FLAGS flags, const size_t kMax);

            HRESULT SetDescriptorHeap(Dx12 &dx12);

            void SetSrvHeap();

            void SetSrvDesc(TextureBuffer &texBuffer, D3D12_SRV_DIMENSION srvDimension);

            void SetShaderResourceView(Dx12& dx12, TextureBuffer& texBuffer);

            void SetDescriptorRange(int NumDescriptors, D3D12_DESCRIPTOR_RANGE_TYPE type,int BaseShaderRegister);
	};
}

