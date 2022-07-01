#include "Quaternion.h"
#include <cmath>
#define PI 3.14159265358979323846264338327950288f
using namespace MCB;

void MCB::Quaternion::SetRota(Vector3D vec, float angle)
{
	vec.V3Norm();
	float s = (sinf(angle / 2));
	x = vec.vec.x * s;
	y = vec.vec.y * s;
	z = vec.vec.z * s;
	w = cosf(angle / 2);
	this->QuaternoinNorm();
}

Quaternion MCB::Quaternion::GetConjugated(Quaternion q)
{
	q.x *= -1;
	q.y *= -1;
	q.z *= -1;
	return q;
}

Quaternion MCB::Quaternion::GetReciprocal(Quaternion q)
{
	return GetConjugated(q);
}

Quaternion MCB::Quaternion::GetCartesianProduct(Quaternion q, Quaternion p)
{
	Quaternion ans;
	ans.x = (q.x * p.w) + (q.y * p.z) - (q.z * p.y) + (q.w * p.x);
	ans.y = (-q.x * p.z) + (q.y * p.w) + (q.z * p.x) + (q.w * p.y);
	ans.z = (q.x * p.y) - (q.y * p.x) + (q.z * p.w) + (q.w * p.z);
	ans.w = (-q.x * p.x) - (q.y * p.y) - (q.z * p.z) + (q.w * p.w);
	ans.QuaternoinNorm();
	return ans;
}

float MCB::Quaternion::Dot(Quaternion a, Quaternion b)
{
	
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

float MCB::Quaternion::GetAngle(Quaternion a, Quaternion b)
{
	return acosf(Dot(a, b));;
}

Quaternion MCB::Quaternion::SetRotationQuaternion(Vector3D rotationAxisVec, Vector3D PositionVec, float angle)
{
	PositionVec.V3Norm();
	Quaternion position{};
	position.x = PositionVec.vec.x;
	position.y = PositionVec.vec.y;
	position.z = PositionVec.vec.z;
	position.w = 0;
	Quaternion RotationAngle;
	RotationAngle.SetRota(rotationAxisVec, angle);

	position = GetCartesianProduct(RotationAngle, position);
	position = GetCartesianProduct(position, GetReciprocal(RotationAngle));

	position.QuaternoinNorm();
	return position;
}

Quaternion MCB::Quaternion::SetRotationQuaternion(Quaternion rotationQuaternion, Vector3D PositionVec)
{
	PositionVec.V3Norm();
	Quaternion position{};
	position.x = PositionVec.vec.x;
	position.y = PositionVec.vec.y;
	position.z = PositionVec.vec.z;
	position.w = 0;

	position = GetCartesianProduct(rotationQuaternion, position);
	position = GetCartesianProduct(position, GetReciprocal(rotationQuaternion));

	position.QuaternoinNorm();
	return position;
}

Quaternion MCB::Quaternion::SetRotationQuaternion(Quaternion rotationQuaternion, Quaternion PositionVec)
{

	Quaternion position{};

	position = GetCartesianProduct(rotationQuaternion, PositionVec);
	position = GetCartesianProduct(PositionVec, GetReciprocal(rotationQuaternion));

	position.QuaternoinNorm();
	return position;
}


void MCB::Quaternion::QuaternoinNorm()
{
	float mag = (float)sqrt(x * x + y * y + z * z + w * w);

	if (mag > 0.0f)
	{
		float oneOverMag = 1.0f / mag;

		x *= oneOverMag;
		y *= oneOverMag;
		z *= oneOverMag;
		w *= oneOverMag;
	}
	

}

MCBMatrix MCB::Quaternion::GetQuaternionRotaMat(Quaternion rotaQ)
{
	MCBMatrix mat;
	mat.MCBMatrixIdentity();
	mat._11 = 1 - (2 * (y * y)) - (2 * (z * z));
	mat._12 = (2 * x * y) + (2 * w * z);
	mat._13 = (2 * x * z) - (2 * w * y);
	mat._14 = 0;

	mat._21 = (2 * x * y) - (2 * w * z);
	mat._22 = 1 - (2 * (x * x)) - (2 * (z * z));
	mat._23 = (2 * y * z) + (2 * w * x);
	mat._24 = 0;

	mat._31 = (2 * x * z) + (2 * w * y);
	mat._32 = (2 * y * z) - (2 * w * x);
	mat._33 = 1 - (2 * (x * x)) - (2 * (y * y));
	mat._34 = 0;

	mat._41 = 0;
	mat._42 = 0;
	mat._43 = 0;
	mat._44 = 1;


	return mat;
}

void MCB::Quaternion::SinCos(float* returnSin, float* returnCos, float theta)
{
	*returnSin = sin(theta);
	*returnCos = cos(theta);
}

Quaternion MCB::Quaternion::SetToRorateObjectToInternal(const Float3 eulerAngle)
{
	Quaternion ans;
	float sp, sb, sh;
	float cp, cb, ch;

	SinCos(&sp, &cp, eulerAngle.x * 0.5f);
	SinCos(&sb, &cb, eulerAngle.z * 0.5f);
	SinCos(&sh, &ch, eulerAngle.y * 0.5f);

	ans.x = ch * sp * cb + sh * cp * sb;
	ans.y = -ch * sp * sb + sh * cp * cb;
	ans.z = -sh * sp * cb + ch * cp * sb;
	ans.w = ch * cp * cb + sh * sp * sb;

	return ans;
}

Vector3D MCB::Quaternion::GetRotationAxis(Quaternion q)
{
	float sinThetaOver2Sq = 1.0f - q.w * q.w;

	if (sinThetaOver2Sq <= 0.0f)
	{
		return Vector3D(0,0,1);
	}


	float oneOverSinThetaOver2 = 1.0f / sqrt(sinThetaOver2Sq);


	return Vector3D(
		q.x * oneOverSinThetaOver2,
		q.y * oneOverSinThetaOver2,
		q.z * oneOverSinThetaOver2
		);

}

void MCB::Quaternion::GetRotationAxis(Quaternion q, Vector3D& AxisVec)
{
	float sinThetaOver2Sq = 1.0f - q.w * q.w;

	if (sinThetaOver2Sq <= 0.0f)
	{
		return;
	}


	float oneOverSinThetaOver2 = 1.0f / sqrt(sinThetaOver2Sq);


	AxisVec = Vector3D(
		q.x * oneOverSinThetaOver2,
		q.y * oneOverSinThetaOver2,
		q.z * oneOverSinThetaOver2
	);
}

float MCB::Quaternion::GetAngle(Quaternion q)
{
	float thetaOver2 = SafeAcos(q.w);
	return thetaOver2 * 2.0f;
}

float MCB::Quaternion::SafeAcos(float a)
{
	if (a <= -1.0f)
	{
		return PI;
	}
	if (a >= 1.0f)
	{
		return 0.0f;
	}
	return acos(a);
}

bool MCB::Quaternion::operator==(Quaternion q)
{
	if (q.x == x && q.y == y && q.z == z && q.w == w)
	{
		return true;
	}
	return false;
}

MCB::Quaternion MCB::Quaternion::Slerp(Quaternion start, Quaternion end, int time, int maxTime)
{
	float Time = (float)time / (float)maxTime;
	Quaternion ans;
	float angle = GetAngle(start, end);

	if (angle < 0)
	{
		angle *= -1;
	}


	float st = sinf(angle);

	if (st == 0)
	{
		return start;
	}

	float sut = sinf(angle * Time);
	float sout = sinf(angle * (1.0f - Time));

	float coeff1 = sout / st;
	float coeff2 = sut / st;

	ans.x = coeff1 * start.x + coeff2 * end.x;
	ans.y = coeff1 * start.y + coeff2 * end.y;
	ans.z = coeff1 * start.z + coeff2 * end.z;
	ans.w = coeff1 * start.w + coeff2 * end.w;

	ans.QuaternoinNorm();
	return ans;
}
