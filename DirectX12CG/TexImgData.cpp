#include "TexImgData.h"

MCB::TexImgData::~TexImgData()
{
    imageData.clear();
}

void MCB::TexImgData::SetImageDataRGBA(DirectX::XMFLOAT4 RGBA)
{
    for (int i = 0; i < imageDataCount; i++)
    {
    DirectX::XMFLOAT4 imageDataSeed = {0,0,0,0};
      imageDataSeed.x = RGBA.x;//R
      imageDataSeed.y = RGBA.y;//G
      imageDataSeed.z = RGBA.z;//B
      imageDataSeed.w = RGBA.w;//A
      imageData.push_back(imageDataSeed);
    }
}
