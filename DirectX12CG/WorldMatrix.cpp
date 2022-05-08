#include "WorldMatrix.h"

using namespace MCB;
using  namespace DirectX;

void WorldMatrix::SetMatRotX(float angle, bool ConvertToRadians)
{
	if (ConvertToRadians)
	{
		matRot *= XMMatrixRotationX(XMConvertToRadians(angle));
	}
	else
	{
		matRot *= XMMatrixRotationX(angle);
	}

}

void WorldMatrix::SetMatRotY(float angle, bool ConvertToRadians)
{
	if (ConvertToRadians)
	{
		matRot *= XMMatrixRotationY(XMConvertToRadians(angle));
	}
	else
	{
		matRot *= XMMatrixRotationY(angle);
	}
}

void WorldMatrix::SetMatRotZ(float angle, bool ConvertToRadians)
{
	if (ConvertToRadians)
	{
		matRot *= XMMatrixRotationZ(XMConvertToRadians(angle));
	}
	else
	{
		matRot *= XMMatrixRotationZ(angle);
	}
}

void WorldMatrix::SetMatRot(float angleX, float angleY, float angleZ, bool ConvertToRadians)
{
	matRot = XMMatrixIdentity();
	if (ConvertToRadians)
	{
		matRot *= XMMatrixRotationZ(XMConvertToRadians(angleZ));
		matRot *= XMMatrixRotationX(XMConvertToRadians(angleX));
		matRot *= XMMatrixRotationY(XMConvertToRadians(angleY));
	}
	else
	{
		matRot *= XMMatrixRotationZ(angleZ);
		matRot *= XMMatrixRotationX(angleX);
		matRot *= XMMatrixRotationY(angleY);
	}
}

XMMATRIX WorldMatrix::ReturnMatRot(XMMATRIX matRot ,float angleX, float angleY, float angleZ, bool ConvertToRadians)
{
	if (ConvertToRadians)
	{
		matRot *= XMMatrixRotationZ(XMConvertToRadians(angleZ));
		matRot *= XMMatrixRotationX(XMConvertToRadians(angleX));
		matRot *= XMMatrixRotationY(XMConvertToRadians(angleY));
	}
	else
	{
		matRot *= XMMatrixRotationZ(angleZ);
		matRot *= XMMatrixRotationX(angleX);
		matRot *= XMMatrixRotationY(angleY);
	}

	return matRot;
}

void WorldMatrix::SetMatScale(float scaleX, float scaleY, float scaleZ)
{
	matScale = XMMatrixScaling(scaleX, scaleY, scaleZ);
}

void WorldMatrix::SetMatTrans(float transX, float transY, float transZ)
{
	matTransform = XMMatrixTranslation(transX, transY, transZ);
}


void WorldMatrix::CreateMatrixWorld(XMMATRIX matScale, XMMATRIX matRot, XMMATRIX matTransform)
{
	this->matScale = matScale;
	this->matRot = matRot;
	this->matTransform = matTransform;

	matWorld = XMMatrixIdentity();
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTransform;
}

void WorldMatrix::UpdataMatrixWorld()
{
	matWorld = XMMatrixIdentity();
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTransform;
}

void WorldMatrix::UpdataBillBordMatrixWorld(View view)
{
	matWorld = XMMatrixIdentity();
	matWorld *= view.billMat;
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTransform;
}

