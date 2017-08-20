#include "app.hpp"

#include "config.hpp"

void App::init(const std::string& app_name) {
	// init glfw
	glfwInit();

	// create window
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr);

    // create vulkan instance
    vulkanInstance = VulkanInstance::create();

    // create surface
    if(glfwCreateWindowSurface(vulkanInstance->getVkInstance(), window, nullptr, &surface) != VK_SUCCESS) {
    	throw std::runtime_error("failed to create surface");
    }

    // choose physical device
    vulkanPhysicalDevice = VulkanPhysicalDevice::choosePhysicalDevice(*vulkanInstance, surface);

    // create logical device
    vulkanDevice = VulkanDevice::create(*vulkanPhysicalDevice);

    // create swapchain
    vulkanSwapchain = VulkanSwapchain::create(*vulkanPhysicalDevice, *vulkanDevice, surface);

    // create pipeline
    vulkanPipeline = VulkanPipeline::create(*vulkanPhysicalDevice, *vulkanDevice, *vulkanSwapchain);
}

void App::run() {
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
        vulkanPipeline->drawFrame();
	}
}