#include "shader.hpp"

Shader::Shader(const Device& device)
		: device(device) {
	// nothing to do here
}

void Shader::addShaderModule(const std::string &filename, VkShaderStageFlagBits stage) {
	std::shared_ptr<ShaderModule> shaderModule = std::make_shared<ShaderModule>(filename, device);
	shaderModules.push_back(shaderModule);

	VkPipelineShaderStageCreateInfo shaderStageCreateInfo = {};
	shaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStageCreateInfo.stage = stage;
	shaderStageCreateInfo.module = shaderModule->getVkShaderModule();
	shaderStageCreateInfo.pName = "main";
	shaderStages.push_back(shaderStageCreateInfo);
}