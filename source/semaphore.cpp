#include "semaphore.hpp"

Semaphore::Semaphore(const Device& device) 
		: device(device) {
	VkSemaphoreCreateInfo semaphoreCreateInfo = {};
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	if (vkCreateSemaphore(device.getVkDevice(), &semaphoreCreateInfo, nullptr, &semaphore) != VK_SUCCESS) {
		throw std::runtime_error("failed to create semaphore");
	}
}

Semaphore::~Semaphore() {
	vkDestroySemaphore(device.getVkDevice(), semaphore, nullptr);
}