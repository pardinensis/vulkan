#pragma once

#include "common_includes.hpp"
#include "vulkan_device.hpp"

class VulkanRenderPass {
private:
	const VulkanDevice& device;

	VkRenderPass renderPass;

public:
	VulkanRenderPass(const VulkanDevice& device, const VkFormat imageFormat);
	~VulkanRenderPass();

	const VkRenderPass& getVkRenderPass() const { return renderPass; }
};