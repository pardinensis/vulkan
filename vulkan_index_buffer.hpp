#pragma once

#include "common_includes.hpp"
#include "vulkan_buffer.hpp"
#include "vulkan_device.hpp"

class VulkanIndexBuffer {
private:
	const VulkanDevice& device;

	std::vector<uint16_t> indexData;

	VulkanBuffer* indexBuffer;

public:
	VulkanIndexBuffer(const VulkanDevice& device, const std::vector<uint16_t> indexData);
	~VulkanIndexBuffer();

	unsigned int getIndexCount() const { return indexData.size(); }
	const VulkanBuffer& getBuffer() const { return *indexBuffer; }
};