#pragma once

#include "common_includes.hpp"

#include "buffer.hpp"
#include "device.hpp"
#include "vertex.hpp"

class InstanceBuffer {
private:
	const Device& device;

	std::vector<glm::vec3> instanceData;

	Buffer* instanceBuffer;

public:
	InstanceBuffer(const Device& device, const std::vector<glm::vec3>& instanceData);
	~InstanceBuffer();

	unsigned int getInstanceCount() const { return instanceData.size(); }
	const Buffer& getBuffer() const { return *instanceBuffer; }
};