#pragma once

#include <memory>

#include "common_includes.hpp"

#include "device.hpp"
#include "shader_module.hpp"

class Shader {
private:
	const Device& device;
	std::vector<std::shared_ptr<ShaderModule>> shaderModules;
	std::vector<VkPipelineShaderStageCreateInfo> shaderStages; 

public:
	Shader(const Device& device);

	void addShaderModule(const std::string& filename, VkShaderStageFlagBits stage);

	const std::vector<VkPipelineShaderStageCreateInfo>& getVkShaderStages() const { return shaderStages; }
};