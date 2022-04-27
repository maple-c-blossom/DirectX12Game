#pragma once

#include <DirectXMath.h>
#include<vector>

namespace MCB
{
	class TexImgData
	{
	public:
        //�������s�N�Z����
        size_t textureWidth = 256;
        //�c�����s�N�Z����
        size_t textureHeight = 256;
        //�z��̗v�f��
        size_t imageDataCount = textureWidth * textureHeight;
        //�摜�C���[�W�f�[�^�̔z��
       std::vector<DirectX::XMFLOAT4> imageData;
    
       ~TexImgData();
        void SetImageDataRGBA(DirectX::XMFLOAT4 RGBA);
  

	};

}

