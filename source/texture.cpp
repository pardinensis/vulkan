#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


Texture::Texture(const Device& device, const std::string& filename) 
		: device(device) {
	stbi_uc* pixels = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);
	if (!pixels) {
		throw std::runtime_error("failed to load texture image: " + filename);
	}

	VkDeviceSize imageSize = width * height * 4;

	// copy cpu data to staging buffer
	Buffer stagingBuffer(device, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	stagingBuffer.fill(imageSize, pixels);
	stbi_image_free(pixels);

	// create empty image
	image = new Image(device, width, height, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	// copy staging buffer to image
	image->transitionLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	stagingBuffer.copyToImage(*image, static_cast<uint32_t>(width), static_cast<uint32_t>(height), VK_IMAGE_ASPECT_COLOR_BIT);
	image->transitionLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	// create image view
	imageView = new ImageView(device, image->getVkImage(), VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT);
}

Texture::~Texture() {
	delete imageView;
	delete image;
}