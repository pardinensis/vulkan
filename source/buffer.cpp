#include "buffer.hpp"

#include "utilities.hpp"

Buffer::Buffer(const Device& device, VkDeviceSize bufferSize, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
		: device(device) {

	VkBufferCreateInfo bufferCreateInfo = {};
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.size = bufferSize;
	bufferCreateInfo.usage = usage;
	bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	if (vkCreateBuffer(device.getVkDevice(), &bufferCreateInfo, nullptr, &buffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to create buffer");
	}

	VkMemoryRequirements memoryRequirements;
	vkGetBufferMemoryRequirements(device.getVkDevice(), buffer, &memoryRequirements);

	VkMemoryAllocateInfo memoryAllocateInfo = {};
	memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryAllocateInfo.allocationSize = memoryRequirements.size;
	memoryAllocateInfo.memoryTypeIndex = findMemoryType(device, memoryRequirements.memoryTypeBits, properties);
	if (vkAllocateMemory(device.getVkDevice(), &memoryAllocateInfo, nullptr, &deviceMemory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate vertex buffer memory");
	}

	vkBindBufferMemory(device.getVkDevice(), buffer, deviceMemory, 0);
}

Buffer::~Buffer() {
	vkDestroyBuffer(device.getVkDevice(), buffer, nullptr);
	vkFreeMemory(device.getVkDevice(), deviceMemory, nullptr);
}

uint32_t Buffer::findMemoryType(const Device& device, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
	VkPhysicalDeviceMemoryProperties memoryProperties;
	vkGetPhysicalDeviceMemoryProperties(device.getPhysicalDevice().getVkPhysicalDevice(), &memoryProperties);

	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
		if (typeFilter & (1 << i)) {
			if (memoryProperties.memoryTypes[i].propertyFlags & properties) {
				return i;
			}
		}
	}

	throw std::runtime_error("failed to find suitable memory type");
}

void Buffer::copyTo(const Buffer& destinationBuffer, VkDeviceSize size) {
	VkCommandBuffer commandBuffer = beginSingleTimeCommands(device);

	VkBufferCopy copyRegion = {};
	copyRegion.srcOffset = 0;
	copyRegion.dstOffset = 0;
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, buffer, destinationBuffer.getVkBuffer(), 1, &copyRegion);

	endSingleTimeCommands(device, commandBuffer);
}

void Buffer::copyToImage(const Image& destinationImage, uint32_t width, uint32_t height, VkImageAspectFlags aspectMask) {
	VkCommandBuffer commandBuffer = beginSingleTimeCommands(device);

	VkBufferImageCopy copyRegion = {};
	copyRegion.bufferOffset = 0;
	copyRegion.bufferRowLength = 0;
	copyRegion.bufferImageHeight = 0;
	copyRegion.imageSubresource.aspectMask = aspectMask;
	copyRegion.imageSubresource.mipLevel = 0;
	copyRegion.imageSubresource.baseArrayLayer = 0;
	copyRegion.imageSubresource.layerCount = 1;
	copyRegion.imageOffset = { 0, 0, 0 };
	copyRegion.imageExtent = { width, height, 1 };
	vkCmdCopyBufferToImage(commandBuffer, buffer, destinationImage.getVkImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);

	endSingleTimeCommands(device, commandBuffer);
}

void Buffer::copyFromImage(const Image& sourceImage, uint32_t width, uint32_t height, VkImageAspectFlags aspectMask) {
	VkCommandBuffer commandBuffer = beginSingleTimeCommands(device);

	VkBufferImageCopy copyRegion = {};
	copyRegion.bufferOffset = 0;
	copyRegion.bufferRowLength = 0;
	copyRegion.bufferImageHeight = 0;
	copyRegion.imageSubresource.aspectMask = aspectMask;
	copyRegion.imageSubresource.mipLevel = 0;
	copyRegion.imageSubresource.baseArrayLayer = 0;
	copyRegion.imageSubresource.layerCount = 1;
	copyRegion.imageOffset = { 0, 0, 0 };
	copyRegion.imageExtent = { width, height, 1 };
	vkCmdCopyImageToBuffer(commandBuffer, sourceImage.getVkImage(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, buffer, 1, &copyRegion);

	endSingleTimeCommands(device, commandBuffer);
}

void Buffer::fill(VkDeviceSize size, void* data) {
	void* bufferData;
	vkMapMemory(device.getVkDevice(), deviceMemory, 0, size, 0, &bufferData);
	std::memcpy(bufferData, data, static_cast<size_t>(size));
	vkUnmapMemory(device.getVkDevice(), deviceMemory);
}

void Buffer::retrieve(VkDeviceSize size, void* data) {
	void* bufferData;
	vkMapMemory(device.getVkDevice(), deviceMemory, 0, size, 0, &bufferData);
	std::memcpy(data, bufferData, static_cast<size_t>(size));
	vkUnmapMemory(device.getVkDevice(), deviceMemory);
}