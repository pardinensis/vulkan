#pragma once

#include "common_includes.hpp"
#include "vulkan_device.hpp"
#include "vulkan_image.hpp"
#include "vulkan_physical_device.hpp"

class VulkanBuffer {
private:
	const VulkanDevice& device;

	VkBuffer buffer;
	VkDeviceMemory deviceMemory;

public:
	static uint32_t findMemoryType(const VulkanDevice& device, uint32_t typeFilter, VkMemoryPropertyFlags properties);

public:
	VulkanBuffer(const VulkanDevice& device, VkDeviceSize bufferSize, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
	~VulkanBuffer();

	const VkBuffer& getVkBuffer() const { return buffer; }
	const VkDeviceMemory& getVkDeviceMemory() const { return deviceMemory; }

	void copyTo(const VulkanBuffer& destinationBuffer, VkDeviceSize size);
	void copyToImage(const VulkanImage& destinationImage, uint32_t width, uint32_t height);
	void fill(VkDeviceSize size, void* data);
};