#include "vulkan_vertex_buffer.hpp"

VulkanVertexBuffer::VulkanVertexBuffer(const VulkanDevice& device, const std::vector<VulkanVertex>& vertexData)
		: device(device), vertexData(vertexData) {

	// create staging buffer
	VkDeviceSize bufferSize = sizeof(vertexData[0]) * vertexData.size();
	VulkanBuffer stagingBuffer(device, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	// copy data
	stagingBuffer.fill(bufferSize, (void*)vertexData.data());

	// create vertex buffer
	vertexBuffer = new VulkanBuffer(device, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	// copy buffer content
	stagingBuffer.copyTo(*vertexBuffer, bufferSize);
}

VulkanVertexBuffer::~VulkanVertexBuffer() {
	delete vertexBuffer;
}
