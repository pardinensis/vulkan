#pragma once

#include "common_includes.hpp"

#include "buffer.hpp"
#include "device.hpp"

class IndexBuffer {
private:
	const Device& device;

	std::vector<uint16_t> indexData;

	Buffer* indexBuffer;

public:
	IndexBuffer(const Device& device, const std::vector<uint16_t> indexData);
	~IndexBuffer();

	unsigned int getIndexCount() const { return indexData.size(); }
	const Buffer& getBuffer() const { return *indexBuffer; }
};