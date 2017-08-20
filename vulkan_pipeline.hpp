#pragma once

#include "common_includes.hpp"
#include "vulkan_device.hpp"
#include "vulkan_physical_device.hpp"
#include "vulkan_shader_module.hpp"
#include "vulkan_swapchain.hpp"

class VulkanPipeline {
private:
	const VulkanDevice& vulkanDevice;
	const VulkanSwapchain& vulkanSwapchain;

	VkPipelineLayout pipelineLayout;
	VkRenderPass renderPass;
	VkPipeline pipeline;
	std::vector<VkFramebuffer> framebuffers;
	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;

	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderFinishedSemaphore;

public:
	VulkanPipeline(const VulkanPhysicalDevice& physicalDevice, const VulkanDevice& vulkanDevice, const VulkanSwapchain& vulkanSwapchain);
	~VulkanPipeline();

	void drawFrame();

public:
	static std::shared_ptr<VulkanPipeline> create(const VulkanPhysicalDevice& physicalDevice, const VulkanDevice& vulkanDevice, const VulkanSwapchain& vulkanSwapchain);
};