#pragma once

#include <DirectXMath.h>
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
        DirectX::XMFLOAT4* imageData;

        void SetImageDataRGBA(DirectX::XMFLOAT4 RGBA);
  

	};

}

