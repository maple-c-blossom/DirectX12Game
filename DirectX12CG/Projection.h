#pragma once

#include <DirectXMath.h>


#pragma region using namespace

using namespace std;
using namespace DirectX;

#pragma endregion using namespace

class Projection
{
public:
	XMMATRIX mat; //�s��
	float fovAngle; //�㉺��p
	float aspectRatio; //�A�X�y�N�g��
	float nearZ;//�t�@�[�N���b�v����(�O)
	float farZ;//�t�@�[�N���b�v����(���)

	void CreateMatrixProjection(float fovAngle, float aspectRatio, float nearZ, float farZ);
	void UpdataMatrixProjection();
};

