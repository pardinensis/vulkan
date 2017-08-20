#pragma once

#include "common_includes.hpp"
#include "vulkan_device.hpp"
#include "vulkan_physical_device.hpp"
#include "vulkan_semaphore.hpp"

class VulkanSwapchain {
private:
	const VulkanPhysicalDevice& vulkanPhysicalDevice;
	const VulkanDevice& vulkanDevice;

	VkSwapchainKHR swapchain;
	std::vector<VkImage> swapchainImages;
	VkFormat swapchainImageFormat;
	VkExtent2D swapchainExtent;
	std::vector<VkImageView> swapchainImageViews;


public:
	VulkanSwapchain(const VulkanPhysicalDevice& vulkanPhysicalDevice, const VulkanDevice& vulkanDevice, const VkSurfaceKHR& surface, const VkExtent2D& preferredExtent);
	~VulkanSwapchain();

	uint32_t aquireNextImage(const VulkanSemaphore& imageAquiredSemaphore);
	uint32_t presentImage(uint32_t imageIndex, const VulkanSemaphore& renderFinishedSemaphore);

	const VkSwapchainKHR getVkSwapchain() const { return swapchain; }
	const VkFormat& getVkFormat() const { return swapchainImageFormat; }
	const VkExtent2D& getVkExtent() const { return swapchainExtent; }
	const std::vector<VkImageView>& getVkImageViews() const { return swapchainImageViews; }

private:
	VkSurfaceFormatKHR chooseSurfaceFormat();
	VkPresentModeKHR choosePresentMode();
	VkExtent2D chooseExtent(const VkExtent2D& preferredExtent);
};