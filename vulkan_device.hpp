#pragma once

#include "common_includes.hpp"
#include "vulkan_physical_device.hpp"

class VulkanDevice {
private:
	const VulkanPhysicalDevice& physicalDevice;

	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;

	VkCommandPool commandPool;
	VkDescriptorPool descriptorPool;

public:
	VulkanDevice(VulkanPhysicalDevice& physicalDevice);
	~VulkanDevice();

	const VulkanPhysicalDevice& getPhysicalDevice() const { return physicalDevice; }

	const VkDevice& getVkDevice() const { return device; }
	const VkQueue& getVkGraphicsQueue() const { return graphicsQueue; }
	const VkQueue& getVkPresentQueue() const { return presentQueue; }
	const VkCommandPool& getVkCommandPool() const { return commandPool; }
	const VkDescriptorPool& getVkDescriptorPool() const { return descriptorPool; }
};