#include "Util.h"
#include <stdlib.h>
#include <time.h>

using namespace MCB;

double MCB::Lerp(double startPos, double endPos, double maxTime, double time)
{
	time /= maxTime;
	double change = endPos - startPos;
	return change * time + startPos;
}
double MCB::InQuad(double startPos, double endPos, double maxTime, double time)
{
	time /= maxTime;
	double change = endPos - startPos;
	return change * time * time + startPos;
}
double MCB::OutQuad(double startPos, double endPos, double maxTime, double time)
{
	time /= maxTime;
	double change = endPos - startPos;
	return -change * time * (time - 2) + startPos;
}
double MCB::InOutQuad(double startPos, double endPos, double maxTime, double time)
{
	time /= maxTime / 2;
	double change = endPos - startPos;
	if (time < 1) return change / 2 * time * time + startPos;
	return -change / 2 * ((--time) * (time - 2) - 1) + startPos;
}
double MCB::OutInRelayQuad(double startPos, double endPos, double relayPos, double maxTime, double relayTime, double time)
{
	if (time < relayTime)
	{
		return OutQuad(relayPos, startPos, relayTime, time);
	}
	else
	{
		return InQuad(endPos, relayPos, maxTime - relayTime, time - relayTime);
	}
}
double MCB::OutOutRelayQuad(double startPos, double endPos, double relayPos, double maxTime, double relayTime, double time)
{
	if (time < relayTime)
	{
		return OutQuad(relayPos, startPos, relayTime, time);
	}
	else
	{
		return OutQuad(endPos, relayPos, maxTime - relayTime, time - relayTime);
	}
}
double MCB::EaseInBack(double startPos, double endPos, double maxTime, double time)
{
	time /= maxTime;
	double change = endPos - startPos;
	double c1 = 1.70158;
	double c3 = c1 + 1;
	return change * (c3 * time * time * time - c1 * time * time) + startPos;
}
double MCB::EaseInBack2(double startPos, double endPos, double maxTime, double time)
{
	time /= maxTime;
	double change = endPos - startPos;
	double c1 = 1.2;
	double c3 = c1 + 0.8;
	return change * (c3 * time * time * time - c1 * time * time) + startPos;
}
double MCB::EaseOutBounce(double startPos, double endPos, double time, double maxTime)
{
	time /= maxTime;
	double change = endPos - startPos;
	double n1 = 7.5625;
	double d1 = 2.75;

	if (time < 1 / d1)
	{
		return n1 * time * time + startPos + change;
	}
	else if (time < 2 / d1)
	{
		return n1 * (time -= 1.5 / d1) * time + 0.75 + startPos + change;
	}
	else if (time < 2.5 / d1)
	{
		return n1 * (time -= 2.25 / d1) * time + 0.9375 + startPos + change;
	}
	else
	{
		return n1 * (time -= 2.625 / d1) * time + 0.984375 + startPos + change;
	}
}

int MCB::Abs(int num)
{
	if (num < 0)
	{
		return num * -1;
	}
	return num;
}

float MCB::Abs(float num)
{
	if (num < 0)
	{
		return num * -1;
	}
	return num;
}

double MCB::Abs(double num)
{
	if (num < 0)
	{
		return num * -1;
	}
	return num;
}

float MCB::ConvertRadius(float angle)
{
	return angle * PI / 180.0f;
}

void MCB::InitRand()
{
	srand(time(nullptr));
}

int MCB::GetRand(int min, int max)
{
	return min + rand() % (max - min);
}

MCB::SimpleFigure::SimpleFigure()
{

	triangle.Init();
	triangle.model = &triangleMaterial;
	triangleMaterial.vertices = {
		{PointA,{1,1,1},{0,0}},
		{PointB,{1,1,1},{0,0}},
		{PointC,{1,1,1},{0,0}}
	};
	triangleMaterial.SetSizeVB();
	triangleMaterial.material.SetVertexBuffer(D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_DIMENSION_BUFFER, triangleMaterial.sizeVB, 1, 1, 1, 1, D3D12_TEXTURE_LAYOUT_ROW_MAJOR);
	triangleMaterial.CreateVertexBuffer(triangleMaterial.material.HeapProp, D3D12_HEAP_FLAG_NONE, triangleMaterial.material.Resdesc, D3D12_RESOURCE_STATE_GENERIC_READ);
	triangleMaterial.VertexMaping();
	triangleMaterial.SetVbView();
	triangleMaterial.material.constMapMaterial->color = color;
	triangle.model->texture.CreateNoTextureFileIsTexture();
}

void MCB::SimpleFigure::DrawTriangle(View view, Projection proj)
{

	Dx12* dx12 = Dx12::GetInstance();
	ShaderResource* descriptor = ShaderResource::GetInstance();

	triangleMaterial.vertices = {
		{PointA,{1,1,1},{0,0}},
		{PointB,{1,1,1},{0,0}},
		{PointC,{1,1,1},{0,0}}
	};

	triangleMaterial.VertexMaping();
	triangleMaterial.material.constMapMaterial->color = color;



	triangle.MatrixUpdata(view,proj);

	//定数バッファビュー(CBV)の設定コマンド
	dx12->commandList->SetGraphicsRootConstantBufferView(2, triangleMaterial.material.constBuffMaterialB1->GetGPUVirtualAddress());

	//SRVヒープの先頭アドレスを取得
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descriptor->srvHeap->GetGPUDescriptorHandleForHeapStart();
	srvGpuHandle.ptr += triangle.model->texture.incrementNum * dx12->device.Get()->GetDescriptorHandleIncrementSize(descriptor->srvHeapDesc.Type);
	//SRVヒープの先頭にあるSRVをパラメータ1番に設定
	dx12->commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	//頂点データ
	dx12->commandList->IASetVertexBuffers(0, 1, &triangleMaterial.vbView);
	//定数バッファビュー(CBV)の設定コマンド
	dx12->commandList->SetGraphicsRootConstantBufferView(0, triangle.constBuffTranceform->GetGPUVirtualAddress());
	//描画コマンド
	dx12->commandList->DrawInstanced(triangleMaterial.vertices.size(), 1, 0, 0);
}
