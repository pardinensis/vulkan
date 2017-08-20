#pragma once

#include "common_includes.hpp"
#include "vulkan_instance.hpp"

class VulkanPhysicalDevice {
private:
	VkPhysicalDevice vkPhysicalDevice;

	int graphicsFamily;
	int presentFamily;

	std::vector<VkExtensionProperties> extensionProperties;

	VkSurfaceCapabilitiesKHR surfaceCapabilities;
	std::vector<VkSurfaceFormatKHR> surfaceFormats;
	std::vector<VkPresentModeKHR> presentModes;

public:
	const std::vector<const char*> DEVICE_EXTENSIONS = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

public:
	VulkanPhysicalDevice(const VkPhysicalDevice& vkPhysicalDevice, const VkSurfaceKHR& vkSurface);

	const VkPhysicalDevice& getVkPhysicalDevice() const { return vkPhysicalDevice; }
	int getGraphicsFamily() const { return graphicsFamily; }
	int getPresentFamily() const { return presentFamily; }
	const VkSurfaceCapabilitiesKHR& getSurfaceCapabilities() const { return surfaceCapabilities; }
	const std::vector<VkSurfaceFormatKHR>& getSurfaceFormats() const { return surfaceFormats; }
	const std::vector<VkPresentModeKHR>& getPresentModes() const { return presentModes; }

	bool isSuitable();

public:
	static VulkanPhysicalDevice* choosePhysicalDevice(const VulkanInstance& vulkanInstance, const VkSurfaceKHR& vkSurface);
};