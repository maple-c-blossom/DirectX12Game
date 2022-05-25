#include "Quaternion.h"
#include <cmath>
using namespace MCB;

void MCB::Quaternion::SetRotaQuaternion(Vector3D vec, float angle)
{
	vec.V3Norm();
	float s = (sinf(angle) / 2);
	quaternion.x = vec.vec.x * s;
	quaternion.y = vec.vec.y * s;
	quaternion.z = vec.vec.z * s;
	quaternion.w = cosf(angle) / 2;
}

Quaternion MCB::Quaternion::GetQuaternionConjugated(Quaternion q)
{
	q.quaternion.x *= -1;
	q.quaternion.y *= -1;
	q.quaternion.z *= -1;
	return q;
}

Quaternion MCB::Quaternion::GetQuaternionReciprocal(Quaternion q)
{

	return Quaternion();
}

Quaternion MCB::Quaternion::GetCartesianProduct(Quaternion q, Quaternion p)
{
	Quaternion ans;
	ans.quaternion.x = (q.quaternion.w * p.quaternion.x) - (q.quaternion.z * p.quaternion.y) + (q.quaternion.y * p.quaternion.z) + (q.quaternion.x * p.quaternion.w);
	ans.quaternion.y = (q.quaternion.z * p.quaternion.x) + (q.quaternion.w * p.quaternion.y) + (q.quaternion.x * p.quaternion.z) + (q.quaternion.y * p.quaternion.w);
	ans.quaternion.z = -(q.quaternion.y * p.quaternion.x) + (q.quaternion.x * p.quaternion.y) + (q.quaternion.w * p.quaternion.z) + (q.quaternion.z * p.quaternion.w);
	ans.quaternion.w = -(q.quaternion.x * p.quaternion.x) - (q.quaternion.y * p.quaternion.y) - (q.quaternion.z * p.quaternion.z) + (q.quaternion.w * p.quaternion.w);
	return ans;
}
