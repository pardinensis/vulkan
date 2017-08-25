#pragma once

#include "common_includes.hpp"

#include "physical_device.hpp"

class Device {
private:
	const PhysicalDevice& physicalDevice;

	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;

	VkCommandPool commandPool;
	VkDescriptorPool descriptorPool;

public:
	Device(PhysicalDevice& physicalDevice);
	~Device();

	const PhysicalDevice& getPhysicalDevice() const { return physicalDevice; }

	const VkDevice& getVkDevice() const { return device; }
	const VkQueue& getVkGraphicsQueue() const { return graphicsQueue; }
	const VkQueue& getVkPresentQueue() const { return presentQueue; }
	const VkCommandPool& getVkCommandPool() const { return commandPool; }
	const VkDescriptorPool& getVkDescriptorPool() const { return descriptorPool; }
};