#include "Texture.h"
#include "MCBFloat.h"
#include <assert.h>

using namespace MCB;
using namespace DirectX;

unsigned short int Texture::AllincrementNum = 0;

MCB::Texture::~Texture()
{

}

void MCB::Texture::CreateTexture(Dx12 dx12, const wchar_t* FileName, unsigned short int incrementNum,ShaderResource *srv)
{
    HRESULT result = texfile.LoadTexture(FileName, WIC_FLAGS_NONE);

    assert(SUCCEEDED(result) && "テクスチャ読み込みエラー");

    result = mipMap.GenerateMipMap(&texfile, TEX_FILTER_DEFAULT, 0);
    //----------------------------

    assert(SUCCEEDED(result) && "ミップマップ生成エラー");

    //画像イメージデータの作成----------------------
   texImg.SetImageDataRGBA(Float4(1.0f, 0.0f, 0.0f, 1.0f));
    //------------------------------------

     //テクスチャバッファ設定---------------------------------------
    texBuff.SetTexHeapProp(D3D12_HEAP_TYPE_CUSTOM, D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
    texBuff.SetTexResourceDesc(texfile, D3D12_RESOURCE_DIMENSION_TEXTURE2D, 1);
    //--------------------------------------


    //テクスチャバッファの生成----------------------
    result = texBuff.CommitResouce(dx12, D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr);
    texBuff.TransferMipmatToTexBuff(texfile, nullptr, result);
    //-----------------------------------

    this->incrementNum = incrementNum;

    srvptr = srv;

    SetSrvHeap(dx12);
}

void MCB::Texture::InitAllincrementNum()
{
    Texture::AllincrementNum = 0;
}

void MCB::Texture::CreateTexture(Dx12 dx12, const std::string& directoryPath, const std::string& filename, unsigned short int incrementNum, ShaderResource* srv)
{
    HRESULT result = texfile.LoadTexture(directoryPath, filename);

    assert(SUCCEEDED(result) && "テクスチャ読み込みエラー");

    result = mipMap.GenerateMipMap(&texfile, TEX_FILTER_DEFAULT, 0);
    //----------------------------

    assert(SUCCEEDED(result) && "ミップマップ生成エラー");

    //画像イメージデータの作成----------------------
    texImg.SetImageDataRGBA(Float4(1.0f, 0.0f, 0.0f, 1.0f));
    //------------------------------------

     //テクスチャバッファ設定---------------------------------------
    texBuff.SetTexHeapProp(D3D12_HEAP_TYPE_CUSTOM, D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
    texBuff.SetTexResourceDesc(texfile, D3D12_RESOURCE_DIMENSION_TEXTURE2D, 1);
    //--------------------------------------


    //テクスチャバッファの生成----------------------
    result = texBuff.CommitResouce(dx12, D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr);
    texBuff.TransferMipmatToTexBuff(texfile, nullptr, result);
    //-----------------------------------

    this->incrementNum = incrementNum;

    srvptr = srv;

    SetSrvHeap(dx12);

}

void MCB::Texture::SetSrvHeap(Dx12 dx12)
{
    if (srvptr == nullptr)
    {
        assert("SRVデスクリプタ作った後にsrvptrにポインタを渡してください。srvptrはnullptrでした。");
    }

    srvptr->SetSrvHeap(incrementNum,dx12);

    srvptr->SetSrvDesc(texBuff, D3D12_SRV_DIMENSION_TEXTURE2D);

    srvptr->SetShaderResourceView(dx12, texBuff);
}

void MCB::Texture::CreateTexture(Dx12 dx12, const wchar_t* FileName, ShaderResource* srv)
{
    HRESULT result = texfile.LoadTexture(FileName, WIC_FLAGS_NONE);

    assert(SUCCEEDED(result) && "テクスチャ読み込みエラー");

    result = mipMap.GenerateMipMap(&texfile, TEX_FILTER_DEFAULT, 0);
    //----------------------------

    assert(SUCCEEDED(result) && "ミップマップ生成エラー");

    //画像イメージデータの作成----------------------
    texImg.SetImageDataRGBA(Float4(1.0f, 0.0f, 0.0f, 1.0f));
    //------------------------------------

     //テクスチャバッファ設定---------------------------------------
    texBuff.SetTexHeapProp(D3D12_HEAP_TYPE_CUSTOM, D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
    texBuff.SetTexResourceDesc(texfile, D3D12_RESOURCE_DIMENSION_TEXTURE2D, 1);
    //--------------------------------------


    //テクスチャバッファの生成----------------------
    result = texBuff.CommitResouce(dx12, D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr);
    texBuff.TransferMipmatToTexBuff(texfile, nullptr, result);
    //-----------------------------------
    this->incrementNum = Texture::AllincrementNum;

    Texture::AllincrementNum++;
    srvptr = srv;

    SetSrvHeap(dx12);
}

void MCB::Texture::CreateTexture(Dx12 dx12, const std::string& directoryPath, const std::string& filename, ShaderResource* srv)
{
    HRESULT result = texfile.LoadTexture(directoryPath, filename);

    assert(SUCCEEDED(result) && "テクスチャ読み込みエラー");

    result = mipMap.GenerateMipMap(&texfile, TEX_FILTER_DEFAULT, 0);
    //----------------------------

    assert(SUCCEEDED(result) && "ミップマップ生成エラー");

    //画像イメージデータの作成----------------------
    texImg.SetImageDataRGBA(Float4(1.0f, 0.0f, 0.0f, 1.0f));
    //------------------------------------

     //テクスチャバッファ設定---------------------------------------
    texBuff.SetTexHeapProp(D3D12_HEAP_TYPE_CUSTOM, D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
    texBuff.SetTexResourceDesc(texfile, D3D12_RESOURCE_DIMENSION_TEXTURE2D, 1);
    //--------------------------------------


    //テクスチャバッファの生成----------------------
    result = texBuff.CommitResouce(dx12, D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr);
    texBuff.TransferMipmatToTexBuff(texfile, nullptr, result);
    //-----------------------------------

    this->incrementNum = Texture::AllincrementNum;

    Texture::AllincrementNum++;
    srvptr = srv;

    SetSrvHeap(dx12);
}
