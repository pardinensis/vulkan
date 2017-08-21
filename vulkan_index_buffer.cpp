#include "vulkan_index_buffer.hpp"

VulkanIndexBuffer::VulkanIndexBuffer(const VulkanPhysicalDevice& physicalDevice, const VulkanDevice& device, const std::vector<uint16_t> indexData)
		: physicalDevice(physicalDevice), device(device), indexData(indexData) {

	// create staging buffer
	VkDeviceSize bufferSize = sizeof(indexData[0]) * indexData.size();
	VulkanBuffer stagingBuffer(physicalDevice, device, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	// copy data
	void* data;
	vkMapMemory(device.getVkDevice(), stagingBuffer.getVkDeviceMemory(), 0, bufferSize, 0, &data);
	std::memcpy(data, indexData.data(), static_cast<size_t>(bufferSize));
	vkUnmapMemory(device.getVkDevice(), stagingBuffer.getVkDeviceMemory());

	// create vertex buffer
	indexBuffer = new VulkanBuffer(physicalDevice, device, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	// copy buffer content
	stagingBuffer.copyTo(*indexBuffer, bufferSize);
}

VulkanIndexBuffer::~VulkanIndexBuffer() {
	delete indexBuffer;
}
