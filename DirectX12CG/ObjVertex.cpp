#include "ObjVertex.h"
void MCB::ObjVertex::CreateVertexBuffer(Dx12& dx12, const D3D12_HEAP_PROPERTIES& HeapProp, D3D12_HEAP_FLAGS flag, const D3D12_RESOURCE_DESC Resdesc, D3D12_RESOURCE_STATES state)
{
    dx12.result = dx12.device->CreateCommittedResource(
        &HeapProp, // �q�[�v�ݒ�
        flag,
        &Resdesc, // ���\�[�X�ݒ�
        state,
        nullptr,
        IID_PPV_ARGS(&vertBuff));
    assert(SUCCEEDED(dx12.result));
}

void MCB::ObjVertex::SetIbView(DXGI_FORMAT format)
{
    ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
    ibView.Format = format;
    ibView.SizeInBytes = sizeIB;
}

void MCB::ObjVertex::CreateIndexBuffer(Dx12& dx12, const D3D12_HEAP_PROPERTIES& HeapProp, D3D12_HEAP_FLAGS flag, const D3D12_RESOURCE_DESC Resdesc, D3D12_RESOURCE_STATES state)
{
    dx12.result = dx12.device->CreateCommittedResource(
        &HeapProp,
        flag,
        &Resdesc,
        state,
        nullptr,
        IID_PPV_ARGS(&indexBuff)
    );

}

HRESULT MCB::ObjVertex::IndexMaping()
{
    HRESULT result = S_OK;
    ////GPU��̃o�b�t�@�ɑΉ��������z���������擾----------------------------
    //result = indexBuff->Map(0, nullptr, (void**)&indexMap);
    ////---------------------------------------

    ////�S�C���f�b�N�X�ɑ΂���-------------------------
    //for (int i = 0; i < _countof(boxIndices); i++)
    //{
    //    indexMap[i] = boxIndices[i];
    //}
    ////-----------------------

    ////�q���������---------------------
    //indexBuff->Unmap(0, nullptr);
    ////------------------------

    return result;
}

HRESULT MCB::ObjVertex::VertexMaping()
{
    HRESULT result = S_OK;

    //result = vertBuff->Map(0, nullptr, (void**)&vertMap);
    //assert(SUCCEEDED(result));

    //// �S���_�ɑ΂���
    //for (int i = 0; i < _countof(Box); i++)
    //{
    //    vertMap[i] = Box[i];   // ���W���R�s�[
    //}

    //// �}�b�v������
    //vertBuff->Unmap(0, nullptr);

    return result;
}
