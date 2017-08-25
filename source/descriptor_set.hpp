#pragma once

#include "common_includes.hpp"

#include "device.hpp"
#include "image_view.hpp"
#include "texture_sampler.hpp"
#include "uniform_buffer.hpp"

class DescriptorSet {
private:
	const Device& device;

	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorSet descriptorSet;

public:
	DescriptorSet(const Device& device, const UniformBuffer& uniformBuffer,
		const ImageView& imageView, const TextureSampler& sampler);
	~DescriptorSet();

	const VkDescriptorSetLayout& getVkDescriptorSetLayout() const { return descriptorSetLayout; }
	const VkDescriptorSet& getVkDescriptorSet() const { return descriptorSet; }
};