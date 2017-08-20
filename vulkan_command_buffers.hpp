#pragma once

#include "common_includes.hpp"
#include "vulkan_device.hpp"
#include "vulkan_framebuffers.hpp"
#include "vulkan_pipeline.hpp"

class VulkanCommandBuffers {
private:
	const VulkanDevice& device;

	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;

public:
	VulkanCommandBuffers(const VulkanDevice& device, const VulkanRenderPass& renderPass, const VulkanPipeline& pipeline,
		const VulkanFramebuffers& framebuffers, int graphicsFamily);
	~VulkanCommandBuffers();

	void submit(uint32_t imageIndex, const VulkanSemaphore& imageAquiredSemaphore, const VulkanSemaphore& renderFinishedSemaphore);
};