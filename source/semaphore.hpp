#pragma once

#include "common_includes.hpp"

#include "device.hpp"

class Semaphore {
private:
	const Device& device;

	VkSemaphore semaphore;

public:
	Semaphore(const Device& device);
	~Semaphore();

	const VkSemaphore& getVkSemaphore() const { return semaphore; }
};