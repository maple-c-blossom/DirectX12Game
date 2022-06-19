#include "MCBFloat.h"
using namespace MCB;

MCB::Float3::Float3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

MCB::Float3::Float3()
{
	x = 0;
	y = 0;
	z = 0;
}

MCB::Float3& Float3::operator+=(const Float3& a)
{
	this->x += a.x;
	this->y += a.y;
	this->z += a.z;
	return *this;
}

MCB::Float3& Float3::operator-=(const Float3& a)
{
	this->x -= a.x;
	this->y -= a.y;
	this->z -= a.z;
	return *this;
}


MCB::Float3 MCB::operator-(MCB::Float3& a, MCB::Float3& b)
{
	Float3 temp = a;
	temp -= b;
	return temp;
}

MCB::Float4::Float4()
{
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}

MCB::Float4::Float4(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

