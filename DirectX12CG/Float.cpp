#include "Float.h"

Float3::Float3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Float3::Float3()
{
	x = 0;
	y = 0;
	z = 0;
}

Float3& Float3::operator+=(const Float3& a)
{
	this->x += a.x;
	this->y += a.y;
	this->z += a.z;
	return *this;
}

Float3& Float3::operator-=(const Float3& a)
{
	this->x -= a.x;
	this->y -= a.y;
	this->z -= a.z;
	return *this;
}



Float3 operator-(Float3& a, Float3& b)
{
	Float3 temp = a;
	temp -= b;
	return temp;
}
