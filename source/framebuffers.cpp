#include "framebuffers.hpp"

Framebuffers::Framebuffers(const Device& device, const Swapchain& swapchain,
	const RenderPass& renderPass, const ImageView& depthImageView)
		: device(device), extent(swapchain.getVkExtent()) {
	const std::vector<ImageView*>& imageViews = swapchain.getImageViews();

	framebuffers.resize(imageViews.size());
	for (size_t i = 0; i < imageViews.size(); ++i) {
		std::array<VkImageView, 2> attachments = { 
			imageViews[i]->getVkImageView(),
			depthImageView.getVkImageView()
		};

		VkFramebufferCreateInfo framebufferCreateInfo = {};
		framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferCreateInfo.renderPass = renderPass.getVkRenderPass();
		framebufferCreateInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferCreateInfo.pAttachments = attachments.data();
		framebufferCreateInfo.width = extent.width;
		framebufferCreateInfo.height = extent.height;
		framebufferCreateInfo.layers = 1;

		if (vkCreateFramebuffer(device.getVkDevice(), &framebufferCreateInfo, nullptr, &framebuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create framebuffer");
		}
	}
}

Framebuffers::~Framebuffers() {
	for (size_t i = 0; i < framebuffers.size(); ++i) {
		vkDestroyFramebuffer(device.getVkDevice(), framebuffers[i], nullptr);
	}
}