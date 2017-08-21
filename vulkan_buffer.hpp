#pragma once

#include "common_includes.hpp"
#include "vulkan_device.hpp"
#include "vulkan_physical_device.hpp"


class VulkanBuffer {
private:
	const VulkanPhysicalDevice& physicalDevice;
	const VulkanDevice& device;

	VkBuffer buffer;
	VkDeviceMemory deviceMemory;

private:
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

public:
	VulkanBuffer(const VulkanPhysicalDevice& physicalDevice, const VulkanDevice& device, VkDeviceSize bufferSize,
		VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
	~VulkanBuffer();

	const VkBuffer& getVkBuffer() const { return buffer; }
	const VkDeviceMemory& getVkDeviceMemory() const { return deviceMemory; }

	void copyTo(const VulkanBuffer& destinationBuffer, VkDeviceSize size);
};