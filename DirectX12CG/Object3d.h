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
#include "Vector3D.h"

namespace MCB
{
    class Object3d
    {
    public:
        //定数バッファ用構造体(行列)------------------------
        typedef struct ConstBufferDataTransform
        {
            DirectX::XMMATRIX mat;
        };
        //---------------------------------

        //行列用定数バッファ
        Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTranceform = nullptr;

        //行列用定数バッファマップ
        ConstBufferDataTransform* constMapTranceform = nullptr;

        //アフィン変換情報
        DirectX::XMFLOAT3 scale = { 1.0f,1.0f,1.0f };
        DirectX::XMFLOAT3 rotasion = { 0.0f,0.0f,0.0f };
        DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };

        //ワールド行列
        WorldMatrix matWorld;

        Vector3D NORM_FRONT_VEC;
        Vector3D nowFrontVec;
        float frontAngle;

        Object3d();

        //親オブジェクトへのポインタ
        Object3d* parent = nullptr;

        bool trackingFlag = false;

        void Init(Dx12& dx12);

        void Updata(View& view, Projection& projection, bool isBillBord = false);

        void Draw(Dx12 dx12, D3D12_VERTEX_BUFFER_VIEW& vbView,D3D12_INDEX_BUFFER_VIEW& ibView,UINT numIndices);
    };

}

