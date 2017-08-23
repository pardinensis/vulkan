#pragma once

#include "common_includes.hpp"
#include "vulkan_buffer.hpp"
#include "vulkan_device.hpp"
#include "vulkan_physical_device.hpp"

struct VulkanUniformBufferObject {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};

class VulkanUniformBuffer {
private:
	const VulkanPhysicalDevice& physicalDevice;
	const VulkanDevice& device;

	VulkanBuffer* uniformBuffer;

public:
	VulkanUniformBuffer(const VulkanPhysicalDevice& physicalDevice, const VulkanDevice& device);
	~VulkanUniformBuffer();

	const VulkanBuffer& getBuffer() const { return *uniformBuffer; }

	void update(const VkExtent2D& extent);
};