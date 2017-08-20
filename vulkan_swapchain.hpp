#pragma once

#include "common_includes.hpp"
#include "vulkan_device.hpp"
#include "vulkan_physical_device.hpp"

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
	VulkanSwapchain(const VulkanPhysicalDevice& vulkanPhysicalDevice, const VulkanDevice& vulkanDevice, const VkSurfaceKHR& surface);
	~VulkanSwapchain();

	const VkSwapchainKHR getVkSwapchain() const { return swapchain; }
	const VkFormat& getVkFormat() const { return swapchainImageFormat; }
	const VkExtent2D& getVkExtent() const { return swapchainExtent; }
	const std::vector<VkImageView>& getVkImageViews() const { return swapchainImageViews; }

private:
	VkSurfaceFormatKHR chooseSurfaceFormat();
	VkPresentModeKHR choosePresentMode();
	VkExtent2D chooseExtent();

public:
	static std::shared_ptr<VulkanSwapchain> create(const VulkanPhysicalDevice& vulkanPhysicalDevice, const VulkanDevice& vulkanDevice, const VkSurfaceKHR& surface);
};