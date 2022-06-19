#include "Sprite.h"
#include "Util.h"
using namespace MCB;
using namespace DirectX;

DirectX::XMMATRIX Sprite::matProje{};

void MCB::Sprite::SpriteTransferVertexBuffer(const Sprite& sprite)
{
    HRESULT result = S_FALSE;

    SpriteVertex vertices[] =
    {
        {{},{0.0f,1.0f}},
        {{},{0.0f,0.0f}},
        {{},{1.0f,1.0f}},
        {{},{1.0f,0.0f}},
    };

    float left = (0.0f - sprite.anchorPoint.x) * sprite.size.x;
    float right = (1.0f - sprite.anchorPoint.x) * sprite.size.x;
    float top = (0.0f - sprite.anchorPoint.y) * sprite.size.y;
    float bottom = (1.0f - sprite.anchorPoint.y) * sprite.size.y;


    enum { LB, LT, RB, RT };

    vertices[LB].pos = { left,bottom,0.0f };
    vertices[LT].pos = { left,top,0.0f };
    vertices[RB].pos = { right,bottom,0.0f };
    vertices[RT].pos = { right,top,0.0f };


    SpriteVertex* vertexMap = nullptr;
    result = sprite.vertBuff->Map(0, nullptr, (void**)&vertexMap);
    assert(SUCCEEDED(result) && "SpriteTransferVertexBuffer時のvertBuff->Mapエラー");
    memcpy(vertexMap, vertices, sizeof(vertices));
    sprite.vertBuff->Unmap(0, nullptr);

}

void MCB::Sprite::SpriteUpdate(Sprite& sprite)
{
    HRESULT result = S_FALSE;
    sprite.matWorld = DirectX::XMMatrixIdentity();
    sprite.matWorld *= DirectX::XMMatrixRotationZ(ConvertRadius(sprite.rotation));
    sprite.matWorld *= XMMatrixTranslation(sprite.position.x, sprite.position.y, sprite.position.z);

    SpriteConstBufferDataTransform* constMap = nullptr;
    result = sprite.constBuff->Map(0, nullptr, (void**)&constMap);
    assert(SUCCEEDED(result) && "SpriteUpdate時のconstBuff->Mapエラー");
    constMap->color = sprite.color;
    constMap->mat = sprite.matWorld * Sprite::matProje;

    sprite.constBuff->Unmap(0, nullptr);

}

void MCB::Sprite::InitMatProje(DxWindow& dxWindow)
{
    Sprite::matProje = DirectX::XMMatrixOrthographicOffCenterLH(
        0.0f, dxWindow.window_width, dxWindow.window_height, 0.0f, 0.0f, 1.0f);

}

MCB::Sprite MCB::Sprite::CreateSprite(Dx12& dx12, DxWindow& dxWindow)
{
    HRESULT result = S_FALSE;

    Sprite tempSprite = {};


    SpriteVertex vertices[] =
    {
        {{0.0f,100.0f,0.0f},{0.0f,1.0f}},
        {{0.0f ,0.0f ,0.0f},{0.0f,0.0f}},
        {{100.0f,100.0f,0.0f},{1.0f,1.0f}},
        {{100.0f,0.0f, 0.0f},{1.0f,0.0f}}
    };

    tempSprite.HeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;


    tempSprite.Resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    tempSprite.Resdesc.Width = (sizeof(vertices));
    tempSprite.Resdesc.Height = 1;
    tempSprite.Resdesc.DepthOrArraySize = 1;
    tempSprite.Resdesc.MipLevels = 1;
    tempSprite.Resdesc.SampleDesc.Count = 1;
    tempSprite.Resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    result = dx12.device.Get()->CreateCommittedResource(
        &tempSprite.HeapProp,
        D3D12_HEAP_FLAG_NONE,
        &tempSprite.Resdesc,
        D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&tempSprite.vertBuff));

    assert(SUCCEEDED(result) && "Sprite生成時の頂点バッファCommittedResourceエラー");

    float left = (0.0f - tempSprite.anchorPoint.x) * tempSprite.size.x;
    float right = (1.0f - tempSprite.anchorPoint.x) * tempSprite.size.x;
    float top = (0.0f - tempSprite.anchorPoint.y) * tempSprite.size.y;
    float bottom = (1.0f - tempSprite.anchorPoint.y) * tempSprite.size.y;


    enum { LB, LT, RB, RT };

    vertices[LB].pos = { left,bottom,0.0f };
    vertices[LT].pos = { left,top,0.0f };
    vertices[RB].pos = { right,bottom,0.0f };
    vertices[RT].pos = { right,top,0.0f };


    SpriteVertex* vertexMap = nullptr;
    result = tempSprite.vertBuff->Map(0, nullptr, (void**)&vertexMap);
    assert(SUCCEEDED(result) && "Sprite生成時のvertBuff->Mapエラー");
    memcpy(vertexMap, vertices, sizeof(vertices));
    tempSprite.vertBuff->Unmap(0, nullptr);

    tempSprite.vbView.BufferLocation = tempSprite.vertBuff->GetGPUVirtualAddress();
    tempSprite.vbView.SizeInBytes = sizeof(vertices);
    tempSprite.vbView.StrideInBytes = sizeof(vertices[0]);

    D3D12_HEAP_PROPERTIES constHeapProp{};
    constHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

    D3D12_RESOURCE_DESC constResdesc{};
    constResdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    constResdesc.Width = (sizeof(SpriteConstBufferDataTransform) + 0xff) & ~0xff;
    constResdesc.Height = 1;
    constResdesc.DepthOrArraySize = 1;
    constResdesc.MipLevels = 1;
    constResdesc.SampleDesc.Count = 1;
    constResdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;


    result = dx12.device.Get()->CreateCommittedResource(
        &constHeapProp,
        D3D12_HEAP_FLAG_NONE,
        &constResdesc,
        D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&tempSprite.constBuff)
    );

    assert(SUCCEEDED(result) && "Sprite生成時の定数バッファのCommittedResourceエラー");
    SpriteConstBufferDataTransform* constMap = nullptr;
    result = tempSprite.constBuff->Map(0, nullptr, (void**)&constMap);
    assert(SUCCEEDED(result) && "Sprite生成時のconstBuff->Mapエラー");
    Float4 tempcolor; tempcolor.x = 1; tempcolor.y = 1;tempcolor.z = 1;tempcolor.w = 1;
    constMap->color = tempcolor;
    constMap->mat = Sprite::matProje;
    tempSprite.constBuff->Unmap(0,nullptr);


    return tempSprite;
}

void MCB::Sprite::SpriteCommonBeginDraw(Dx12& dx12, const PipelineRootSignature& pipeline, ShaderResource& descHeap)
{
    dx12.commandList->SetPipelineState(pipeline.pipeline.pipelinestate.Get());
    dx12.commandList->SetGraphicsRootSignature(pipeline.rootsignature.rootsignature.Get());


    //プリミティブ形状の設定コマンド（三角形リスト）--------------------------
    dx12.commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    //SRVヒープの設定コマンド
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> ppHeaps[] = { descHeap.srvHeap };
    dx12.commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps->GetAddressOf());
}


void MCB::Sprite::SpriteDraw(Sprite& sprite, Dx12& dx12, ShaderResource descriptor, Texture& tex, float positionX, float positionY,
                            float size_x, float size_y)
{
    Sprite tempsprite = sprite;

    Float2 size;
    Float2 anchorPoint;

    size.x = size_x;
    size.y = size_y;

    tempsprite.position.x = positionX;
    tempsprite.position.y = positionY;
    tempsprite.position.z = 0;

    tempsprite.SpriteUpdate(tempsprite);

    if (size.x == 0 || size.y == 0)
    {
        D3D12_RESOURCE_DESC resdesc = tex.texBuff.texbuff->GetDesc();

        if (size.x == 0)
        {
            tempsprite.size.x = (float)resdesc.Width;
        }

        if (size.y == 0)
        {
            tempsprite.size.y = (float)resdesc.Height;
        }
    }
    
    if(size.x != 0 || size.y != 0)
    {
        if (size.x != 0)
        {
            tempsprite.size.x = size.x;
        }

        if (size.y != 0)
        {
            tempsprite.size.y = size.y;
        }
    }



    if (tempsprite.size.x != sprite.size.x || tempsprite.size.y != sprite.size.y)
    {
        tempsprite.SpriteTransferVertexBuffer(tempsprite);
        sprite.size = tempsprite.size;
    }

    //SRVヒープの先頭アドレスを取得
    D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descriptor.srvHeap->GetGPUDescriptorHandleForHeapStart();


    srvGpuHandle.ptr += tex.incrementNum * dx12.device.Get()->GetDescriptorHandleIncrementSize(descriptor.srvHeapDesc.Type);

    //SRVヒープの先頭にあるSRVをパラメータ1番に設定
    dx12.commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

    //頂点データ
    dx12.commandList->IASetVertexBuffers(0, 1, &vbView);
    //定数バッファビュー(CBV)の設定コマンド
    dx12.commandList->SetGraphicsRootConstantBufferView(0, tempsprite.constBuff->GetGPUVirtualAddress());
    //描画コマンド
    dx12.commandList->DrawInstanced(4,1,0,0);

}

void MCB::Sprite::SpriteFlipDraw(Sprite& sprite, Dx12& dx12, ShaderResource descriptor, Texture& tex, bool isflipX, bool isflipY)
{
    Sprite tempSprite = sprite;

    tempSprite.isFlipX = isflipX;
    tempSprite.isFlipY = isflipY;

    //SRVヒープの先頭アドレスを取得
    D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descriptor.srvHeap->GetGPUDescriptorHandleForHeapStart();


    srvGpuHandle.ptr += tex.incrementNum * dx12.device.Get()->GetDescriptorHandleIncrementSize(descriptor.srvHeapDesc.Type);

    //SRVヒープの先頭にあるSRVをパラメータ1番に設定
    dx12.commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

    //頂点データ
    dx12.commandList->IASetVertexBuffers(0, 1, &vbView);
    //定数バッファビュー(CBV)の設定コマンド
    dx12.commandList->SetGraphicsRootConstantBufferView(0, sprite.constBuff->GetGPUVirtualAddress());
    //描画コマンド
    dx12.commandList->DrawInstanced(4, 1, 0, 0);

}
