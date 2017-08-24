#include "vulkan_framebuffers.hpp"

VulkanFramebuffers::VulkanFramebuffers(const VulkanDevice& device, const VulkanSwapchain& swapchain, const VulkanRenderPass& renderPass)
		: device(device), extent(swapchain.getVkExtent()) {
	const std::vector<VulkanImageView*>& imageViews = swapchain.getImageViews();

	framebuffers.resize(imageViews.size());
	for (size_t i = 0; i < imageViews.size(); ++i) {
		VkImageView attachments[] = { imageViews[i]->getVkImageView() };

		VkFramebufferCreateInfo framebufferCreateInfo = {};
		framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferCreateInfo.renderPass = renderPass.getVkRenderPass();
		framebufferCreateInfo.attachmentCount = 1;
		framebufferCreateInfo.pAttachments = attachments;
		framebufferCreateInfo.width = extent.width;
		framebufferCreateInfo.height = extent.height;
		framebufferCreateInfo.layers = 1;

		if (vkCreateFramebuffer(device.getVkDevice(), &framebufferCreateInfo, nullptr, &framebuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create framebuffer");
		}
	}
}

VulkanFramebuffers::~VulkanFramebuffers() {
	for (size_t i = 0; i < framebuffers.size(); ++i) {
		vkDestroyFramebuffer(device.getVkDevice(), framebuffers[i], nullptr);
	}
}