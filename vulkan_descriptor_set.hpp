#pragma once

#include "common_includes.hpp"
#include "vulkan_device.hpp"
#include "vulkan_uniform_buffer.hpp"

class VulkanDescriptorSet {
private:
	const VulkanDevice& device;

	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorSet descriptorSet;

public:
	VulkanDescriptorSet(const VulkanDevice& device, const VulkanUniformBuffer& uniformBuffer);
	~VulkanDescriptorSet();

	const VkDescriptorSetLayout& getVkDescriptorSetLayout() const { return descriptorSetLayout; }
	const VkDescriptorSet& getVkDescriptorSet() const { return descriptorSet; }
};