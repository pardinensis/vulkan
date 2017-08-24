#include "vulkan_descriptor_set.hpp"

VulkanDescriptorSet::VulkanDescriptorSet(const VulkanDevice& device, const VulkanUniformBuffer& uniformBuffer,
	const VulkanImageView& imageView, const VulkanTextureSampler& sampler)
		: device(device) {
	// setup descriptor set layout for uniform buffer
	VkDescriptorSetLayoutBinding uniformBufferLayoutBinding = {};
	uniformBufferLayoutBinding.binding = 0;
	uniformBufferLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uniformBufferLayoutBinding.descriptorCount = 1;
	uniformBufferLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	uniformBufferLayoutBinding.pImmutableSamplers = nullptr;

	// setup descriptor set layout for texture sampler
	VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
	samplerLayoutBinding.binding = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	samplerLayoutBinding.pImmutableSamplers = nullptr;

	std::array<VkDescriptorSetLayoutBinding, 2> descriptorSetLayoutBindings = {
		uniformBufferLayoutBinding, samplerLayoutBinding
	};

	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
	descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptorSetLayoutCreateInfo.bindingCount = static_cast<uint32_t>(descriptorSetLayoutBindings.size());
	descriptorSetLayoutCreateInfo.pBindings = descriptorSetLayoutBindings.data();
	if (vkCreateDescriptorSetLayout(device.getVkDevice(), &descriptorSetLayoutCreateInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout");
	}

	// setup descriptor set
	VkDescriptorSetLayout descriptorSetLayouts[] = { descriptorSetLayout };
	VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {};
	descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	descriptorSetAllocateInfo.descriptorPool = device.getVkDescriptorPool();
	descriptorSetAllocateInfo.descriptorSetCount = 1;
	descriptorSetAllocateInfo.pSetLayouts = descriptorSetLayouts;
	if (vkAllocateDescriptorSets(device.getVkDevice(), &descriptorSetAllocateInfo, &descriptorSet) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor set");
	}

	VkDescriptorBufferInfo descriptorBufferInfo = {};
	descriptorBufferInfo.buffer = uniformBuffer.getBuffer().getVkBuffer();
	descriptorBufferInfo.offset = 0;
	descriptorBufferInfo.range = sizeof(VulkanUniformBufferObject);

	std::array<VkWriteDescriptorSet, 2> writeDescriptorSets = {};
	writeDescriptorSets[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDescriptorSets[0].dstSet = descriptorSet;
	writeDescriptorSets[0].dstBinding = 0;
	writeDescriptorSets[0].dstArrayElement = 0;
	writeDescriptorSets[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	writeDescriptorSets[0].descriptorCount = 1;
	writeDescriptorSets[0].pBufferInfo = &descriptorBufferInfo;

	VkDescriptorImageInfo descriptorImageInfo = {};
	descriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	descriptorImageInfo.imageView = imageView.getVkImageView();
	descriptorImageInfo.sampler = sampler.getVkSampler();

	writeDescriptorSets[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDescriptorSets[1].dstSet = descriptorSet;
	writeDescriptorSets[1].dstBinding = 1;
	writeDescriptorSets[1].dstArrayElement = 0;
	writeDescriptorSets[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	writeDescriptorSets[1].descriptorCount = 1;
	writeDescriptorSets[1].pImageInfo = &descriptorImageInfo;

	vkUpdateDescriptorSets(device.getVkDevice(), static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);
}

VulkanDescriptorSet::~VulkanDescriptorSet() {
	vkDestroyDescriptorSetLayout(device.getVkDevice(), descriptorSetLayout, nullptr);
}