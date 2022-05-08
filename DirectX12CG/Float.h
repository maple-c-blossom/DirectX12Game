#pragma once
class Float3
{
public:
	float x;
	float y;
	float z;

	Float3(float x, float y, float z);
	Float3();

	Float3& operator+=(const Float3& a);
	Float3& operator-=(const Float3& a);
};
Float3 operator-(Float3& a, Float3& b);
	
class Float2
{
public:
	float x;
	float y;
};

