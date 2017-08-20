#pragma once

#include "common_includes.hpp"
#include "vulkan_command_buffers.hpp"
#include "vulkan_device.hpp"
#include "vulkan_framebuffers.hpp"
#include "vulkan_instance.hpp"
#include "vulkan_physical_device.hpp"
#include "vulkan_pipeline.hpp"
#include "vulkan_render_pass.hpp"
#include "vulkan_semaphore.hpp"
#include "vulkan_shader.hpp"
#include "vulkan_swapchain.hpp"

class App {
private:
	GLFWwindow* window;
	VkSurfaceKHR surface;

	VulkanInstance* vulkanInstance;
    VulkanPhysicalDevice* vulkanPhysicalDevice;
    VulkanDevice* vulkanDevice;
    VulkanSwapchain* vulkanSwapchain;
    VulkanRenderPass* vulkanRenderPass;
    VulkanShader* vulkanShader;
    VulkanPipeline* vulkanPipeline;
    VulkanFramebuffers* vulkanFramebuffers;
    VulkanCommandBuffers* vulkanCommandBuffers;
    VulkanSemaphore* vulkanSemaphoreImageAquired;
    VulkanSemaphore* vulkanSemaphoreRenderFinished;

public:
	void init(const std::string& app_name);
	void run();
    void cleanup();
};