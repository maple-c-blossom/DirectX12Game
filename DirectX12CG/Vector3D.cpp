#include "Vector3D.h"
#include <cmath>


using namespace MCB;
MCB::Vector3D::Vector3D(Float3 start, Float3 end)
{
	vec = end - start;
}
MCB::Vector3D::Vector3D()
{
	vec.x = 0;
	vec.y = 0;
	vec.z = 0;
}
MCB::Vector3D::Vector3D(Float3 vec)
{
	this->vec = vec;
}
MCB::Vector3D::Vector3D(float x, float y, float z)
{
	vec.x = x;
	vec.y = y;
	vec.z = z;
}
Vector3D MCB::Vector3D::V3Get(Float3 start, Float3 end)
{
	 Vector3D temp(start,end);
	 return temp;
}
float MCB::Vector3D::V3Len() const
{
	return sqrtf(((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z)));
}

void MCB::Vector3D::V3Norm()
{
	float VecLen = V3Len();
	if (VecLen == 0)
	{
		return;
	}

	vec.x = vec.x / VecLen;
	vec.y = vec.y / VecLen;
	vec.z = vec.z / VecLen;

}

float MCB::Vector3D::GetV3Dot(Vector3D vector)
{
	return (vec.x * vector.vec.x) + (vec.y * vector.vec.y) + (vec.z * vector.vec.z);
}

Vector3D MCB::Vector3D::GetV3Cross(Vector3D vector)
{
	Vector3D temp;
	temp.vec.x = vec.y * vector.vec.z - vec.z * vector.vec.y;
	temp.vec.y = vec.z * vector.vec.x - vec.x * vector.vec.z;
	temp.vec.z = vec.x * vector.vec.y - vec.y * vector.vec.x;
	return temp;
}

float MCB::Vector3D::GetInnerProduct(Vector3D vector)
{
	Vector3D temp;
	temp.vec = vec;
	temp.V3Norm();
	vector.V3Norm();
	return temp.GetV3Dot(vector);
}

Vector3D& MCB::Vector3D::operator+=(const Vector3D& Vec)
{
	this->vec.x += Vec.vec.x;
	this->vec.y += Vec.vec.y;
	this->vec.z += Vec.vec.z;
	return *this;
}

Vector3D& MCB::Vector3D::operator-=(const Vector3D& Vec)
{
	this->vec.x -= Vec.vec.x;
	this->vec.y -= Vec.vec.y;
	this->vec.z -= Vec.vec.z;
	return *this;
}

Vector3D& MCB::Vector3D::operator*=(const float& k)
{
	this->vec.x *= k;
	this->vec.y *= k;
	this->vec.z *= k;
	return *this;
}

Vector3D& MCB::Vector3D::operator=(const Float3 a)
{
	this->vec.x = a.x;
	this->vec.y = a.y;
	this->vec.z = a.z;
	return *this;
}

Vector3D MCB::operator+(Vector3D& vecA, Vector3D& vecB)
{
	Vector3D temp;
	temp = vecA;
	temp += vecB;
	return temp;
}

Vector3D MCB::operator-(Vector3D& vecA, Vector3D& vecB)
{
	Vector3D temp;
	temp = vecA;
	temp -= vecB;
	return temp;
}

Vector3D MCB::operator*(Vector3D& vecA, float k)
{
	Vector3D temp;
	temp = vecA;
	temp *= k;
	return temp;
}

