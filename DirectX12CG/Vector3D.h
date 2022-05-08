#pragma once
#include "Float.h"
namespace MCB
{

	class Vector3D
	{
	public:
		Float3 vec;
		Vector3D(Float3 start, Float3 end);
		Vector3D();
		Vector3D(const Float3 vec);
		Vector3D(const float x, const float y,const float z);

		Vector3D V3Get(Float3 start, Float3 end);
		float V3Len() const;
		void V3Norm();
		float GetV3Dot(Vector3D vector);
		float GetInnerProduct(Vector3D vector);
		Vector3D GetV3Cross(Vector3D vector);

		Vector3D& operator+=(const Vector3D& Vec);
		Vector3D& operator-=(const Vector3D& Vec);
		Vector3D& operator*=(const float& k);
		Vector3D& operator= (const Float3 a);
	};
	Vector3D operator+(Vector3D& vecA, Vector3D& vecB);
	Vector3D operator-(Vector3D& vecA, Vector3D& vecB);
	Vector3D operator*(Vector3D& vecA,float k);

}

