#include "Pipeline.h"

void MCB::Pipeline::SetSampleMask()
{
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
}

void MCB::Pipeline::SetSampleMask(unsigned int sampleMask)
{
	pipelineDesc.SampleMask = sampleMask;
}

void MCB::Pipeline::SetRasterizerState(bool DepthClipEnable , D3D12_CULL_MODE cullmode, D3D12_FILL_MODE fillmode)
{
	pipelineDesc.RasterizerState.CullMode = cullmode;  // 背面カリング
	pipelineDesc.RasterizerState.FillMode = fillmode; // ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = DepthClipEnable; // 深度クリッピングを有効に
}

void MCB::Pipeline::SetAllAddRasterizerState()
{
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;  // 背面カリング
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に
}

void MCB::Pipeline::SetSpriteAllAddRasterizerState()
{
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;  // 背面カリング
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に
}

void MCB::Pipeline::SetGpipleneDesc(D3D12_SHADER_BYTECODE &byteCode, ID3DBlob* blob)
{
	byteCode.pShaderBytecode = blob->GetBufferPointer();
	byteCode.BytecodeLength = blob->GetBufferSize();

}

void MCB::Pipeline::SetGpipleneDescVS(ID3DBlob* blob)
{
	pipelineDesc.VS.pShaderBytecode = blob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = blob->GetBufferSize();

}

void MCB::Pipeline::SetGpipleneDescPS(ID3DBlob* blob)
{
	pipelineDesc.PS.pShaderBytecode = blob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = blob->GetBufferSize();
	pipelineDesc.PS.BytecodeLength = blob->GetBufferSize();
}

void MCB::Pipeline::SetGpipleneDescAll(ID3DBlob* VS, ID3DBlob* PS)
{
	pipelineDesc.VS.pShaderBytecode = VS->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = VS->GetBufferSize();

	pipelineDesc.PS.pShaderBytecode = PS->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = PS->GetBufferSize();
}

void MCB::Pipeline::SetGpipleneDescAll(Shader* shader)
{
	pipelineDesc.VS.pShaderBytecode = shader->vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = shader->vsBlob->GetBufferSize();

	pipelineDesc.GS.pShaderBytecode = shader->gsBlob->GetBufferPointer();
	pipelineDesc.GS.BytecodeLength = shader->gsBlob->GetBufferSize();

	pipelineDesc.PS.pShaderBytecode = shader->psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = shader->psBlob->GetBufferSize();

}

void MCB::Pipeline::SetSpriteGpipleneDescAll(Shader* shader)
{
	pipelineDesc.VS.pShaderBytecode = shader->vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = shader->vsBlob->GetBufferSize();

	pipelineDesc.PS.pShaderBytecode = shader->psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = shader->psBlob->GetBufferSize();

}


void MCB::Pipeline::SetNormalBlendDesc()
{
	//共通設定
	blenddesc.BlendEnable = true;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;
}

void MCB::Pipeline::SetNormalBlendDesc(bool blendEnable, D3D12_BLEND_OP blendOp, D3D12_BLEND srcBlend, D3D12_BLEND destBlend)
{
	blenddesc.BlendEnable = blendEnable;
	blenddesc.BlendOpAlpha = blendOp;
	blenddesc.SrcBlendAlpha = srcBlend;
	blenddesc.DestBlendAlpha = destBlend;
}

void MCB::Pipeline::SetAlphaBlend()
{
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
}

void MCB::Pipeline::SetAddBlend()
{
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_ONE;
	blenddesc.DestBlend = D3D12_BLEND_ONE;
}

void MCB::Pipeline::SetSubBlend()
{
	blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
	blenddesc.SrcBlend = D3D12_BLEND_ONE;
	blenddesc.DestBlend = D3D12_BLEND_ONE;
}

void MCB::Pipeline::SetInvBlend()
{
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
	blenddesc.DestBlend = D3D12_BLEND_ZERO;
}

void MCB::Pipeline::SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType)
{
	pipelineDesc.PrimitiveTopologyType = topologyType;
}

void MCB::Pipeline::SetNumRenderTargets(unsigned int numRenderTarget)
{
	pipelineDesc.NumRenderTargets = numRenderTarget; // 描画対象は1つ
}

void MCB::Pipeline::SetRTVFormats(DXGI_FORMAT RTVFormat, unsigned int FormatNum)
{
	if (FormatNum >= 8) return;

	pipelineDesc.RTVFormats[FormatNum] = RTVFormat;

}

void MCB::Pipeline::SetSampleDescCount(unsigned int count)
{
	pipelineDesc.SampleDesc.Count = count;
}

void MCB::Pipeline::SetRootSignature(RootSignature rootSignature)
{
	pipelineDesc.pRootSignature = rootSignature.rootsignature.Get();
}

void MCB::Pipeline::CreateGraphicsPipelineState(Dx12 *dx12)
{
	dx12->result = dx12->device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelinestate));
	assert(SUCCEEDED(dx12->result));
}



void MCB::Pipeline::SetRenderTaegetBlendDesc(D3D12_RENDER_TARGET_BLEND_DESC renderTarget)
{
	blenddesc = renderTarget;
}

void MCB::Pipeline::SetRenderTargetWriteMask(D3D12_COLOR_WRITE_ENABLE writeEnable)
{
	blenddesc.RenderTargetWriteMask = writeEnable;//標準設定
}
