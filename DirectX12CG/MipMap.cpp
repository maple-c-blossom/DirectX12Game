#include "MipMap.h"

using namespace DirectX;


HRESULT MCB::MipMap::GenerateMipMap(DirectX::ScratchImage* scratchimg, DirectX::TEX_FILTER_FLAGS flag, size_t levels, TexMetadata &metadata)
{
	HRESULT result;
	//�~�b�v�}�b�v����
	GenerateMipMaps(scratchimg->GetImages(), scratchimg->GetImageCount(), scratchimg->GetMetadata(), flag, levels, mipChain);
	if (SUCCEEDED(result))
	{
		*scratchimg = std::move(mipChain);
		metadata = scratchimg->GetMetadata();
	}
	metadata.format = MakeSRGB(metadata.format);
	
	return result;
}
