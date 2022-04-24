#pragma once
#include <DirectXMath.h>


#pragma region using namespace

using namespace std;
using namespace DirectX;

#pragma endregion using namespace

class View
{
    public:
    XMMATRIX mat = XMMatrixIdentity();//�s��
    XMFLOAT3 eye{ 0,0,0 }; //���_���W(�J�������g�̍��W)
    XMFLOAT3 target{ 0,0,1 };//�����_���W(�J���������Ă�����W�AXMMatrixLookAtLH�͂��̍��W���王�_���W�ւ̃x�N�g�������߂邱�ƂŌ��������肵�Ă���)
    XMFLOAT3 up{ 0,1,0 };//������x�N�g��(�J�����̏㑤���ǂ��������肷����W�B)

    /// <summary>
    /// �����̒l���N���X�̂��ꂼ��̕ϐ��ɑ��������Ńr���[�ϊ��s����쐬���܂��B
    /// </summary>
    /// <param name="eye">���_���W</param>
    /// <param name="target">�����_���W</param>
    /// <param name="up">������x�N�g��</param>
    void CreateMatrixView(XMFLOAT3 eye, XMFLOAT3 target, XMFLOAT3 up);

    /// <summary>
    /// ���łɂ���ϐ��𗘗p���čs����쐬���܂�
    /// </summary>
    void UpDateMatrixView();
};

