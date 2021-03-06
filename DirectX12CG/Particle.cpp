#include "Particle.h"

using namespace MCB;
using namespace std;
MCB::Particle::Particle(Dx12& dx12)
{
    NORM_FRONT_VEC.vec = { 0,0,1 };
    nowFrontVec = NORM_FRONT_VEC;
}

MCB::Particle::~Particle()
{
    //delete vertex;
    constBuffTranceform->Unmap(0, nullptr);
    //vert.material.constBuffMaterialB1->Unmap(0, nullptr);
}

void Particle::Init(Dx12& dx12)
{
    D3D12_HEAP_PROPERTIES HeapProp{};
    HeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

    D3D12_RESOURCE_DESC Resdesc{};
    Resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    Resdesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
    Resdesc.Height = 1;
    Resdesc.DepthOrArraySize = 1;
    Resdesc.MipLevels = 1;
    Resdesc.SampleDesc.Count = 1;
    Resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    dx12.result = dx12.device->CreateCommittedResource
    (
        &HeapProp,        //ヒープ設定
        D3D12_HEAP_FLAG_NONE,
        &Resdesc,//リソース設定
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&constBuffTranceform)
    );

    assert(SUCCEEDED(dx12.result));

    dx12.result = constBuffTranceform->Map(0, nullptr, (void**)&constMapTranceform);
}

void Particle::Updata(View& view, Projection& projection, bool isBillBord)
{
    matWorld.SetMatScale(scale.x, scale.y, scale.z);
    matWorld.SetMatRot(rotasion.x, rotasion.y, rotasion.z, false);
    matWorld.SetMatTrans(position.x, position.y, position.z);
    if (isBillBord)
    {
        if (parent == nullptr)
        {
            matWorld.UpdataBillBordMatrixWorld(view);
        }
        else
        {
            matWorld.UpdataMatrixWorld();
        }
    }
    else
    {
        matWorld.UpdataMatrixWorld();
    }

    if (parent != nullptr)
    {
        matWorld.matWorld *= parent->matWorld.matWorld;
    }

    constMapTranceform->mat = matWorld.matWorld * view.mat * projection.mat;
}

void Particle::Draw(Dx12& dx12)
{
    //頂点データ
    dx12.commandList->IASetVertexBuffers(0, 1, &vert.vbView);
    //インデックスデータ
    //dx12.commandList->IASetIndexBuffer(&model->ibView);
    //定数バッファビュー(CBV)の設定コマンド
    dx12.commandList->SetGraphicsRootConstantBufferView(0, constBuffTranceform->GetGPUVirtualAddress());
    //描画コマンド
    dx12.commandList->DrawInstanced(_countof(vert.vertices), 1, 0, 0);

}

