#pragma once

#include "common_includes.hpp"
#include "vulkan_device.hpp"

class VulkanImage {
private:
	const VulkanDevice& device;
	const VkFormat& format;

	VkImage image;
	VkDeviceMemory imageMemory;

public:
	VulkanImage(const VulkanDevice& device, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
		VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
	~VulkanImage();

	void transitionLayout(VkImageLayout oldLayout, VkImageLayout newLayout);

	const VkFormat& getVkFormat() const { return format; }
	const VkImage& getVkImage() const { return image; }
	const VkDeviceMemory& getVkDeviceMemory() const { return imageMemory; }
};