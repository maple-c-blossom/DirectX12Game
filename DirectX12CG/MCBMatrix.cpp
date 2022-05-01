#include "MCBMatrix.h"

void MCB::MCBMatrix::MCBMatrixIdentity()
{
	_11 = 1.0f; _12 = 0.0f; _13 = 0.0f; _14 = 0.0f;
	_21 = 0.0f; _22 = 1.0f; _23 = 0.0f; _24 = 0.0f;
	_31 = 0.0f; _32 = 0.0f; _33 = 1.0f; _34 = 0.0f;
	_41 = 0.0f; _42 = 0.0f; _43 = 0.0f; _44 = 1.0f;
}

MCB::MCBMatrix MCB::MCBMatrix::MCBMatrixRota(double angle, DirectX::XMFLOAT3 vector)
{
	MCBMatrix matRot = ReturnMatrixIdentity();

	matRot._11 = (vector.x * vector.x) * (1 - cos(angle)) + cos(angle);
	matRot._12 = vector.x * vector.y * (1 - cos(angle)) - vector.z * sin(angle);
	matRot._13 = vector.x * vector.z * (1 - cos(angle)) + vector.y * sin(angle);
	matRot._14 = 0;

	matRot._21 = vector.x * vector.y * (1 - cos(angle)) + vector.z * sin(angle);
	matRot._22 = (vector.y * vector.y) * (1 - cos(angle)) + cos(angle);
	matRot._23 = vector.y * vector.z * (1 - cos(angle)) + vector.x * sin(angle);
	matRot._24 = 0;

	matRot._31 = vector.x * vector.z * (1 - cos(angle)) - vector.y * sin(angle);
	matRot._32 = vector.y * vector.z * (1 - cos(angle)) + vector.x * sin(angle);
	matRot._33 = (vector.z * vector.z) * (1 - cos(angle)) + cos(angle);
	matRot._34 = 0;

	matRot._41 = 0;
	matRot._42 = 0;
	matRot._43 = 0;
	matRot._44 = 1;

	return matRot;


}

MCB::MCBMatrix MCB::MCBMatrix::ReturnMatrixIdentity()
{
	MCBMatrix ans;
		ans._11 = 1.0f; ans._12 = 0.0f; ans._13 = 0.0f; ans._14 = 0.0f;
		ans._21 = 0.0f; ans._22 = 1.0f; ans._23 = 0.0f; ans._24 = 0.0f;
		ans._31 = 0.0f; ans._32 = 0.0f; ans._33 = 1.0f; ans._34 = 0.0f;
		ans._41 = 0.0f; ans._42 = 0.0f; ans._43 = 0.0f; ans._44 = 1.0f;
	return ans;
}

MCB::MCBMatrix MCB::MCBMatrix::operator*(MCBMatrix matrix)
{
	MCBMatrix ans;

#pragma region １行目
	ans._11 = (_11 * matrix._11) + (_12 * matrix._21) + (_13 * matrix._31) + (_14 * matrix._41);
	ans._12 = (_11 * matrix._12) + (_12 * matrix._22) + (_13 * matrix._32) + (_14 * matrix._42);
	ans._13 = (_11 * matrix._13) + (_12 * matrix._23) + (_13 * matrix._33) + (_14 * matrix._43);
	ans._14 = (_11 * matrix._14) + (_12 * matrix._24) + (_13 * matrix._34) + (_14 * matrix._44);
#pragma endregion 1行目

#pragma region 2行目
	ans._21 = (_21 * matrix._11) + (_22 * matrix._21) + (_23 * matrix._31) + (_24 * matrix._41);
	ans._22 = (_21 * matrix._12) + (_22 * matrix._22) + (_23 * matrix._32) + (_24 * matrix._42);
	ans._23 = (_21 * matrix._13) + (_22 * matrix._23) + (_23 * matrix._33) + (_24 * matrix._43);
	ans._24 = (_21 * matrix._14) + (_22 * matrix._24) + (_23 * matrix._34) + (_24 * matrix._44);
#pragma endregion 2行目

#pragma region 3行目
	ans._31 = (_31 * matrix._11) + (_32 * matrix._21) + (_33 * matrix._31) + (_34 * matrix._41);
	ans._32 = (_31 * matrix._12) + (_32 * matrix._22) + (_33 * matrix._32) + (_34 * matrix._42);
	ans._33 = (_31 * matrix._13) + (_32 * matrix._23) + (_33 * matrix._33) + (_34 * matrix._43);
	ans._34 = (_31 * matrix._14) + (_32 * matrix._24) + (_33 * matrix._34) + (_34 * matrix._44);
#pragma endregion 3行目

#pragma region 4行目
	ans._41 = (_41 * matrix._11) + (_42 * matrix._21) + (_43 * matrix._31) + (_44 * matrix._41);
	ans._42 = (_41 * matrix._12) + (_42 * matrix._22) + (_43 * matrix._32) + (_44 * matrix._42);
	ans._43 = (_41 * matrix._13) + (_42 * matrix._23) + (_43 * matrix._33) + (_44 * matrix._43);
	ans._44 = (_41 * matrix._14) + (_42 * matrix._24) + (_43 * matrix._34) + (_44 * matrix._44);
#pragma endregion 4行目

	return ans;
}

MCB::MCBMatrix MCB::MCBMatrix::operator*(float s)
{
	MCBMatrix ans;
	ans._11 = _11 * s;
	ans._12 = _12 * s;
	ans._13 = _13 * s;
	ans._14 = _14 * s;

	ans._21 = _21 * s;
	ans._22 = _22 * s;
	ans._23 = _23 * s;
	ans._24 = _24 * s;

	ans._31 = _31 * s;
	ans._32 = _32 * s;
	ans._33 = _33 * s;
	ans._34 = _34 * s;

	ans._41 = _41 * s;
	ans._42 = _42 * s;
	ans._43 = _43 * s;
	ans._44 = _44 * s;

	return ans;
}

MCB::MCBMatrix MCB::MCBMatrix::operator/(float s)
{
	MCBMatrix ans;
	ans._11 = _11 / s;
	ans._12 = _12 / s;
	ans._13 = _13 / s;
	ans._14 = _14 / s;

	ans._21 = _21 / s;
	ans._22 = _22 / s;
	ans._23 = _23 / s;
	ans._24 = _24 / s;

	ans._31 = _31 / s;
	ans._32 = _32 / s;
	ans._33 = _33 / s;
	ans._34 = _34 / s;

	ans._41 = _41 / s;
	ans._42 = _42 / s;
	ans._43 = _43 / s;
	ans._44 = _44 / s;

	return ans;
}

MCB::MCBMatrix MCB::MCBMatrix::operator+(MCBMatrix matrix)
{
	MCBMatrix ans;
	ans._11 = _11 + matrix._11; 
	ans._12 = _12 + matrix._12; 
	ans._13 = _13 + matrix._13; 
	ans._14 = _14 + matrix._14; 

	ans._21 = _21 + matrix._21;
	ans._22 = _22 + matrix._22;
	ans._23 = _23 + matrix._23;
	ans._24 = _24 + matrix._24;

	ans._31 = _31 + matrix._31;
	ans._32 = _32 + matrix._32;
	ans._33 = _33 + matrix._33;
	ans._34 = _34 + matrix._34;

	ans._41 = _41 + matrix._41;
	ans._42 = _42 + matrix._42;
	ans._43 = _43 + matrix._43;
	ans._44 = _44 + matrix._44;


	return ans;
}

MCB::MCBMatrix MCB::MCBMatrix::operator-(MCBMatrix matrix)
{
	MCBMatrix ans;
	ans._11 = _11 - matrix._11;
	ans._12 = _12 - matrix._12;
	ans._13 = _13 - matrix._13;
	ans._14 = _14 - matrix._14;

	ans._21 = _21 - matrix._21;
	ans._22 = _22 - matrix._22;
	ans._23 = _23 - matrix._23;
	ans._24 = _24 - matrix._24;

	ans._31 = _31 - matrix._31;
	ans._32 = _32 - matrix._32;
	ans._33 = _33 - matrix._33;
	ans._34 = _34 - matrix._34;

	ans._41 = _41 - matrix._41;
	ans._42 = _42 - matrix._42;
	ans._43 = _43 - matrix._43;
	ans._44 = _44 - matrix._44;


	return ans;
}

MCB::MCBMatrix& MCB::MCBMatrix::operator*=(MCBMatrix matrix)
{
	*this = *this * matrix;
	return(*this);
}

MCB::MCBMatrix& MCB::MCBMatrix::operator*=(float s)
{
	*this = *this * s;
	return(*this);
}

MCB::MCBMatrix& MCB::MCBMatrix::operator/=(float s)
{
	*this = *this / s;
	return(*this);
}

MCB::MCBMatrix& MCB::MCBMatrix::operator+=(MCBMatrix matrix)
{
	*this = *this + matrix;
	return(*this);
}

MCB::MCBMatrix& MCB::MCBMatrix::operator-=(MCBMatrix matrix)
{
	*this = *this - matrix;
	return *this;
}
