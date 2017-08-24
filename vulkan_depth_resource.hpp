#pragma once

#include "common_includes.hpp"
#include "vulkan_device.hpp"
#include "vulkan_image.hpp"
#include "vulkan_image_view.hpp"

class VulkanDepthResource {

private:
	const VulkanDevice& device;

	VulkanImage* image;
	VulkanImageView* imageView;

public:
	VulkanDepthResource(const VulkanDevice& device, const VkExtent2D& extent);
	~VulkanDepthResource();

	const VulkanImageView& getImageView() const { return *imageView; }
};