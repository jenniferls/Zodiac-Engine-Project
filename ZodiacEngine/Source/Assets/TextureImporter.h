#pragma once
#include "VulkanTexture.h"

namespace Zodiac {
    class TextureImporter {
        public:
        TextureImporter() = default;
        ~TextureImporter() = default;

        bool LoadTexture(const char* filePath, VulkanTexture& texture, VulkanDevice* device);

        private:

    };
}
