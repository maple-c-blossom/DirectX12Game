#pragma once
#include <DirectXMath.h>
#include <Windows.h>
#include <wrl.h>
#include "Dx12.h"
#include <d3d12.h>
#include "ObjectMaterial.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include "MCBFloat.h"
#include "TextureFile.h"

namespace MCB
{
    //���_�f�[�^�\����-------------------------------------
    typedef struct ObjectVertex
    {
        Float3 pos;//xyz���W
        Float3 normal;//�@���x�N�g��
        Float2 uv;//uv���W
    };
    //--------------------------------------
    class Model
    {
    public:

        ~Model();

        Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff = nullptr;

        Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;

        uint16_t* indexMap = nullptr;

        ObjectVertex* vertMap = nullptr;


        TextureFile texture;

        std::vector<ObjectVertex> vertices;
        std::vector<unsigned short> indices;

        unsigned int sizeVB = static_cast<unsigned int>(sizeof(ObjectVertex) * vertices.size());
        unsigned int sizeIB = static_cast<unsigned int>(sizeof(unsigned short) * indices.size());

        D3D12_INDEX_BUFFER_VIEW ibView{};

        D3D12_VERTEX_BUFFER_VIEW vbView{};

        //D3D12_HEAP_PROPERTIES heapprop{};   // �q�[�v�ݒ�

        //D3D12_RESOURCE_DESC resdesc{};  // ���\�[�X�ݒ�

        ObjectMaterial material;

        void CreateVertexBuffer(Dx12& dx12, const D3D12_HEAP_PROPERTIES& HeapProp, D3D12_HEAP_FLAGS flag, const D3D12_RESOURCE_DESC Resdesc, D3D12_RESOURCE_STATES state);

        void SetIbView(DXGI_FORMAT format);

        void SetVbView();

        void CreateIndexBuffer(Dx12& dx12, const D3D12_HEAP_PROPERTIES& HeapProp, D3D12_HEAP_FLAGS flag, const D3D12_RESOURCE_DESC Resdesc, D3D12_RESOURCE_STATES state);

        HRESULT IndexMaping();

        HRESULT VertexMaping();

        void CreateModel(const std::string fileName);

        void SetSizeIB();

        void SetSizeVB();

        void LoadMaterial( const std::string& directoryPath,const std::string& filename);


    };

}


