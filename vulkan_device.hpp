#pragma once

#include "common_includes.hpp"
#include "vulkan_physical_device.hpp"

class VulkanDevice {
private:
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;

public:
	VulkanDevice(VulkanPhysicalDevice& physicalDevice);
	~VulkanDevice();

	const VkDevice& getVkDevice() const { return device; }
	const VkQueue& getVkGraphicsQueue() const { return graphicsQueue; }
	const VkQueue& getVkPresentQueue() const { return presentQueue; }

public:
	static std::shared_ptr<VulkanDevice> create(VulkanPhysicalDevice& physicalDevice);
};