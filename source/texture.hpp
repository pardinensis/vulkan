#pragma once

#include <stb_image.h>

#include "common_includes.hpp"

#include "buffer.hpp"
#include "image.hpp"
#include "image_view.hpp"

class Texture {
private:
	const Device& device;

	int width;
	int height;
	int channels;

	Image* image;
	ImageView* imageView;

public:
	Texture(const Device& device, const std::string& filename);
	~Texture();

	const Image& getImage() const { return *image; }
	const ImageView& getImageView() const { return *imageView; }
};