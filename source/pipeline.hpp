#pragma once

#include "common_includes.hpp"

#include "descriptor_set.hpp"
#include "device.hpp"
#include "physical_device.hpp"
#include "render_pass.hpp"
#include "shader.hpp"

class Pipeline {
private:
	const Device& device;

	VkPipelineLayout pipelineLayout;
	VkPipeline pipeline;

public:
	Pipeline(const Device& device, const RenderPass& renderPass, const VkExtent2D& vkExtent,
		const Shader& shader, const DescriptorSet& descriptorSet);
	~Pipeline();

	void drawFrame();

	const VkPipelineLayout& getVkPipelineLayout() const { return pipelineLayout; }
	const VkPipeline& getVkPipeline() const { return pipeline; }
};