#include "PIpelineRootSignature.h"
using namespace MCB;

PipelineRootSignature MCB::PipelineRootSignature::Create3DObjectPipeline(Dx12& dx12, Depth& depth, RootParameter& rootparams)
{
    PipelineRootSignature pipelinerootsognature;

    pipelinerootsognature.texSample.Init();

    // 頂点シェーダの読み込みとコンパイル--------------------------------
#pragma region 頂点シェーダの読み込みとコンパイル

    pipelinerootsognature.shader.ShaderCompile(L"Resources\\Shaders\\OBJShader\\OBJVertexShader.hlsl", "main", VS);

#pragma endregion 頂点シェーダの読み込みとコンパイル
    //------------------------------------------

     //ジオメトリシェーダの読み込みとコンパイル---------------
#pragma region ジオメトリシェーダの読み込みとコンパイル

    pipelinerootsognature.shader.ShaderCompile(L"Resources\\Shaders\\OBJShader\\OBJGeometryShader.hlsl", "main", GS);

#pragma endregion ジオメトリシェーダの読み込みとコンパイル
    //---------------------------------


    // ピクセルシェーダの読み込みとコンパイル-------------------------------
#pragma region ピクセルシェーダの読み込みとコンパイル

    pipelinerootsognature.shader.ShaderCompile(L"Resources\\Shaders\\OBJShader\\OBJPixelShader.hlsl", "main", PS);

#pragma endregion ピクセルシェーダの読み込みとコンパイル
    //--------------------------------

#pragma region 頂点シェーダとピクセルシェーダをパイプラインに設定

    pipelinerootsognature.pipeline.SetGpipleneDescAll(&pipelinerootsognature.shader);

#pragma endregion 頂点シェーダとピクセルシェーダをパイプラインに設定
    //-----------------------------------

    //サンプルマスクとラスタライザステートの設定------------------------------------
#pragma region サンプルマスクとラスタライザステートの設定
    pipelinerootsognature.pipeline.SetSampleMask();

    pipelinerootsognature.pipeline.SetAllAddRasterizerState();
#pragma endregion サンプルマスクとラスタライザステートの設定
    //------------------------------------


     //ブレンドステートの設定-------------------------------
#pragma region ブレンドステートの設定

    pipelinerootsognature.pipeline.SetRenderTaegetBlendDesc(pipelinerootsognature.pipeline.pipelineDesc.BlendState.RenderTarget[0]);

    pipelinerootsognature.pipeline.SetRenderTargetWriteMask();

    pipelinerootsognature.pipeline.SetNormalBlendDesc();

    pipelinerootsognature.pipeline.SetAlphaBlend();


#pragma endregion ブレンドステートの設定
    //--------------------------

    //頂点レイアウトの設定------------------
#pragma region 頂点レイアウトの設定

    pipelinerootsognature.pipeline.pipelineDesc.InputLayout.pInputElementDescs = pipelinerootsognature. shader.inputLayout;
    pipelinerootsognature.pipeline.pipelineDesc.InputLayout.NumElements = _countof(pipelinerootsognature.shader.inputLayout);

#pragma endregion 頂点レイアウトの設定
    //----------------------------

    //図形の形状を三角形に設定-------------------------
    pipelinerootsognature.pipeline.SetPrimitiveTopologyType();
    //------------------

    //その他の設定----------------
#pragma region その他の設定

    pipelinerootsognature.pipeline.SetNumRenderTargets();
    pipelinerootsognature.pipeline.SetRTVFormats();
    pipelinerootsognature.pipeline.SetSampleDescCount();

#pragma endregion その他の設定
    //----------------

    depth.SetDepthStencilState(pipelinerootsognature.pipeline.pipelineDesc);

    //ルートシグネチャの生成--------------------------
#pragma region ルートシグネチャの生成


    pipelinerootsognature.rootsignature.InitRootSignatureDesc(rootparams, pipelinerootsognature.texSample);

    pipelinerootsognature.rootsignature.SetSerializeRootSignature(pipelinerootsognature.shader, dx12);

    pipelinerootsognature.rootsignature.CreateRootSignature(&dx12);

    // パイプラインにルートシグネチャをセット

    pipelinerootsognature.pipeline.SetRootSignature(pipelinerootsognature.rootsignature);

#pragma endregion ルートシグネチャの生成
    //--------------------------------

   //パイプラインステートの生成-------------------------
#pragma region パイプラインステートの生成

    pipelinerootsognature.pipeline.CreateGraphicsPipelineState(&dx12);

#pragma endregion パイプラインステートの生成
    //-----------------------------


	return pipelinerootsognature;
}

PipelineRootSignature MCB::PipelineRootSignature::CreateSpritePipeline(Dx12& dx12, Depth& depth, RootParameter& rootparams)
{

    PipelineRootSignature pipelinerootsognature;

    pipelinerootsognature.texSample.Init();

    // 頂点シェーダの読み込みとコンパイル--------------------------------
#pragma region 頂点シェーダの読み込みとコンパイル

    pipelinerootsognature.shader.ShaderCompile(L"Resources\\Shaders\\SpriteShader\\SpriteVertexShader.hlsl", "main", VS);

#pragma endregion 頂点シェーダの読み込みとコンパイル
    //------------------------------------------

    // ピクセルシェーダの読み込みとコンパイル-------------------------------
#pragma region ピクセルシェーダの読み込みとコンパイル

    pipelinerootsognature.shader.ShaderCompile(L"Resources\\Shaders\\SpriteShader\\SpritePixelShader.hlsl", "main", PS);

#pragma endregion ピクセルシェーダの読み込みとコンパイル
    //--------------------------------

#pragma region 頂点シェーダとピクセルシェーダをパイプラインに設定

    pipelinerootsognature.pipeline.SetSpriteGpipleneDescAll(&pipelinerootsognature.shader);

#pragma endregion 頂点シェーダとピクセルシェーダをパイプラインに設定
    //-----------------------------------

    //サンプルマスクとラスタライザステートの設定------------------------------------
#pragma region サンプルマスクとラスタライザステートの設定
    pipelinerootsognature.pipeline.SetSampleMask();

    pipelinerootsognature.pipeline.SetSpriteAllAddRasterizerState();

#pragma endregion サンプルマスクとラスタライザステートの設定
    //------------------------------------


     //ブレンドステートの設定-------------------------------
#pragma region ブレンドステートの設定

    pipelinerootsognature.pipeline.SetRenderTaegetBlendDesc(pipelinerootsognature.pipeline.pipelineDesc.BlendState.RenderTarget[0]);

    pipelinerootsognature.pipeline.SetRenderTargetWriteMask();

    pipelinerootsognature.pipeline.SetNormalBlendDesc();

    pipelinerootsognature.pipeline.SetAlphaBlend();


#pragma endregion ブレンドステートの設定
    //--------------------------

    //頂点レイアウトの設定------------------
#pragma region 頂点レイアウトの設定

    pipelinerootsognature.pipeline.pipelineDesc.InputLayout.pInputElementDescs = pipelinerootsognature.shader.SpriteinputLayout;
    pipelinerootsognature.pipeline.pipelineDesc.InputLayout.NumElements = _countof(pipelinerootsognature.shader.SpriteinputLayout);

#pragma endregion 頂点レイアウトの設定
    //----------------------------

    //図形の形状を三角形に設定-------------------------
    pipelinerootsognature.pipeline.SetPrimitiveTopologyType();
    //------------------

    //その他の設定----------------
#pragma region その他の設定

    pipelinerootsognature.pipeline.SetNumRenderTargets();
    pipelinerootsognature.pipeline.SetRTVFormats();
    pipelinerootsognature.pipeline.SetSampleDescCount();

#pragma endregion その他の設定
    //----------------

    depth.SetSpriteDepthStencilState(pipelinerootsognature.pipeline.pipelineDesc);

    //ルートシグネチャの生成--------------------------
#pragma region ルートシグネチャの生成


    pipelinerootsognature.rootsignature.InitRootSignatureDesc(rootparams, pipelinerootsognature.texSample);

    pipelinerootsognature.rootsignature.SetSerializeRootSignature(pipelinerootsognature.shader, dx12);

    pipelinerootsognature.rootsignature.CreateRootSignature(&dx12);

    // パイプラインにルートシグネチャをセット

    pipelinerootsognature.pipeline.SetRootSignature(pipelinerootsognature.rootsignature);

#pragma endregion ルートシグネチャの生成
    //--------------------------------

   //パイプラインステートの生成-------------------------
#pragma region パイプラインステートの生成

    pipelinerootsognature.pipeline.CreateGraphicsPipelineState(&dx12);

#pragma endregion パイプラインステートの生成
    //-----------------------------


    return pipelinerootsognature;
}
