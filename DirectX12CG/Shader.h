#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <vector>

namespace MCB
{
    enum ShaderNames
    {
        VS,
        PS,
        GS,
    };
    class Shader
    {
    public:
        Microsoft::WRL::ComPtr<ID3DBlob> vsBlob = nullptr; // 頂点シェーダオブジェクト
        Microsoft::WRL::ComPtr<ID3DBlob> gsBlob = nullptr; // ジオメトリシェーダオブジェクト
        Microsoft::WRL::ComPtr<ID3DBlob> psBlob = nullptr; // ピクセルシェーダオブジェクト
        Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr; // エラーオブジェクト

       /* std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;*/

      // 頂点レイアウト------------------
#pragma region 頂点レイアウト

        D3D12_INPUT_ELEMENT_DESC inputLayout[3] =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}, // (1行で書いたほうが見やすい)
            {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},//法線ベクトル
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}//uv座標
        };


        D3D12_INPUT_ELEMENT_DESC SpriteinputLayout[2] =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}, // (1行で書いたほうが見やすい)
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}//uv座標
        };


#pragma endregion 頂点レイアウト
     //--------------------


        void ShaderCompile(const wchar_t* shaderFileName, const char* entryPoint,unsigned int ShaderName);

        /*void SetInputLayout(const char* semanticName, DXGI_FORMAT format);

        void SetInputLayout(const char* semanticName, DXGI_FORMAT format, unsigned int semanticIndex,  unsigned int inputSlot, 
            unsigned int AlignedByteOffset, D3D12_INPUT_CLASSIFICATION InputSlotClass, unsigned int InstanceDataStepRate);*/

    };

}

