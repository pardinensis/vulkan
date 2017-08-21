#include "vulkan_vertex_buffer.hpp"

VulkanVertexBuffer::VulkanVertexBuffer(const VulkanPhysicalDevice& physicalDevice, const VulkanDevice& device, const std::vector<VulkanVertex>& vertexData)
		: physicalDevice(physicalDevice), device(device), vertexData(vertexData) {

	// create staging buffer
	VkDeviceSize bufferSize = sizeof(vertexData[0]) * vertexData.size();
	VulkanBuffer stagingBuffer(physicalDevice, device, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	// copy data
	void* data;
	vkMapMemory(device.getVkDevice(), stagingBuffer.getVkDeviceMemory(), 0, bufferSize, 0, &data);
	std::memcpy(data, vertexData.data(), static_cast<size_t>(bufferSize));
	vkUnmapMemory(device.getVkDevice(), stagingBuffer.getVkDeviceMemory());

	// create vertex buffer
	vertexBuffer = new VulkanBuffer(physicalDevice, device, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	// copy buffer content
	stagingBuffer.copyTo(*vertexBuffer, bufferSize);
}

VulkanVertexBuffer::~VulkanVertexBuffer() {
	delete vertexBuffer;
}
