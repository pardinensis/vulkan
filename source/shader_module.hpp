#pragma once

#include "common_includes.hpp"

#include "device.hpp"

class ShaderModule {
private:
	const Device& vulkanDevice;
	VkShaderModule shaderModule;

public:
	ShaderModule(const std::string& filename, const Device& vulkanDevice);
	~ShaderModule();

	const VkShaderModule& getVkShaderModule() const { return shaderModule; }
};