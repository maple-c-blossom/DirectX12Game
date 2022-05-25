#include "Shader.h"
#include <d3dcompiler.h>
#include <string>
#include <cassert>

using namespace std;


void MCB::Shader::ShaderCompile(const wchar_t* shaderFileName, const char* entryPoint, unsigned int ShaderName)
{
    HRESULT result;

    switch (ShaderName)
    {
    case VS:
        result = D3DCompileFromFile(
            shaderFileName,  // シェーダファイル名
            nullptr,
            D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
            entryPoint, "vs_5_0", // エントリーポイント名、シェーダーモデル指定
            D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
            0,
            &vsBlob, &errorBlob);

        if (FAILED(result)) {
            // errorBlobからエラー内容をstring型にコピー
            std::string error;
            error.resize(errorBlob->GetBufferSize());

            copy_n((char*)errorBlob->GetBufferPointer(),
                errorBlob->GetBufferSize(),
                error.begin());
            error += "\n";
            // エラー内容を出力ウィンドウに表示
            OutputDebugStringA(error.c_str());
            assert(0 && "VSコンパイルエラー");
        }
        break;

    case PS:
        result = D3DCompileFromFile(
            shaderFileName,  // シェーダファイル名
            nullptr,
            D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
            entryPoint, "ps_5_0", // エントリーポイント名、シェーダーモデル指定
            D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
            0,
            &psBlob, &errorBlob);

        if (FAILED(result)) {
            // errorBlobからエラー内容をstring型にコピー
            std::string error;
            error.resize(errorBlob->GetBufferSize());

            copy_n((char*)errorBlob->GetBufferPointer(),
                errorBlob->GetBufferSize(),
                error.begin());
            error += "\n";
            // エラー内容を出力ウィンドウに表示
            OutputDebugStringA(error.c_str());
            assert(0 && "PSコンパイルエラー");
        }
        break;
    case GS:
        result = D3DCompileFromFile(
            shaderFileName,  // シェーダファイル名
            nullptr,
            D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
            entryPoint, "gs_5_0", // エントリーポイント名、シェーダーモデル指定
            D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
            0,
            &gsBlob, &errorBlob);

        if (FAILED(result)) {
            // errorBlobからエラー内容をstring型にコピー
            std::string error;
            error.resize(errorBlob->GetBufferSize());

            copy_n((char*)errorBlob->GetBufferPointer(),
                errorBlob->GetBufferSize(),
                error.begin());
            error += "\n";
            // エラー内容を出力ウィンドウに表示
            OutputDebugStringA(error.c_str());
            assert(0 && "GSコンパイルエラー");
        }
        break;
    default:
        break;
    }
    



}
//
//void MCB::Shader::SetInputLayout(const char* semanticName, DXGI_FORMAT format, unsigned int semanticIndex, unsigned int inputSlot, unsigned int AlignedByteOffset, D3D12_INPUT_CLASSIFICATION InputSlotClass, unsigned int InstanceDataStepRate)
//{
//    D3D12_INPUT_ELEMENT_DESC temp = { semanticName,semanticIndex,format,inputSlot,AlignedByteOffset,InputSlotClass,InstanceDataStepRate };
//
//    inputLayout.push_back(temp);
//}
//
//void MCB::Shader::SetInputLayout(const char* semanticName, DXGI_FORMAT format)
//{
//    D3D12_INPUT_ELEMENT_DESC temp = { semanticName,0,format,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0};
//
//    inputLayout.push_back(temp);
//}
