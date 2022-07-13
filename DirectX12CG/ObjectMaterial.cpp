#include "ObjectMaterial.h"

using namespace MCB;
using namespace DirectX;

MCB::ObjectMaterial::~ObjectMaterial()
{
    constBuffMaterialB1->Unmap(0, nullptr);
}

void ObjectMaterial::Init()
{
    Dx12* dx12 = Dx12::GetInstance();
    HeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

 
    Resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    Resdesc.Width = (sizeof(ConstBufferDataMaterialB1) + 0xff) & ~0xff;
    Resdesc.Height = 1;
    Resdesc.DepthOrArraySize = 1;
    Resdesc.MipLevels = 1;
    Resdesc.SampleDesc.Count = 1;
    Resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    dx12->result = dx12->device->CreateCommittedResource
    (
        &HeapProp,        //ヒープ設定
        D3D12_HEAP_FLAG_NONE,
        &Resdesc,//リソース設定
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&constBuffMaterialB1)
    );
    assert(SUCCEEDED(dx12->result));

    dx12->result = constBuffMaterialB1->Map(0, nullptr, (void**)&constMapMaterial);

    assert(SUCCEEDED(dx12->result));

    constMapMaterial->ambient = material.ambient;
    constMapMaterial->diffuse = material.diffuse;
    constMapMaterial->specular = material.specular;
    constMapMaterial->alpha = material.alpha;
    constMapMaterial->color = material.color;

    //constMapMaterial->color = XMFLOAT4(1, 1, 1, 1.0f);
}

void MCB::ObjectMaterial::SetIndex(D3D12_RESOURCE_DIMENSION dimension, UINT sizeIB, int height, int DepthOrArraySize, int MipLevels, int SampleDescCount, D3D12_TEXTURE_LAYOUT layput)
{
    Resdesc.Dimension = dimension;
    Resdesc.Width = sizeIB;
    Resdesc.Height = height;
    Resdesc.DepthOrArraySize = DepthOrArraySize;
    Resdesc.MipLevels = MipLevels;
    Resdesc.SampleDesc.Count = SampleDescCount;
    Resdesc.Layout = layput;
}

void MCB::ObjectMaterial::Update()
{
    constMapMaterial->ambient = material.ambient;
    constMapMaterial->diffuse = material.diffuse;
    constMapMaterial->specular = material.specular;
    constMapMaterial->alpha = material.alpha;
    constMapMaterial->color = material.color;
}

void MCB::ObjectMaterial::SetVertexBuffer(D3D12_HEAP_TYPE heaptype, D3D12_RESOURCE_DIMENSION dimension, UINT sizeVB, int height, int DepthOrArraySize, int MipLevels, int SampleDescCount, D3D12_TEXTURE_LAYOUT layput)
{
    HeapProp.Type = heaptype; // GPUへの転送用

    Resdesc.Dimension = dimension;
    Resdesc.Width = sizeVB; // 頂点データ全体のサイズ
    Resdesc.Height = height;
    Resdesc.DepthOrArraySize = DepthOrArraySize;
    Resdesc.MipLevels = MipLevels;
    Resdesc.SampleDesc.Count = SampleDescCount;
    Resdesc.Layout = layput;

}
