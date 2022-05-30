#pragma once
#include "Vector3D.h"
namespace MCB
{
	class Quaternion
	{
		Float4 quaternion = { 0,0,0,0 };
		
		/// <summary>
		/// vec���ł�angle���̉�]��\���N�H�[�^�j�I���𐶐�
		/// </summary>
		/// <param name="vec">��]���̃x�N�g��</param>
		/// <param name="angle">��]�p�x(���W�A��)</param>
		void SetRotaQuaternion(Vector3D vec,float angle);

		/// <summary>
		/// �����œn���ꂽ�N�H�[�^�j�I���̋�����Ԃ�
		/// </summary>
		/// <param name="q">�������ق����N�H�[�^�j�I��</param>
		/// <returns></returns>
		Quaternion GetQuaternionConjugated(Quaternion q);

		/// <summary>
		/// �N�H�[�^�j�I���̋t����Ԃ�
		/// </summary>
		/// <param name="q">�t�����ق����N�H�[�^�j�I��</param>
		/// <returns></returns>
		Quaternion GetQuaternionReciprocal(Quaternion q);

		/// <summary>
		/// ��̃N�H�[�^�j�I���̒��ς�Ԃ�
		/// �������̏��Ԃ��ς��ƌ��ʂ��ς��̂Œ��ӂ��邱��!!
		/// </summary>
		/// <param name="q">���ӂɂ��������N�H�[�^�j�I��</param>
		/// <param name="p">�E�ӂɂ��������N�H�[�^�j�I��</param>
		/// <returns></returns>
		Quaternion GetCartesianProduct(Quaternion q, Quaternion p);

		//cos�Ƃ���sin�Ƃւ̕ϊ�
		// cos = q1��q2�̓���
		//sin = sqrtf(1 - (cos*cos));
		
		//�� = acos(q1��q2�̓���)
		//qSLerp = (q1*sin((1-t)��) + q2sin(t��) ) / sin��

	};
}

