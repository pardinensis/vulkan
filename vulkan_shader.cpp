#include "vulkan_shader.hpp"

VulkanShader::VulkanShader(const VulkanDevice& device)
		: device(device) {
	// nothing to do here
}

void VulkanShader::addShaderModule(const std::string &filename, VkShaderStageFlagBits stage) {
	std::shared_ptr<VulkanShaderModule> shaderModule = std::make_shared<VulkanShaderModule>(filename, device);
	shaderModules.push_back(shaderModule);

	VkPipelineShaderStageCreateInfo shaderStageCreateInfo = {};
	shaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStageCreateInfo.stage = stage;
	shaderStageCreateInfo.module = shaderModule->getVkShaderModule();
	shaderStageCreateInfo.pName = "main";
	shaderStages.push_back(shaderStageCreateInfo);
}