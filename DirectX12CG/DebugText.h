#pragma once
#include "Sprite.h"
#include "Dx12.h"
#include "DxWindow.h"
#include "Descriptor.h"
#include <string>

namespace MCB
{
	class DebugText
	{
	private:
		static const int maxCharCount = 0xff;//1フレ当たりの最大表示数
		static const int fontWidth = 9;//フォントの横幅
		static const int fontHeight = 18;//フォントの縦幅
		static const int fontLineCount = 14;//フォントの１行当たりの文字数
		char buffer[maxCharCount]{};
	public:
		Sprite sprite[maxCharCount];
		int spriteIndex = 0;
		Texture* debugfont;
		Dx12* dx12;

		void Init(Dx12 &dx,DxWindow window,Texture* texture);
		void Print( float x, float y, float scale, const char* text, ... );
		void AllDraw(ShaderResource descriptor);
	};

}

