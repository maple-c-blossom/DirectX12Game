#include <Windows.h> 



// �E�B���h�E�v���V�[�W�� 
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {  // ���b�Z�[�W�ɉ����ăQ�[���ŗL�̏������s�� 
	switch (msg) {
		// �E�B���h�E���j�����ꂽ 
	case WM_DESTROY:
		// OS�ɑ΂��āA�A�v���̏I����`���� 
		PostQuitMessage(0);
		return 0;
	}
	// �W���̃��b�Z�[�W�������s�� 
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

//�L�[�{�[�h������-------------------------------
void KeyInit(BYTE* key, BYTE* oldkey, int ArraySize)
{
    for (int i = 0; i < ArraySize; i++)
    {
        oldkey[i] = key[i];
        key[i] = 0;
    }
}
//---------------------

//���͊֐�-----------------------------------
#pragma region ���͊֐�
//�����Ă�����
bool IsKeyDown(int keyNum, BYTE* key)
{
    if (key[keyNum])
    {
        return true;
    }
    return false;
};
//�����Ă��Ȃ����
bool IsKeyNDown(int keyNum, BYTE* key)
{
    if (!key[keyNum])
    {
        return true;
    }
    return false;
};
//���������
bool IsKeyPress(int keyNum, BYTE* key, BYTE* oldkey)
{
    if (key[keyNum] && oldkey[keyNum])
    {
        return true;
    }
    return false;
};
//�������u��
bool IsKeyTrigger(int keyNum, BYTE* key, BYTE* oldkey)
{
    if (key[keyNum] && !oldkey[keyNum])
    {
        return true;
    }
    return false;
};
//�������u��
bool IsKeyRelease(int keyNum, BYTE* key, BYTE* oldkey)
{
    if (!key[keyNum] && oldkey[keyNum])
    {
        return true;
    }
    return false;
};
#pragma endregion ���͊֐�
//--------------------


// Windows�A�v���ł̃G���g���[�|�C���g(main�֐�) 
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) 
{  
	

  // �E�B���h�E�̐����i�O����-----------------------
#pragma region �E�B���h�E����
    // �E�B���h�E�T�C�Y
    const int window_width = 1280;  // ����
    const int window_height = 720;  // �c��

    WNDCLASSEX w{}; // �E�B���h�E�N���X�̐ݒ�
    w.cbSize = sizeof(WNDCLASSEX);
    w.lpfnWndProc = (WNDPROC)WindowProc; // �E�B���h�E�v���V�[�W����ݒ�
    w.lpszClassName = L"DirectXGame"; // �E�B���h�E�N���X��
    w.hInstance = GetModuleHandle(nullptr); // �E�B���h�E�n���h��
    w.hCursor = LoadCursor(NULL, IDC_ARROW); // �J�[�\���w��

    // �E�B���h�E�N���X��OS�ɓo�^
    RegisterClassEx(&w);
    // �E�B���h�E�T�C�Y{ X���W Y���W ���� �c�� }
    RECT wrc = { 0, 0, window_width, window_height };
    AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false); // �����ŃT�C�Y�␳


    // �E�B���h�E�I�u�W�F�N�g�̐���
    HWND hwnd = CreateWindow(w.lpszClassName, // �N���X��
        L"DirectX12Game",         // �^�C�g���o�[�̕���
        WS_OVERLAPPEDWINDOW,        // �W���I�ȃE�B���h�E�X�^�C��
        CW_USEDEFAULT,              // �\��X���W�iOS�ɔC����j
        CW_USEDEFAULT,              // �\��Y���W�iOS�ɔC����j
        wrc.right - wrc.left,       // �E�B���h�E����
        wrc.bottom - wrc.top,   // �E�B���h�E�c��
        nullptr,                // �e�E�B���h�E�n���h��
        nullptr,                // ���j���[�n���h��
        w.hInstance,            // �Ăяo���A�v���P�[�V�����n���h��
        nullptr);               // �I�v�V����

    // �E�B���h�E�\��
    ShowWindow(hwnd, SW_SHOW);

#pragma endregion �E�B���h�E����
    //-------------

    MSG msg{}; //���b�Z�[�W��������

#pragma region DirectX������

#pragma endregion 

    while (true)
    {

        //���b�Z�[�W�֌W------------------
#pragma region ���b�Z�[�W�֌W

// ���b�Z�[�W������H
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) 
        {
            TranslateMessage(&msg); // �L�[���̓��b�Z�[�W�̏���
            DispatchMessage(&msg); // �v���V�[�W���Ƀ��b�Z�[�W�𑗂�
        }

        // x�{�^���ŏI�����b�Z�[�W��������Q�[�����[�v�𔲂���
        if (msg.message == WM_QUIT) 
        {
            break;
        }

#pragma endregion ���b�Z�[�W�֌W
        //--------------------
    }



    UnregisterClass(w.lpszClassName, w.hInstance);

	return 0;
}
