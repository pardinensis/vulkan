#pragma once

#include "common_includes.hpp"

#include "buffer.hpp"
#include "device.hpp"
#include "vertex.hpp"

class VertexBuffer {
private:
	const Device& device;

	std::vector<Vertex> vertexData;

	Buffer* vertexBuffer;

public:
	VertexBuffer(const Device& device, const std::vector<Vertex>& vertexData);
	~VertexBuffer();

	unsigned int getVertexCount() const { return vertexData.size(); }
	const Buffer& getBuffer() const { return *vertexBuffer; }
};