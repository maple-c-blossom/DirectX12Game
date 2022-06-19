#pragma once
#include <DirectXMath.h>


namespace MCB
{
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

	class Float2
	{
	public:
		float x;
		float y;
	};

	class Float4
	{
	public:
		float x = 0;
		float y = 0;
		float z = 0;
		float w = 0;

		Float4();
		Float4(float x, float y, float z, float w);
	};

	MCB::Float3 operator-(MCB::Float3& a, MCB::Float3& b);
}

