#include "vulkan_descriptor_set.hpp"

VulkanDescriptorSet::VulkanDescriptorSet(const VulkanDevice& device, const VulkanUniformBuffer& uniformBuffer) 
		: device(device) {
	// setup descriptor set layout
	VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {};
	descriptorSetLayoutBinding.binding = 0;
	descriptorSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorSetLayoutBinding.descriptorCount = 1;
	descriptorSetLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	descriptorSetLayoutBinding.pImmutableSamplers = nullptr;

	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
	descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptorSetLayoutCreateInfo.bindingCount = 1;
	descriptorSetLayoutCreateInfo.pBindings = &descriptorSetLayoutBinding;
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

	VkWriteDescriptorSet writeDescriptorSet = {};
	writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDescriptorSet.dstSet = descriptorSet;
	writeDescriptorSet.dstBinding = 0;
	writeDescriptorSet.dstArrayElement = 0;
	writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	writeDescriptorSet.descriptorCount = 1;
	writeDescriptorSet.pBufferInfo = &descriptorBufferInfo;
	writeDescriptorSet.pImageInfo = nullptr;
	writeDescriptorSet.pTexelBufferView = nullptr;
	vkUpdateDescriptorSets(device.getVkDevice(), 1, &writeDescriptorSet, 0, nullptr);
}

VulkanDescriptorSet::~VulkanDescriptorSet() {
	vkDestroyDescriptorSetLayout(device.getVkDevice(), descriptorSetLayout, nullptr);
}