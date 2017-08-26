#include "index_buffer.hpp"

IndexBuffer::IndexBuffer(const Device& device, const std::vector<uint32_t> indexData)
		: device(device), indexData(indexData) {

	// create staging buffer
	VkDeviceSize bufferSize = sizeof(indexData[0]) * indexData.size();
	Buffer stagingBuffer(device, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	// copy data
	stagingBuffer.fill(bufferSize, (void*)(indexData.data()));

	// create vertex buffer
	indexBuffer = new Buffer(device, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	// copy buffer content
	stagingBuffer.copyTo(*indexBuffer, bufferSize);
}

IndexBuffer::~IndexBuffer() {
	delete indexBuffer;
}
