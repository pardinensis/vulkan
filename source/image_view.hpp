#pragma once

#include "common_includes.hpp"

#include "device.hpp"

class ImageView {

private:
	const Device& device;

	VkImageView imageView;

public:
	ImageView(const Device& device, const VkImage& image, const VkFormat& format, const VkImageAspectFlags& aspect);
	~ImageView();

	const VkImageView& getVkImageView() const { return imageView; }
};