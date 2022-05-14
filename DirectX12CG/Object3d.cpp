#include "Object3d.h"

using namespace MCB;
using namespace std;
MCB::Object3d::Object3d()
{
    NORM_FRONT_VEC.vec = { 0,0,1 };
    nowFrontVec = NORM_FRONT_VEC;
}

MCB::Object3d::~Object3d()
{
    delete vertex;
}

void Object3d::Init(Dx12& dx12)
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
        &HeapProp,        //�q�[�v�ݒ�
        D3D12_HEAP_FLAG_NONE,
        &Resdesc,//���\�[�X�ݒ�
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

void Object3d::Draw(Dx12 dx12, D3D12_VERTEX_BUFFER_VIEW& vbView, D3D12_INDEX_BUFFER_VIEW& ibView, UINT numIndices)
{
    //���_�f�[�^
    dx12.commandList->IASetVertexBuffers(0, 1, &vbView);
    //�C���f�b�N�X�f�[�^
    dx12.commandList->IASetIndexBuffer(&ibView);
    //�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
    dx12.commandList->SetGraphicsRootConstantBufferView(2, constBuffTranceform->GetGPUVirtualAddress());
    //�`��R�}���h
    dx12.commandList->DrawIndexedInstanced(numIndices, 1, 0, 0, 0);

}

void MCB::Object3d::CreateModel(const char* fileName)
{
    std::ifstream file;

    std::vector<Float3> position;//���_���W
    std::vector<Float3> normals;//�@���x�N�g��
    std::vector<Float3> texcoords;//�e�N�X�`��UV

    
    

    file.open(fileName);
    if (file.fail())
    {
        assert(0);
    }


    string line;
    while (getline(file, line))
    {
        std::istringstream line_stream(line);

        string key;
        getline(line_stream, key, ' ');



    }

    file.close();

}
