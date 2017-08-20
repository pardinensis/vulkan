#pragma once

#include "common_includes.hpp"
#include "vulkan_device.hpp"
#include "vulkan_shader_module.hpp"

#include <memory>

class VulkanShader {
private:
	const VulkanDevice& device;
	std::vector<std::shared_ptr<VulkanShaderModule>> shaderModules;
	std::vector<VkPipelineShaderStageCreateInfo> shaderStages; 

public:
	VulkanShader(const VulkanDevice& device);

	void addShaderModule(const std::string& filename, VkShaderStageFlagBits stage);

	const std::vector<VkPipelineShaderStageCreateInfo>& getVkShaderStages() const { return shaderStages; }
};