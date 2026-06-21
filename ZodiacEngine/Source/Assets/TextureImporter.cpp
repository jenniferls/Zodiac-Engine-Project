#include <Zodiacpch.h>
#include "TextureImporter.h"
#include "ktx.h"

bool Zodiac::TextureImporter::LoadTexture(const char* filePath, VulkanTexture& texture, VulkanDevice* device) {
    ktxTexture* tex;
    ktx_error_code_e result = ktxTexture_CreateFromNamedFile(filePath, KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT, &tex);
    if (result != KTX_SUCCESS) {
        std::cerr << "Failed to load texture: " << ktxErrorString(result) << std::endl;
        return false;
	}
    texture.Create(tex->pData, tex->baseWidth, tex->baseHeight, false, device);
	return true;
}
