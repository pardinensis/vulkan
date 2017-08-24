#include "vulkan_index_buffer.hpp"

VulkanIndexBuffer::VulkanIndexBuffer(const VulkanDevice& device, const std::vector<uint16_t> indexData)
		: device(device), indexData(indexData) {

	// create staging buffer
	VkDeviceSize bufferSize = sizeof(indexData[0]) * indexData.size();
	VulkanBuffer stagingBuffer(device, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	// copy data
	stagingBuffer.fill(bufferSize, (void*)(indexData.data()));

	// create vertex buffer
	indexBuffer = new VulkanBuffer(device, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	// copy buffer content
	stagingBuffer.copyTo(*indexBuffer, bufferSize);
}

VulkanIndexBuffer::~VulkanIndexBuffer() {
	delete indexBuffer;
}
