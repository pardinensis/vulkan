#pragma once

#include "common_includes.hpp"
#include "vulkan_device.hpp"
#include "vulkan_physical_device.hpp"
#include "vulkan_vertex.hpp"

class VulkanVertexBuffer {
private:
	const VulkanPhysicalDevice& physicalDevice;
	const VulkanDevice& device;

	VkBuffer vertexBuffer;
	VkDeviceMemory deviceMemory;

	std::vector<VulkanVertex> vertexData;

private:
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

public:
	VulkanVertexBuffer(const VulkanPhysicalDevice& physicalDevice, const VulkanDevice& device, const std::vector<VulkanVertex>& vertexData);
	~VulkanVertexBuffer();

	const VkBuffer& getVkBuffer() const { return vertexBuffer; }
	unsigned int getVertexCount() const { return vertexData.size(); }
};