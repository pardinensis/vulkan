#include "depth_resource.hpp"

DepthResource::DepthResource(const Device& device, const VkExtent2D& extent)
		: device(device) {
	image = new Image(device, extent.width, extent.height, VK_FORMAT_D32_SFLOAT,
		VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	imageView = new ImageView(device, image->getVkImage(), VK_FORMAT_D32_SFLOAT, VK_IMAGE_ASPECT_DEPTH_BIT);
	image->transitionLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
}

DepthResource::~DepthResource() {
	delete imageView;
	delete image;
}