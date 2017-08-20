#pragma once

#include "common_includes.hpp"
#include "vulkan_device.hpp"

class VulkanSemaphore {
private:
	const VulkanDevice& device;

	VkSemaphore semaphore;

public:
	VulkanSemaphore(const VulkanDevice& device);
	~VulkanSemaphore();

	const VkSemaphore& getVkSemaphore() const { return semaphore; }
};