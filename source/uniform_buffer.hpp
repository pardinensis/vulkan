#pragma once

#include "common_includes.hpp"

#include "buffer.hpp"
#include "device.hpp"

struct UniformBufferObject {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};

class UniformBuffer {
private:
	const Device& device;

	Buffer* uniformBuffer;

public:
	UniformBuffer(const Device& device);
	~UniformBuffer();

	const Buffer& getBuffer() const { return *uniformBuffer; }

	void update(const VkExtent2D& extent);
};