#pragma once
#include "MipMap.h"
#include "TexImgData.h"
#include "TextureBuffer.h"
#include "TextureFile.h"
#include "Descriptor.h"

namespace MCB
{
	class Texture
	{
	public:
		MipMap mipMap;
		TexImgData texImg;
		TextureBuffer texBuff;
		TextureFile texfile;
		ShaderResource* srvptr = nullptr;
		unsigned short int incrementNum = 0;


		~Texture();

		void CreateTexture(Dx12 dx12, const wchar_t* FileName,unsigned short int incrementNum, ShaderResource* srv);

		void CreateTexture(Dx12 dx12, const std::string& directoryPath, const std::string& filename, unsigned short int incrementNum, ShaderResource* srv);

		void CreateTexture(Dx12 dx12, const wchar_t* FileName, ShaderResource* srv);

		void CreateTexture(Dx12 dx12, const std::string& directoryPath, const std::string& filename, ShaderResource* srv);


		void SetSrvHeap(Dx12 dx12);

	};

}