#pragma once

#include "common_includes.hpp"

#include "device.hpp"

class RenderPass {
private:
	const Device& device;

	VkRenderPass renderPass;

public:
	RenderPass(const Device& device, const VkFormat imageFormat);
	~RenderPass();

	const VkRenderPass& getVkRenderPass() const { return renderPass; }
};