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
#include "Projection.h"

namespace MCB
{
    class Object3d
    {
    public:
        //�萔�o�b�t�@�p�\����(�s��)------------------------
        typedef struct ConstBufferDataTransform
        {
            DirectX::XMMATRIX mat;
        };
        //---------------------------------
    
        //�s��p�萔�o�b�t�@
        Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTranceform = nullptr;

        //�s��p�萔�o�b�t�@�}�b�v
        ConstBufferDataTransform* constMapTranceform = nullptr;

        //�A�t�B���ϊ����
        DirectX::XMFLOAT3 scale = { 1.0f,1.0f,1.0f };
        DirectX::XMFLOAT3 rotasion = { 0.0f,0.0f,0.0f };
        DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };

        //���[���h�s��
        WorldMatrix matWorld;
        //�e�I�u�W�F�N�g�ւ̃|�C���^
        Object3d* parent = nullptr;




        void Init(Dx12& dx12);

        void Updata(View& view, Projection& projection);

        void Draw(Dx12 dx12, D3D12_VERTEX_BUFFER_VIEW& vbView,D3D12_INDEX_BUFFER_VIEW& ibView,UINT numIndices);
    };

}
