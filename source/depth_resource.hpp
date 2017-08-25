#pragma once

#include "common_includes.hpp"

#include "device.hpp"
#include "image.hpp"
#include "image_view.hpp"

class DepthResource {

private:
	const Device& device;

	Image* image;
	ImageView* imageView;

public:
	DepthResource(const Device& device, const VkExtent2D& extent);
	~DepthResource();

	const ImageView& getImageView() const { return *imageView; }
};