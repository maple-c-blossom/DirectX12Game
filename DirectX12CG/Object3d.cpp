#include "Object3d.h"

using namespace MCB;
using namespace std;
MCB::Object3d::Object3d(Dx12& dx12)
{
    NORM_FRONT_VEC.vec = { 0,0,1 };
    nowFrontVec = NORM_FRONT_VEC;
    //model->material.Init(dx12);
    this->Init(dx12);
}

MCB::Object3d::Object3d()
{
}

MCB::Object3d::~Object3d()
{
    //delete vertex;
    constBuffTranceform->Unmap(0, nullptr);
    //model->texture.scratchImg.Release();
    //delete model;
}

void Object3d::Init(Dx12& dx12)
{
    NORM_FRONT_VEC.vec = { 0,0,1 };
    nowFrontVec = NORM_FRONT_VEC;

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

void Object3d::Updata(View& view, Projection& projection,bool isBillBord)
{
    matWorld.SetMatScale(scale.x, scale.y, scale.z);
    matWorld.SetMatRot(rotasion.x, rotasion.y, rotasion.z,false);
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

void Object3d::Updata(View& view, Projection& projection,Quaternion q, bool isBillBord)
{
    MCBMatrix matRot;
    matRot.MCBMatrixIdentity();
    matWorld.SetMatScale(scale.x, scale.y, scale.z);
    matWorld.matRot = matRot.MatrixConvertXMMatrix(q.GetQuaternionRotaMat(q));
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

void Object3d::Draw(Dx12 dx12, ShaderResource descriptor)
{
    //定数バッファビュー(CBV)の設定コマンド
    dx12.commandList->SetGraphicsRootConstantBufferView(2, model->material.constBuffMaterialB1->GetGPUVirtualAddress());

    //SRVヒープの先頭アドレスを取得
    D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descriptor.srvHeap->GetGPUDescriptorHandleForHeapStart();


    srvGpuHandle.ptr += model->texture.incrementNum * dx12.device.Get()->GetDescriptorHandleIncrementSize(descriptor.srvHeapDesc.Type);

    //SRVヒープの先頭にあるSRVをパラメータ1番に設定
    dx12.commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

    //頂点データ
    dx12.commandList->IASetVertexBuffers(0, 1, &model->vbView);
    //インデックスデータ
    dx12.commandList->IASetIndexBuffer(&model->ibView);
    //定数バッファビュー(CBV)の設定コマンド
    dx12.commandList->SetGraphicsRootConstantBufferView(0, constBuffTranceform->GetGPUVirtualAddress());
    //描画コマンド
    dx12.commandList->DrawIndexedInstanced((unsigned int) model->indices.size(), 1, 0, 0, 0);

}

void Object3d::Draw(Dx12 dx12, ShaderResource descriptor, unsigned short int incremant)
{
    //定数バッファビュー(CBV)の設定コマンド
    dx12.commandList->SetGraphicsRootConstantBufferView(2, model->material.constBuffMaterialB1->GetGPUVirtualAddress());

    //SRVヒープの先頭アドレスを取得
    D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descriptor.srvHeap->GetGPUDescriptorHandleForHeapStart();


    srvGpuHandle.ptr += incremant * dx12.device.Get()->GetDescriptorHandleIncrementSize(descriptor.srvHeapDesc.Type);

    //SRVヒープの先頭にあるSRVをパラメータ1番に設定
    dx12.commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

    //頂点データ
    dx12.commandList->IASetVertexBuffers(0, 1, &model->vbView);
    //インデックスデータ
    dx12.commandList->IASetIndexBuffer(&model->ibView);
    //定数バッファビュー(CBV)の設定コマンド
    dx12.commandList->SetGraphicsRootConstantBufferView(0, constBuffTranceform->GetGPUVirtualAddress());
    //描画コマンド
    dx12.commandList->DrawIndexedInstanced((unsigned int)model->indices.size(), 1, 0, 0, 0);

}
