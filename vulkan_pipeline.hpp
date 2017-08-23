#pragma once

#include "common_includes.hpp"
#include "vulkan_descriptor_set.hpp"
#include "vulkan_device.hpp"
#include "vulkan_physical_device.hpp"
#include "vulkan_render_pass.hpp"
#include "vulkan_shader.hpp"

class VulkanPipeline {
private:
	const VulkanDevice& device;

	VkPipelineLayout pipelineLayout;
	VkPipeline pipeline;

public:
	VulkanPipeline(const VulkanDevice& device, const VulkanRenderPass& renderPass, const VkExtent2D& vkExtent,
		const VulkanShader& shader, const VulkanDescriptorSet& descriptorSet);
	~VulkanPipeline();

	void drawFrame();

	const VkPipelineLayout& getVkPipelineLayout() const { return pipelineLayout; }
	const VkPipeline& getVkPipeline() const { return pipeline; }
};