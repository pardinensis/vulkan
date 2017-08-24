#pragma once

#include "contrib/stb_image.h"

#include "common_includes.hpp"
#include "vulkan_buffer.hpp"
#include "vulkan_image.hpp"
#include "vulkan_image_view.hpp"

class VulkanTexture {
private:
	const VulkanDevice& device;

	int width;
	int height;
	int channels;

	VulkanImage* image;
	VulkanImageView* imageView;

public:
	VulkanTexture(const VulkanDevice& device, const std::string& filename);
	~VulkanTexture();

	const VulkanImage& getImage() const { return *image; }
	const VulkanImageView& getImageView() const { return *imageView; }
};