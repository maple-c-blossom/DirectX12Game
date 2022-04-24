#pragma once
#include <Windows.h>
class DxWindow
{
public:
    // �E�B���h�E�T�C�Y
    const int window_width = 1280;  // ����
    const int window_height = 720;  // �c��
    bool breakFlag = false;


    MSG msg{}; //���b�Z�[�W��������
    WNDCLASSEX w{}; // �E�B���h�E�N���X�̐ݒ�
        // �E�B���h�E�T�C�Y{ X���W Y���W ���� �c�� }
    RECT wrc = { 0, 0, window_width, window_height };
    HWND hwnd;

    DxWindow();
    ~DxWindow();
	// �E�B���h�E�v���V�[�W�� 
    static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    void messageUpdate();
};
