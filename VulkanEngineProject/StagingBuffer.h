#pragma once
#include <vulkan\vulkan.h>

#include "VulkanBuffer.h"
#include "VulkanDevice.h"

class StagingBuffer : public Zodiac::VulkanBuffer {
public:
	StagingBuffer();
	~StagingBuffer();

	virtual void SetData() override;
	virtual void SetData(unsigned int count) override;
	virtual void SetData(unsigned int startIndex, unsigned int count) override;

private:

};