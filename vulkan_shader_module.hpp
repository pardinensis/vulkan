#pragma once

#include "common_includes.hpp"
#include "vulkan_device.hpp"

class VulkanShaderModule {
private:
	const VulkanDevice& vulkanDevice;
	VkShaderModule shaderModule;

public:
	VulkanShaderModule(const std::string& filename, const VulkanDevice& vulkanDevice);
	~VulkanShaderModule();

	const VkShaderModule& getVkShaderModule() const { return shaderModule; }
};