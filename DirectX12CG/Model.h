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
#include "Texture.h"

namespace MCB
{
    //頂点データ構造体-------------------------------------
    typedef struct ObjectVertex
    {
        Float3 pos;//xyz座標
        Float3 normal;//法線ベクトル
        Float2 uv;//uv座標
    };
    //--------------------------------------
    class Model
    {
    public:

        Model(Dx12 dx, const std::string fileName, ShaderResource* srv);

        ~Model();

        Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff = nullptr;

        Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;




        Texture texture;

        std::vector<ObjectVertex> vertices;
        std::vector<unsigned short> indices;

        unsigned int sizeVB = static_cast<unsigned int>(sizeof(ObjectVertex) * vertices.size());
        unsigned int sizeIB = static_cast<unsigned int>(sizeof(unsigned short) * indices.size());

        D3D12_INDEX_BUFFER_VIEW ibView{};

        D3D12_VERTEX_BUFFER_VIEW vbView{};

        //D3D12_HEAP_PROPERTIES heapprop{};   // ヒープ設定

        //D3D12_RESOURCE_DESC resdesc{};  // リソース設定

        ObjectMaterial material;

        void CreateVertexBuffer(Dx12& dx12, const D3D12_HEAP_PROPERTIES& HeapProp, D3D12_HEAP_FLAGS flag, const D3D12_RESOURCE_DESC Resdesc, D3D12_RESOURCE_STATES state);

        void SetIbView(DXGI_FORMAT format);

        void SetVbView();

        void CreateIndexBuffer(Dx12& dx12, const D3D12_HEAP_PROPERTIES& HeapProp, D3D12_HEAP_FLAGS flag, const D3D12_RESOURCE_DESC Resdesc, D3D12_RESOURCE_STATES state);

        HRESULT IndexMaping();

        HRESULT VertexMaping();

        void CreateModel(Dx12 dx,const std::string fileName, ShaderResource* srv);

        void SetSizeIB();

        void SetSizeVB();

        void LoadMaterial(Dx12 dx12, const std::string& directoryPath,const std::string& filename, ShaderResource* srv);

        void Init(Dx12 dx12, const std::string fileName, ShaderResource* srv);
        
    };

}


