#pragma once

#include "common_includes.hpp"
#include "vulkan_device.hpp"

class VulkanImageView {

private:
	const VulkanDevice& device;

	VkImageView imageView;

public:
	VulkanImageView(const VulkanDevice& device, const VkImage& image, const VkFormat& format);
	~VulkanImageView();

	const VkImageView& getVkImageView() const { return imageView; }
};