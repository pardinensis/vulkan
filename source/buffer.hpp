#pragma once

#include "common_includes.hpp"

#include "device.hpp"
#include "image.hpp"
#include "physical_device.hpp"

class Buffer {
private:
	const Device& device;

	VkBuffer buffer;
	VkDeviceMemory deviceMemory;

public:
	static uint32_t findMemoryType(const Device& device, uint32_t typeFilter, VkMemoryPropertyFlags properties);

public:
	Buffer(const Device& device, VkDeviceSize bufferSize, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
	~Buffer();

	const VkBuffer& getVkBuffer() const { return buffer; }
	const VkDeviceMemory& getVkDeviceMemory() const { return deviceMemory; }

	void copyTo(const Buffer& destinationBuffer, VkDeviceSize size);
	void copyToImage(const Image& destinationImage, uint32_t width, uint32_t height);
	void fill(VkDeviceSize size, void* data);
};