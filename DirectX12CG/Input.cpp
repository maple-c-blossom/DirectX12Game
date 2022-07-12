#include "Input.h"
#include <cassert>
#define DIRECINPUT_VERSION 0x0800 //DirectInput�̃o�[�W�����w��

using namespace MCB;

void Input::Init(HRESULT &result, WNDCLASSEX w, HWND hwnd)
{
	//���͌n������--------------
#pragma region ���͌n������


	result = DirectInput8Create(w.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);
	assert(SUCCEEDED(result));
	//�L�[�{�[�h�f�o�C�X�̐���-----------------
#pragma region �L�[�{�[�h�f�o�C�X�̐���


	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
	assert(SUCCEEDED(result));

#pragma endregion �L�[�{�[�h�f�o�C�X�̐���
	//--------------------------

	//���̓f�[�^�`���Z�b�g--------------------------------
	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));
	//---------------------------------

	//�r�����x������-------------------------------------------------------------
	result = devkeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));

#pragma endregion ���͌n������
	//----------------
}

void Input::UpDateInit(HRESULT &result)
{
	//�L�[�{�[�h������-------------------------------------
#pragma region �L�[�{�[�h������

		//�L�[�{�[�h���̎擾�J�n-----------------
	devkeyboard->Acquire();
	//----------------------------

	//�S�L�[�̓��͏�Ԃ��擾����---------------------------
	KeyInit();
	result = devkeyboard->GetDeviceState(sizeof(key), key);
	//----------------------------

#pragma endregion �L�[�{�[�h������
		//----------------------------------------

}

//Input::Input(HRESULT& result, WNDCLASSEX w, HWND hwnd)
//{
//	//���͌n������--------------
//#pragma region ���͌n������
//
//
//	result = DirectInput8Create(w.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);
//	assert(SUCCEEDED(result));
//	//�L�[�{�[�h�f�o�C�X�̐���-----------------
//#pragma region �L�[�{�[�h�f�o�C�X�̐���
//
//
//	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
//	assert(SUCCEEDED(result));
//
//#pragma endregion �L�[�{�[�h�f�o�C�X�̐���
//	//--------------------------
//
//	//���̓f�[�^�`���Z�b�g--------------------------------
//	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);
//	assert(SUCCEEDED(result));
//	//---------------------------------
//
//	//�r�����x������-------------------------------------------------------------
//	result = devkeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
//	assert(SUCCEEDED(result));
//
//#pragma endregion ���͌n������
//	//----------------
//}


Input* MCB::Input::GetInstance()
{
	static Input* instance = new Input;
	return instance;
}

void MCB::Input::DeleteInstace()
{
	delete Input::GetInstance();
}

Input* MCB::Input::GetInitInstance(HRESULT& result, WNDCLASSEX w, HWND hwnd)
{
	static Input* instance = Input::GetInstance();
	instance->Init(result, w, hwnd);
	return instance;
}

void Input::KeyInit()
{
	for (int i = 0; i < 256; i++)
	{
		oldkey[i] = key[i];
	}

}

bool Input::IsKeyDown(int keyNum)
{
		if (key[keyNum])
		{
			return true;
		}
		return false;
}

bool Input::IsKeyNDown(int keyNum)
{
	if (!key[keyNum])
	{
		return true;
	}
	return false;
}

bool Input::IsKeyPress(int keyNum)
{
	if (key[keyNum] && oldkey[keyNum])
	{
		return true;
	}
	return false;
}

bool Input::IsKeyTrigger(int keyNum)
{
	if (key[keyNum] && !oldkey[keyNum])
	{
		return true;
	}
	return false;
}

bool Input::IsKeyRelease(int keyNum)
{
	if (!key[keyNum] && oldkey[keyNum])
	{
		return true;
	}
	return false;
}

