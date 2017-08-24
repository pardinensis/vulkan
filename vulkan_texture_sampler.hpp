#pragma once

#include "common_includes.hpp"
#include "vulkan_device.hpp"

class VulkanTextureSampler {

private:
	const VulkanDevice& device;

	VkSampler sampler;

public:
	VulkanTextureSampler(const VulkanDevice& device);
	~VulkanTextureSampler();

	const VkSampler& getVkSampler() const { return sampler; }
};