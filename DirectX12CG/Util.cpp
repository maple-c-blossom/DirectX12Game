#include "Util.h"

using namespace MCB;

double MCB::Lerp(double startPos, double endPos, double maxTime, double time)
{
	time /= maxTime;
	double change = endPos - startPos;
	return change * time + startPos;
}
double MCB::InQuad(double startPos, double endPos, double maxTime, double time)
{
	time /= maxTime;
	double change = endPos - startPos;
	return change * time * time + startPos;
}
double MCB::OutQuad(double startPos, double endPos, double maxTime, double time)
{
	time /= maxTime;
	double change = endPos - startPos;
	return -change * time * (time - 2) + startPos;
}
double MCB::InOutQuad(double startPos, double endPos, double maxTime, double time)
{
	time /= maxTime / 2;
	double change = endPos - startPos;
	if (time < 1) return change / 2 * time * time + startPos;
	return -change / 2 * ((--time) * (time - 2) - 1) + startPos;
}
double MCB::OutInRelayQuad(double startPos, double endPos, double relayPos, double maxTime, double relayTime, double time)
{
	if (time < relayTime)
	{
		return OutQuad(relayPos, startPos, relayTime, time);
	}
	else
	{
		return InQuad(endPos, relayPos, maxTime - relayTime, time - relayTime);
	}
}
double MCB::OutOutRelayQuad(double startPos, double endPos, double relayPos, double maxTime, double relayTime, double time)
{
	if (time < relayTime)
	{
		return OutQuad(relayPos, startPos, relayTime, time);
	}
	else
	{
		return OutQuad(endPos, relayPos, maxTime - relayTime, time - relayTime);
	}
}
double MCB::EaseInBack(double startPos, double endPos, double maxTime, double time)
{
	time /= maxTime;
	double change = endPos - startPos;
	double c1 = 1.70158;
	double c3 = c1 + 1;
	return change * (c3 * time * time * time - c1 * time * time) + startPos;
}
double MCB::EaseInBack2(double startPos, double endPos, double maxTime, double time)
{
	time /= maxTime;
	double change = endPos - startPos;
	double c1 = 1.2;
	double c3 = c1 + 0.8;
	return change * (c3 * time * time * time - c1 * time * time) + startPos;
}
double MCB::EaseOutBounce(double startPos, double endPos, double time, double maxTime)
{
	time /= maxTime;
	double change = endPos - startPos;
	double n1 = 7.5625;
	double d1 = 2.75;

	if (time < 1 / d1)
	{
		return n1 * time * time + startPos + change;
	}
	else if (time < 2 / d1)
	{
		return n1 * (time -= 1.5 / d1) * time + 0.75 + startPos + change;
	}
	else if (time < 2.5 / d1)
	{
		return n1 * (time -= 2.25 / d1) * time + 0.9375 + startPos + change;
	}
	else
	{
		return n1 * (time -= 2.625 / d1) * time + 0.984375 + startPos + change;
	}
}

int MCB::Abs(int num)
{
	if (num < 0)
	{
		return num * -1;
	}
	return num;
}

float MCB::Abs(float num)
{
	if (num < 0)
	{
		return num * -1;
	}
	return num;
}

double MCB::Abs(double num)
{
	if (num < 0)
	{
		return num * -1;
	}
	return num;
}
