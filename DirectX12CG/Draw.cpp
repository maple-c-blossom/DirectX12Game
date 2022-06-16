#include "Draw.h"

void MCB::Draw::AfterDraw(Dx12 &dx12, Depth depth, ShaderResource srv, PipelineRootSignature pipeline)
{
    dx12.commandList->SetPipelineState(pipeline.pipeline.pipelinestate.Get());
    dx12.commandList->SetGraphicsRootSignature(pipeline.rootsignature.rootsignature.Get());


    //�v���~�e�B�u�`��̐ݒ�R�}���h�i�O�p�`���X�g�j--------------------------
    dx12.commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //SRV�q�[�v�̐ݒ�R�}���h
    dx12.commandList->SetDescriptorHeaps(1, srv.srvHeap.GetAddressOf());
}
