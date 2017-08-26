#include "uniform_buffer.hpp"

UniformBuffer::UniformBuffer(const Device& device)
		: device(device) {

	VkDeviceSize bufferSize = sizeof(UniformBufferObject);
	uniformBuffer = new Buffer(device, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

UniformBuffer::~UniformBuffer() {
	delete uniformBuffer;
}

void UniformBuffer::update(const VkExtent2D& extent, const glm::mat4& viewMatrix) {
	// static auto startTime = std::chrono::high_resolution_clock::now();
	// auto currentTime = std::chrono::high_resolution_clock::now();
	// float time = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count() / 1000.0f;
	float time = 0;

	UniformBufferObject uniformBufferObject = {};
	uniformBufferObject.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	uniformBufferObject.view = viewMatrix;
	uniformBufferObject.proj = glm::perspective(glm::radians(60.0f), extent.width / (float) extent.height, 0.01f, 10.0f);
	uniformBufferObject.proj[1][1] *= -1; // compensate for the inverse y coordinate axis (vulkan: y up, opengl: y down)

	void* data;
	vkMapMemory(device.getVkDevice(), uniformBuffer->getVkDeviceMemory(), 0, sizeof(uniformBufferObject), 0, &data);
    memcpy(data, &uniformBufferObject, sizeof(uniformBufferObject));
	vkUnmapMemory(device.getVkDevice(), uniformBuffer->getVkDeviceMemory());

	uniformBuffer->fill(sizeof(uniformBufferObject), &uniformBufferObject);
}