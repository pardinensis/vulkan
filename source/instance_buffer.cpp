#include "instance_buffer.hpp"

InstanceBuffer::InstanceBuffer(const Device& device, const std::vector<glm::vec3>& instanceData)
	: device(device), instanceData(instanceData) {

	// create staging buffer
	VkDeviceSize bufferSize = sizeof(instanceData[0]) * instanceData.size();
	Buffer stagingBuffer(device, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	// copy data
	stagingBuffer.fill(bufferSize, (void*)instanceData.data());

	// create vertex buffer
	instanceBuffer = new Buffer(device, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	// copy buffer content
	stagingBuffer.copyTo(*instanceBuffer, bufferSize);
}

InstanceBuffer::~InstanceBuffer() {
	delete instanceBuffer;
}