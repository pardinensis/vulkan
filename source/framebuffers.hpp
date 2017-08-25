#pragma once

#include "common_includes.hpp"

#include "render_pass.hpp"
#include "swapchain.hpp"

class Framebuffers {
private:
	const Device& device;
	const VkExtent2D extent;
	
	std::vector<VkFramebuffer> framebuffers;

public:
	Framebuffers(const Device& device, const Swapchain& swapchain,
		const RenderPass& renderPass, const ImageView& depthImageView);
	~Framebuffers();

	int getFramebufferCount() const { return framebuffers.size(); }
	const std::vector<VkFramebuffer>& getVkFramebuffers() const { return framebuffers; }
	const VkExtent2D& getVkExtent() const { return extent; }
};