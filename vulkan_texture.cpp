#include "vulkan_texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "contrib/stb_image.h"


VulkanTexture::VulkanTexture(const VulkanDevice& device, const std::string& filename) 
		: device(device) {
	stbi_uc* pixels = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);
	if (!pixels) {
		throw std::runtime_error("failed to load texture image: " + filename);
	}

	VkDeviceSize imageSize = width * height * 4;

	// copy cpu data to staging buffer
	VulkanBuffer stagingBuffer(device, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	stagingBuffer.fill(imageSize, pixels);
	stbi_image_free(pixels);

	// create empty image
	image = new VulkanImage(device, width, height, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	// copy staging buffer to image
	image->transitionLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	stagingBuffer.copyToImage(*image, static_cast<uint32_t>(width), static_cast<uint32_t>(height));
	image->transitionLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	// create image view
	imageView = new VulkanImageView(device, image->getVkImage(), VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT);
}

VulkanTexture::~VulkanTexture() {
	delete imageView;
	delete image;
}