#include "Draw.h"

void MCB::Draw::SetBeforeResourceBarrier(Dx12& dx12)
{
    barrierDesc = {};
    barrierDesc.Transition.pResource = dx12.backBuffers[bbIndex].Get(); // �o�b�N�o�b�t�@���w��
    barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT; // �\������
    barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // �`��
    dx12.commandList->ResourceBarrier(1, &barrierDesc);
}

void MCB::Draw::BeforeDraw(Dx12 &dx12, Depth depth, ShaderResource srv, PipelineRootSignature pipeline)
{
    dx12.commandList->SetPipelineState(pipeline.pipeline.pipelinestate.Get());
    dx12.commandList->SetGraphicsRootSignature(pipeline.rootsignature.rootsignature.Get());


    //�v���~�e�B�u�`��̐ݒ�R�}���h�i�O�p�`���X�g�j--------------------------
    dx12.commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //SRV�q�[�v�̐ݒ�R�}���h
    dx12.commandList->SetDescriptorHeaps(1, srv.srvHeap.GetAddressOf());
}

void MCB::Draw::SetBeforeBbIndex(Dx12& dx12)
{
    bbIndex = dx12.swapchain->GetCurrentBackBufferIndex();
}

D3D12_RESOURCE_BARRIER MCB::Draw::GetResouceBarrier()
{
    return barrierDesc;
}

UINT MCB::Draw::GetBbIndex()
{
    return bbIndex;
}

void MCB::Draw::SetAfterResourceBarrier(Dx12& dx12)
{
    barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // �`��
    barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;   // �\����
    dx12.commandList->ResourceBarrier(1, &barrierDesc);
}

void MCB::Draw::CloseDrawCommandOrder(Dx12& dx12)
{
    // ���߂̃N���[�Y----------------------------------
    dx12.result = dx12.commandList->Close();
    assert(SUCCEEDED(dx12.result) && "���߃N���[�Y�i�K�ł̃G���[");
    //------------
}


void MCB::Draw::SetRenderTargetView(Dx12& dx12, Depth& depth)
{
    rtvHandle = dx12.rtvHeaps->GetCPUDescriptorHandleForHeapStart();
    rtvHandle.ptr += bbIndex * dx12.device->GetDescriptorHandleIncrementSize(dx12.rtvHeapDesc.Type);
    dsvHandle = depth.dsvHeap->GetCPUDescriptorHandleForHeapStart();
    dx12.commandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);
}


void MCB::Draw::SetViewPort(Dx12& dx12,DxWindow& dxWindow)
{
    viewport = {};

    viewport.Width = dxWindow.window_width;
    viewport.Height = dxWindow.window_height;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    dx12.commandList->RSSetViewports(1, &viewport);
}

void MCB::Draw::ClearScreen(Dx12& dx12, const float* clearColor)
{
    dx12.commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
    dx12.commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void MCB::Draw::CommandListExecution(Dx12& dx12)
{
    // �R�}���h���X�g�̎��s-------------------------------------
#pragma region �R�}���h���X�g���s
    ID3D12CommandList* commandLists[] = { dx12.commandList.Get() }; // �R�}���h���X�g�̔z��
    dx12.commandQueue->ExecuteCommandLists(1, commandLists);

    // �o�b�t�@���t���b�v�i���\�̓��ւ�)-----------------------
    dx12.result = dx12.swapchain->Present(1, 0);
    assert(SUCCEEDED(dx12.result) && "�o�b�t�@�t���b�v�i�K�ł̃G���[");
    //-----------------

#pragma region �R�}���h���s�����҂�
    // �R�}���h���X�g�̎��s������҂�
    dx12.commandQueue->Signal(dx12.fence.Get(), ++dx12.fenceVal);
    if (dx12.fence->GetCompletedValue() != dx12.fenceVal)
    {
        HANDLE event = CreateEvent(nullptr, false, false, nullptr);
        dx12.fence->SetEventOnCompletion(dx12.fenceVal, event);
        WaitForSingleObject(event, INFINITE);
        CloseHandle(event);
    }
#pragma endregion �R�}���h���s�����҂�
}

void MCB::Draw::SetScissorrect(Dx12& dx12, DxWindow& dxWindow)
{
    scissorrect = {};

    scissorrect.left = 0;                                       // �؂蔲�����W��
    scissorrect.right = scissorrect.left + dxWindow.window_width;        // �؂蔲�����W�E
    scissorrect.top = 0;                                        // �؂蔲�����W��
    scissorrect.bottom = scissorrect.top + dxWindow.window_height;       // �؂蔲�����W��

    dx12.commandList->RSSetScissorRects(1, &scissorrect);
}

void MCB::Draw::ResetQueAndCommandList(Dx12& dx12)
{
    //�L���[���N���A
    dx12.result = dx12.commandAllocator->Reset(); // �L���[���N���A
    assert(SUCCEEDED(dx12.result) && "�L���[�N���A�i�K�ł̃G���[");

    //�ĂуR�}���h���X�g�����߂鏀��
    dx12.result = dx12.commandList->Reset(dx12.commandAllocator.Get(), nullptr);  // �ĂуR�}���h���X�g�𒙂߂鏀��
    assert(SUCCEEDED(dx12.result) && "�R�}���h���X�g�Ē��~�����i�K�ł̃G���[");
}

void MCB::Draw::PreDraw(Dx12& dx12, Depth depth, ShaderResource srv, PipelineRootSignature pipeline, DxWindow& dxWindow, const float* clearColor)
{
    SetBeforeBbIndex(dx12);
    SetBeforeResourceBarrier(dx12);
    SetRenderTargetView(dx12, depth);
    ClearScreen(dx12, clearColor);
    SetViewPort(dx12, dxWindow);
    SetScissorrect(dx12, dxWindow);
    BeforeDraw(dx12, depth, srv, pipeline);

}

void MCB::Draw::PostDraw(Dx12& dx12)
{
    SetAfterResourceBarrier(dx12);

    CloseDrawCommandOrder(dx12);

    CommandListExecution(dx12);

    ResetQueAndCommandList(dx12);
}
