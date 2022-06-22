#include "DebugText.h"

void MCB::DebugText::Init(Dx12& dx, DxWindow window, Texture* texture)
{
	for (int i = 0; maxCharCount > i; i++)
	{
		sprite[i] = sprite[i].CreateSprite(dx, window);
	}
	debugfont = texture;
	dx12 = &dx;
}

void MCB::DebugText::Print(float x, float y, float scale, const char* text, ...)
{
	
	va_list args;
	va_start(args, text);
	int w = vsnprintf(buffer, maxCharCount - 1, text, args);
	for (int i = 0; i < w; i++)
	{
		if (spriteIndex >= maxCharCount) break;

		const unsigned char& character = buffer[i];

		int fontIndex = character - 32;
		if (character >= 0x7f)
		{
			fontIndex = 0;
		}

		int fontIndexY = fontIndex / fontLineCount;
		int fontIndexX = fontIndex % fontLineCount;

		sprite[spriteIndex].position = {x + fontWidth * scale * i,y,0};
		sprite[spriteIndex].texLeftTop = {(float)fontIndexX * fontWidth,(float)fontIndexY * fontHeight};
		sprite[spriteIndex].cuttingSize = {fontWidth,fontHeight};
		sprite[spriteIndex].size = {fontWidth * scale,fontHeight * scale};
		sprite[spriteIndex].anchorPoint = {0,0};

		sprite[spriteIndex].SpriteTransferVertexBuffer(sprite[spriteIndex], debugfont);

		sprite[spriteIndex].SpriteUpdate(sprite[spriteIndex]);


		spriteIndex++;
	}
}

void MCB::DebugText::AllDraw(ShaderResource descriptor)
{
	for (int i = 0; i < spriteIndex; i++)
	{
		//SRVヒープの先頭アドレスを取得
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descriptor.srvHeap->GetGPUDescriptorHandleForHeapStart();


		srvGpuHandle.ptr += debugfont->incrementNum * dx12->device.Get()->GetDescriptorHandleIncrementSize(descriptor.srvHeapDesc.Type);

		//SRVヒープの先頭にあるSRVをパラメータ1番に設定
		dx12->commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

		//頂点データ
		dx12->commandList->IASetVertexBuffers(0, 1, &sprite[i].vbView);
		//定数バッファビュー(CBV)の設定コマンド
		dx12->commandList->SetGraphicsRootConstantBufferView(0, sprite[i].constBuff->GetGPUVirtualAddress());
		//描画コマンド
		dx12->commandList->DrawInstanced(4, 1, 0, 0);
	}
	spriteIndex = 0;
}

