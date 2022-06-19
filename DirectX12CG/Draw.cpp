#include "Draw.h"

void MCB::Draw::SetBeforeResourceBarrier(Dx12& dx12)
{
    barrierDesc = {};
    barrierDesc.Transition.pResource = dx12.backBuffers[bbIndex].Get(); // バックバッファを指定
    barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT; // 表示から
    barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画
    dx12.commandList->ResourceBarrier(1, &barrierDesc);
}

void MCB::Draw::BeforeDraw(Dx12 &dx12, Depth depth, ShaderResource srv, PipelineRootSignature pipeline)
{
    dx12.commandList->SetPipelineState(pipeline.pipeline.pipelinestate.Get());
    dx12.commandList->SetGraphicsRootSignature(pipeline.rootsignature.rootsignature.Get());


    //プリミティブ形状の設定コマンド（三角形リスト）--------------------------
    dx12.commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //SRVヒープの設定コマンド
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
    barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画
    barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;   // 表示に
    dx12.commandList->ResourceBarrier(1, &barrierDesc);
}

void MCB::Draw::CloseDrawCommandOrder(Dx12& dx12)
{
    // 命令のクローズ----------------------------------
    dx12.result = dx12.commandList->Close();
    assert(SUCCEEDED(dx12.result) && "命令クローズ段階でのエラー");
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
    // コマンドリストの実行-------------------------------------
#pragma region コマンドリスト実行
    ID3D12CommandList* commandLists[] = { dx12.commandList.Get() }; // コマンドリストの配列
    dx12.commandQueue->ExecuteCommandLists(1, commandLists);

    // バッファをフリップ（裏表の入替え)-----------------------
    dx12.result = dx12.swapchain->Present(1, 0);
    assert(SUCCEEDED(dx12.result) && "バッファフリップ段階でのエラー");
    //-----------------

#pragma region コマンド実行完了待ち
    // コマンドリストの実行完了を待つ
    dx12.commandQueue->Signal(dx12.fence.Get(), ++dx12.fenceVal);
    if (dx12.fence->GetCompletedValue() != dx12.fenceVal)
    {
        HANDLE event = CreateEvent(nullptr, false, false, nullptr);
        dx12.fence->SetEventOnCompletion(dx12.fenceVal, event);
        WaitForSingleObject(event, INFINITE);
        CloseHandle(event);
    }
#pragma endregion コマンド実行完了待ち
}

void MCB::Draw::SetScissorrect(Dx12& dx12, DxWindow& dxWindow)
{
    scissorrect = {};

    scissorrect.left = 0;                                       // 切り抜き座標左
    scissorrect.right = scissorrect.left + dxWindow.window_width;        // 切り抜き座標右
    scissorrect.top = 0;                                        // 切り抜き座標上
    scissorrect.bottom = scissorrect.top + dxWindow.window_height;       // 切り抜き座標下

    dx12.commandList->RSSetScissorRects(1, &scissorrect);
}

void MCB::Draw::ResetQueAndCommandList(Dx12& dx12)
{
    //キューをクリア
    dx12.result = dx12.commandAllocator->Reset(); // キューをクリア
    assert(SUCCEEDED(dx12.result) && "キュークリア段階でのエラー");

    //再びコマンドリストをためる準備
    dx12.result = dx12.commandList->Reset(dx12.commandAllocator.Get(), nullptr);  // 再びコマンドリストを貯める準備
    assert(SUCCEEDED(dx12.result) && "コマンドリスト再貯蓄準備段階でのエラー");
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
