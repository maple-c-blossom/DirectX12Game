#pragma once
#include <Windows.h>

namespace MCB
{
    class DxWindow
    {
    private:
        DxWindow() {};
        DxWindow(const DxWindow& dx12) {};
        DxWindow& operator=(const DxWindow& dx12) {};
        ~DxWindow();

    public:
        static DxWindow* GetInstance();
        static DxWindow* GetInitInstance();
        static void DeleteInstance();

        // ウィンドウサイズ
        const int window_width = 1280;  // 横幅
        const int window_height = 720;  // 縦幅
        bool breakFlag = false;


        MSG msg{}; //メッセージここから
        WNDCLASSEX window{}; // ウィンドウクラスの設定
            // ウィンドウサイズ{ X座標 Y座標 横幅 縦幅 }
        RECT wrc = { 0, 0, window_width, window_height };
        HWND hwnd;
        const wchar_t* windowName = L"LE2A_02_アベ_リュウタ_CG";


        // ウィンドウプロシージャ 
        static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

        void messageUpdate();
        void Init();

    };
}


