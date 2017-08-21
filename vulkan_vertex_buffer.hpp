#pragma once

#include "common_includes.hpp"
#include "vulkan_buffer.hpp"
#include "vulkan_device.hpp"
#include "vulkan_physical_device.hpp"
#include "vulkan_vertex.hpp"

class VulkanVertexBuffer {
private:
	const VulkanPhysicalDevice& physicalDevice;
	const VulkanDevice& device;

	std::vector<VulkanVertex> vertexData;

	VulkanBuffer* vertexBuffer;

public:
	VulkanVertexBuffer(const VulkanPhysicalDevice& physicalDevice, const VulkanDevice& device, const std::vector<VulkanVertex>& vertexData);
	~VulkanVertexBuffer();

	unsigned int getVertexCount() const { return vertexData.size(); }
	const VulkanBuffer& getBuffer() const { return *vertexBuffer; }
};