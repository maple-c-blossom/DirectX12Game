#pragma once
#include "Vector3D.h"
namespace MCB
{
	class Quaternion
	{
		Float4 quaternion = { 0,0,0,0 };

		void SetRotaQuaternion(Vector3D vec,float angle);
		Quaternion GetQuaternionConjugated(Quaternion q);
		Quaternion GetQuaternionReciprocal(Quaternion q);
		Quaternion GetCartesianProduct(Quaternion q, Quaternion p);

	};
}

