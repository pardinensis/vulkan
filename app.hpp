#pragma once

#include "common_includes.hpp"
#include "vulkan_device.hpp"
#include "vulkan_instance.hpp"
#include "vulkan_physical_device.hpp"
#include "vulkan_pipeline.hpp"
#include "vulkan_swapchain.hpp"

class App {
private:
	GLFWwindow* window;
	VkSurfaceKHR surface;

	std::shared_ptr<VulkanInstance> vulkanInstance;
    std::shared_ptr<VulkanPhysicalDevice> vulkanPhysicalDevice;
    std::shared_ptr<VulkanDevice> vulkanDevice;
    std::shared_ptr<VulkanSwapchain> vulkanSwapchain;
    std::shared_ptr<VulkanPipeline> vulkanPipeline;

public:
	void init(const std::string& app_name);
	void run();
};