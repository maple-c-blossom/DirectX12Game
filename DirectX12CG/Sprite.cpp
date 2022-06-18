#include "Sprite.h"

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

    assert(SUCCEEDED(result) && "Sprite�������̒��_�o�b�t�@CommittedResource�G���[");

    SpriteVertex* vertexMap = nullptr;
    result = tempSprite.vertBuff->Map(0, nullptr, (void**)&vertexMap);
    assert(SUCCEEDED(result) && "Sprite��������vertBuff->Map�G���[");
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

    assert(SUCCEEDED(result) && "Sprite�������̒萔�o�b�t�@��CommittedResource�G���[");
    SpriteConstBufferDataTransform* constMap = nullptr;
    result = tempSprite.constBuff->Map(0, nullptr, (void**)&constMap);
    assert(SUCCEEDED(result) && "Sprite��������constBuff->Map�G���[");
    Float4 tempcolor; tempcolor.x = 1; tempcolor.y = 1;tempcolor.z = 1;tempcolor.w = 1;
    constMap->color = tempcolor;
    constMap->mat = DirectX::XMMatrixOrthographicOffCenterLH(
        0.0f, dxWindow.window_width, dxWindow.window_height, 0.0f, 0.0f, 1.0f);
    tempSprite.constBuff->Unmap(0,nullptr);


    return tempSprite;
}

void MCB::Sprite::SpriteCommonBeginDraw(Dx12& dx12, const PipelineRootSignature& pipeline, ShaderResource& descHeap)
{
    dx12.commandList->SetPipelineState(pipeline.pipeline.pipelinestate.Get());
    dx12.commandList->SetGraphicsRootSignature(pipeline.rootsignature.rootsignature.Get());


    //�v���~�e�B�u�`��̐ݒ�R�}���h�i�O�p�`���X�g�j--------------------------
    dx12.commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    //SRV�q�[�v�̐ݒ�R�}���h
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> ppHeaps[] = { descHeap.srvHeap };
    dx12.commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps->GetAddressOf());
}

void MCB::Sprite::SpriteDraw(const Sprite& sprite, Dx12& dx12, ShaderResource descriptor, Texture& tex)
{
    //SRV�q�[�v�̐擪�A�h���X���擾
    D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descriptor.srvHeap->GetGPUDescriptorHandleForHeapStart();


    srvGpuHandle.ptr += tex.incrementNum * dx12.device.Get()->GetDescriptorHandleIncrementSize(descriptor.srvHeapDesc.Type);

    //SRV�q�[�v�̐擪�ɂ���SRV���p�����[�^1�Ԃɐݒ�
    dx12.commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

    //���_�f�[�^
    dx12.commandList->IASetVertexBuffers(0, 1, &vbView);
    //�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
    dx12.commandList->SetGraphicsRootConstantBufferView(0,sprite.constBuff->GetGPUVirtualAddress());
    //�`��R�}���h
    dx12.commandList->DrawInstanced(4,1,0,0);

}
