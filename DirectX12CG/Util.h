#pragma once
#include <math.h>
#include "Object3d.h"
#define PI 3.14159265358979323846264338327950288f
namespace MCB
{
	double Lerp(double startPos, double endPos, double maxTime, double time);
	double InQuad(double startPos, double endPos, double maxTime, double time);
	double OutQuad(double startPos, double endPos, double maxTime, double time);
	double InOutQuad(double startPos, double endPos, double maxTime, double time);
	double OutInRelayQuad(double startPos, double endPos, double relayPos, double maxTime, double relayTime, double time);
	double OutOutRelayQuad(double startPos, double endPos, double relayPos, double maxTime, double relayTime, double time);
	double EaseInBack(double startPos, double endPos, double maxTime, double time);
	double EaseInBack2(double startPos, double endPos, double maxTime, double time);
	double EaseOutBounce(double startPos, double endPos, double time, double maxTime);
	int Abs(int num);
	float Abs(float num);
	double Abs(double num);
	float ConvertRadius(float angle);

	void InitRand();
	int GetRand(int min, int max);

	class SimpleFigure
	{
	public:
		SimpleFigure();
		Model triangleMaterial;
		Object3d triangle;

		Float3 PointA = { -0.5f,-0.5f,1.0f }; Float3 PointB = { -0.5f,0.5f,1.0f };
		Float3 PointC = { 0.5f,-0.5f,1.0f }; Float4 color = { 1.0f,1.0f,1.0f,1.0f };
		void  DrawTriangle(View view, Projection proj);

	};
}

