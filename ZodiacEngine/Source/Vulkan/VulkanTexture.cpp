#include "Zodiacpch.h"
#include "VulkanTexture.h"
#include "Validation.h"
#include "Initializers.h"

Zodiac::VulkanTexture::VulkanTexture() {
    
}

Zodiac::VulkanTexture::~VulkanTexture() {
    
}

void Zodiac::VulkanTexture::Create(const void* pPixels, uint32_t imageWidth, uint32_t imageHeight, bool isCubemap, VulkanDevice* device){
    VkFormat format = VK_FORMAT_R8G8B8A8_SRGB; //TODO: Format should be read from outside this function
    VkExtent3D extent = {
        .width = imageWidth,
        .height = imageHeight,
        .depth = 1
    };
    
    VkImageCreateFlags imageCreateFlags = 0;
    uint arrayLayers = 1u;
    if(isCubemap){
        imageCreateFlags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
        arrayLayers = 6u;
    }
    
    VkImageUsageFlagBits usage = (VkImageUsageFlagBits)(VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
    VkMemoryPropertyFlagBits propertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    
    VkImageCreateInfo imageCreateInfo = Initializers::ImageCreateInfo(format, extent, usage, VK_IMAGE_TYPE_2D, imageCreateFlags, arrayLayers);
    ErrorCheck(vkCreateImage(*device->GetDevice(), &imageCreateInfo, NULL, &m_image));
}
