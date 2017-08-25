#pragma once

#include "common_includes.hpp"

#include "device.hpp"

class Image {
private:
	const Device& device;
	const VkFormat& format;

	VkImage image;
	VkDeviceMemory imageMemory;

public:
	Image(const Device& device, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
		VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
	~Image();

	void transitionLayout(VkImageLayout oldLayout, VkImageLayout newLayout);

	const VkFormat& getVkFormat() const { return format; }
	const VkImage& getVkImage() const { return image; }
	const VkDeviceMemory& getVkDeviceMemory() const { return imageMemory; }
};