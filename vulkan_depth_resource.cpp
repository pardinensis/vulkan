#include "vulkan_depth_resource.hpp"

VulkanDepthResource::VulkanDepthResource(const VulkanDevice& device, const VkExtent2D& extent)
		: device(device) {
	image = new VulkanImage(device, extent.width, extent.height, VK_FORMAT_D32_SFLOAT,
		VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	imageView = new VulkanImageView(device, image->getVkImage(), VK_FORMAT_D32_SFLOAT, VK_IMAGE_ASPECT_DEPTH_BIT);
	image->transitionLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
}

VulkanDepthResource::~VulkanDepthResource() {
	delete imageView;
	delete image;
}