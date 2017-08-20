#include "vulkan_shader_module.hpp"

#include <fstream>

VulkanShaderModule::VulkanShaderModule(const std::string& filename, const VulkanDevice& vulkanDevice)
		: vulkanDevice(vulkanDevice) {
	std::ifstream file(filename, std::ios::ate | std::ios::binary);
	if (!file.is_open()) {
		throw std::runtime_error("failed to open shader file: " + filename);
	}

	size_t fileSize = (size_t) file.tellg();
	std::vector<char> buffer(fileSize);
	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();

	VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
	shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderModuleCreateInfo.codeSize = buffer.size();
	shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(buffer.data());
	if (vkCreateShaderModule(vulkanDevice.getVkDevice(), &shaderModuleCreateInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shader module");
	}
}

VulkanShaderModule::~VulkanShaderModule() {
	vkDestroyShaderModule(vulkanDevice.getVkDevice(), shaderModule, nullptr);
}