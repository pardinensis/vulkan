#pragma once

#include "common_includes.hpp"
#include "vulkan_render_pass.hpp"
#include "vulkan_swapchain.hpp"

class VulkanFramebuffers {
private:
	const VulkanDevice& device;
	const VkExtent2D extent;
	
	std::vector<VkFramebuffer> framebuffers;

public:
	VulkanFramebuffers(const VulkanDevice& device, const VulkanSwapchain& swapchain, const VulkanRenderPass& renderPass);
	~VulkanFramebuffers();

	int getFramebufferCount() const { return framebuffers.size(); }
	const std::vector<VkFramebuffer>& getVkFramebuffers() const { return framebuffers; }
	const VkExtent2D& getVkExtent() const { return extent; }
};