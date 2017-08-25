#include "vertex_buffer.hpp"

VertexBuffer::VertexBuffer(const Device& device, const std::vector<Vertex>& vertexData)
		: device(device), vertexData(vertexData) {

	// create staging buffer
	VkDeviceSize bufferSize = sizeof(vertexData[0]) * vertexData.size();
	Buffer stagingBuffer(device, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	// copy data
	stagingBuffer.fill(bufferSize, (void*)vertexData.data());

	// create vertex buffer
	vertexBuffer = new Buffer(device, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	// copy buffer content
	stagingBuffer.copyTo(*vertexBuffer, bufferSize);
}

VertexBuffer::~VertexBuffer() {
	delete vertexBuffer;
}
