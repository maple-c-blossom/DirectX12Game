#pragma once

#include <DirectXMath.h>
#include<vector>
#include "MCBFloat.h"

namespace MCB
{
	class TexImgData
	{
	public:
        //横方向ピクセル数
        size_t textureWidth = 256;
        //縦方向ピクセル数
        size_t textureHeight = 256;
        //配列の要素数
        size_t imageDataCount = textureWidth * textureHeight;
        //画像イメージデータの配列
       std::vector<DirectX::XMFLOAT4> imageData;
    
       ~TexImgData();
        void SetImageDataRGBA(DirectX::XMFLOAT4 RGBA);
        void SetImageDataRGBA(Float4 RGBA);
  

	};

}

