#pragma once
#include "Vector3D.h"
#include "MCBMatrix.h"
namespace MCB
{
	class Quaternion
	{
	public:
		float x;
		float y;
		float z;
		float w;

		void SetRota(Vector3D vec,float angle);
		Quaternion GetConjugated(Quaternion q);
		Quaternion GetReciprocal(Quaternion q);
		Quaternion GetCartesianProduct(Quaternion q, Quaternion p);
		float Dot(Quaternion a, Quaternion b);
		float GetAngle(Quaternion a, Quaternion b);
		Quaternion Slerp(Quaternion start, Quaternion end, int time,int maxTime);
		Quaternion SetRotationQuaternion(Vector3D rotationAxisVec, Vector3D PositionVec, float angle);
		Quaternion SetRotationQuaternion(Quaternion rotationQuaternion, Vector3D PositionVec);
		Quaternion SetRotationQuaternion(Quaternion rotationQuaternion, Quaternion PositionVec);
		void QuaternoinNorm();
		MCBMatrix GetQuaternionRotaMat(Quaternion rotaQ);
		void SinCos(float* returnSin, float* returnCos, float theta);
		Quaternion SetToRorateObjectToInternal(const Float3 eulerAngle);
		Vector3D GetRotationAxis(Quaternion q);
		void GetRotationAxis(Quaternion q,Vector3D& AxisVec);
		float GetAngle(Quaternion q);
		float SafeAcos(float a);

		bool operator== (Quaternion q);
	};
}

