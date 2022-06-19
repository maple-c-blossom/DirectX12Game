#include "TextureFile.h"

using namespace DirectX;
using namespace std;

MCB::TextureFile::~TextureFile()
{
    scratchImg.Release();
  
}

HRESULT MCB::TextureFile::LoadTexture(const wchar_t *FileName, DirectX::WIC_FLAGS flag)
{
	return LoadFromWICFile(FileName, flag, &metadata, scratchImg);
}

HRESULT MCB::TextureFile::LoadTexture(const std::string& directoryPath, const std::string& filename)
{
    string filepath = directoryPath + filename;

    wchar_t wfilepath[128];
    int iBufferSize = MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, wfilepath, _countof(wfilepath));

    return LoadFromWICFile(wfilepath, WIC_FLAGS_NONE,&metadata,scratchImg);
}