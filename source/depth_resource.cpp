#include "depth_resource.hpp"

#include "buffer.hpp"

DepthResource::DepthResource(const Device& device, const VkExtent2D& extent)
		: device(device) {
	image = new Image(device, extent.width, extent.height, VK_FORMAT_D32_SFLOAT,
		VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	imageView = new ImageView(device, image->getVkImage(), VK_FORMAT_D32_SFLOAT, VK_IMAGE_ASPECT_DEPTH_BIT);
	image->transitionLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
}

std::vector<float> DepthResource::downloadDepthImage() {
	uint32_t width = image->getWidth();
	uint32_t height = image->getHeight();
	VkDeviceSize imageSize = width * height * 4;

	Buffer unstagingBuffer(device, imageSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	image->transitionLayout(VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
	unstagingBuffer.copyFromImage(*image, width, height, VK_IMAGE_ASPECT_DEPTH_BIT);
	image->transitionLayout(VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

	std::vector<float> depthData(width * height);
	unstagingBuffer.retrieve(imageSize, depthData.data());

	return depthData;
}

DepthResource::~DepthResource() {
	delete imageView;
	delete image;
}