#pragma once

#include "common_includes.hpp"

#include "device.hpp"
#include "image_view.hpp"
#include "physical_device.hpp"
#include "semaphore.hpp"

class Swapchain {
private:
	const PhysicalDevice& vulkanPhysicalDevice;
	const Device& vulkanDevice;

	VkSwapchainKHR swapchain;
	std::vector<VkImage> swapchainImages;
	VkFormat swapchainImageFormat;
	VkExtent2D swapchainExtent;

	std::vector<ImageView*> swapchainImageViews;


public:
	Swapchain(const PhysicalDevice& vulkanPhysicalDevice, const Device& vulkanDevice, const VkSurfaceKHR& surface, const VkExtent2D& preferredExtent);
	~Swapchain();

	uint32_t aquireNextImage(const Semaphore& imageAquiredSemaphore);
	uint32_t presentImage(uint32_t imageIndex, const Semaphore& renderFinishedSemaphore);

	const VkSwapchainKHR getVkSwapchain() const { return swapchain; }
	const VkFormat& getVkFormat() const { return swapchainImageFormat; }
	const VkExtent2D& getVkExtent() const { return swapchainExtent; }
	const std::vector<ImageView*>& getImageViews() const { return swapchainImageViews; }

private:
	VkSurfaceFormatKHR chooseSurfaceFormat();
	VkPresentModeKHR choosePresentMode();
	VkExtent2D chooseExtent(const VkExtent2D& preferredExtent);
};