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
    vulkanInstance = new VulkanInstance();

    // create surface
    if(glfwCreateWindowSurface(vulkanInstance->getVkInstance(), window, nullptr, &surface) != VK_SUCCESS) {
    	throw std::runtime_error("failed to create surface");
    }

    // choose physical device
    vulkanPhysicalDevice = VulkanPhysicalDevice::choosePhysicalDevice(*vulkanInstance, surface);

    // create logical device
    vulkanDevice = new VulkanDevice(*vulkanPhysicalDevice);

    // create swapchain
    vulkanSwapchain = new VulkanSwapchain(*vulkanPhysicalDevice, *vulkanDevice, surface);

    // create render pass
    vulkanRenderPass = new VulkanRenderPass(*vulkanDevice, vulkanSwapchain->getVkFormat());

    // create shader
    vulkanShader = new VulkanShader(*vulkanDevice);
    vulkanShader->addShaderModule("shader/vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
    vulkanShader->addShaderModule("shader/frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);

    // create pipeline
    vulkanPipeline = new VulkanPipeline(*vulkanDevice, *vulkanRenderPass, vulkanSwapchain->getVkExtent(), *vulkanShader);

    // create framebuffers
    vulkanFramebuffers = new VulkanFramebuffers(*vulkanDevice, *vulkanSwapchain, *vulkanRenderPass);

    // create command buffers
    vulkanCommandBuffers = new VulkanCommandBuffers(*vulkanDevice, *vulkanRenderPass, *vulkanPipeline,
        *vulkanFramebuffers, vulkanPhysicalDevice->getGraphicsFamily());

    // create semaphores
    vulkanSemaphoreImageAquired = new VulkanSemaphore(*vulkanDevice);
    vulkanSemaphoreRenderFinished = new VulkanSemaphore(*vulkanDevice);
}

void App::run() {
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

        vkQueueWaitIdle(vulkanDevice->getVkPresentQueue());
        uint32_t imageIndex = vulkanSwapchain->aquireNextImage(*vulkanSemaphoreImageAquired);
        vulkanCommandBuffers->submit(imageIndex, *vulkanSemaphoreImageAquired, *vulkanSemaphoreRenderFinished);
        vulkanSwapchain->presentImage(imageIndex, *vulkanSemaphoreRenderFinished);
    }

    vkDeviceWaitIdle(vulkanDevice->getVkDevice());
}

void App::cleanup() {
    delete vulkanSemaphoreRenderFinished;
    delete vulkanSemaphoreImageAquired;
    delete vulkanCommandBuffers;
    delete vulkanFramebuffers;
    delete vulkanPipeline;
    delete vulkanShader;
    delete vulkanRenderPass;
    delete vulkanSwapchain;
    delete vulkanDevice;
    delete vulkanPhysicalDevice;
    delete vulkanInstance;
}