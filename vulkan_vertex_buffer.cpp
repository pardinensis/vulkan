#include "vulkan_vertex_buffer.hpp"

VulkanVertexBuffer::VulkanVertexBuffer(const VulkanPhysicalDevice& physicalDevice, const VulkanDevice& device, const std::vector<VulkanVertex>& vertexData)
		: physicalDevice(physicalDevice), device(device), vertexData(vertexData) {

	VkBufferCreateInfo bufferCreateInfo = {};
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.size = sizeof(VulkanVertex) * vertexData.size();
	bufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(device.getVkDevice(), &bufferCreateInfo, nullptr, &vertexBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to create vertex buffer");
	}

	VkMemoryRequirements memoryRequirements;
	vkGetBufferMemoryRequirements(device.getVkDevice(), vertexBuffer, &memoryRequirements);

	VkMemoryAllocateInfo memoryAllocateInfo = {};
	memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryAllocateInfo.allocationSize = memoryRequirements.size;
	memoryAllocateInfo.memoryTypeIndex = findMemoryType(memoryRequirements.memoryTypeBits,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	if (vkAllocateMemory(device.getVkDevice(), &memoryAllocateInfo, nullptr, &deviceMemory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate vertex buffer memory");
	}

	vkBindBufferMemory(device.getVkDevice(), vertexBuffer, deviceMemory, 0);

	void* data;
	vkMapMemory(device.getVkDevice(), deviceMemory, 0, bufferCreateInfo.size, 0, &data);
	std::memcpy(data, vertexData.data(), static_cast<size_t>(bufferCreateInfo.size));
	vkUnmapMemory(device.getVkDevice(), deviceMemory);
}

VulkanVertexBuffer::~VulkanVertexBuffer() {
	vkDestroyBuffer(device.getVkDevice(), vertexBuffer, nullptr);
	vkFreeMemory(device.getVkDevice(), deviceMemory, nullptr);
}

uint32_t VulkanVertexBuffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
	VkPhysicalDeviceMemoryProperties memoryProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice.getVkPhysicalDevice(), &memoryProperties);

	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
		if (typeFilter & (1 << i)) {
			if (memoryProperties.memoryTypes[i].propertyFlags & properties) {
				return i;
			}
		}
	}

	throw std::runtime_error("failed to find suitable memory type");
}
