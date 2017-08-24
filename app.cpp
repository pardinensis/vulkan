#include "app.hpp"

#include "config.hpp"

void App::init(const std::string& app_name) {
	// init glfw
	glfwInit();

	// create window
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    // glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr);

    glfwSetWindowUserPointer(window, this);
    glfwSetWindowSizeCallback(window, App::onWindowResized);

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
    VkExtent2D preferredExtent = { WINDOW_WIDTH, WINDOW_HEIGHT };
    vulkanSwapchain = new VulkanSwapchain(*vulkanPhysicalDevice, *vulkanDevice, surface, preferredExtent);

    // create render pass
    vulkanRenderPass = new VulkanRenderPass(*vulkanDevice, vulkanSwapchain->getVkFormat());

    // create shader
    vulkanShader = new VulkanShader(*vulkanDevice);
    vulkanShader->addShaderModule("shader/triangle_vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
    vulkanShader->addShaderModule("shader/triangle_frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);

    // create uniform buffer
    vulkanUniformBuffer = new VulkanUniformBuffer(*vulkanDevice);

    // create texture
    vulkanTexture = new VulkanTexture(*vulkanDevice, "../texture/og.jpg");
    vulkanTextureSampler = new VulkanTextureSampler(*vulkanDevice);

    // create descriptor set
    vulkanDescriptorSet = new VulkanDescriptorSet(*vulkanDevice, *vulkanUniformBuffer, vulkanTexture->getImageView(), *vulkanTextureSampler);

    // create pipeline
    vulkanPipeline = new VulkanPipeline(*vulkanDevice, *vulkanRenderPass, vulkanSwapchain->getVkExtent(), *vulkanShader, *vulkanDescriptorSet);

    // create depth resource
    vulkanDepthResource = new VulkanDepthResource(*vulkanDevice, vulkanSwapchain->getVkExtent());

    // create framebuffers
    vulkanFramebuffers = new VulkanFramebuffers(*vulkanDevice, *vulkanSwapchain, *vulkanRenderPass, vulkanDepthResource->getImageView());

    // create vertex buffer
    std::vector<VulkanVertex> vertexData = {
        {{-0.5f, -0.5f, 0.2f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
        {{ 0.5f, -0.5f, 0.2f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        {{-0.5f,  0.5f, 0.2f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
        {{ 0.5f,  0.5f, 0.2f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
        {{ 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        {{-0.5f,  0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
        {{ 0.5f,  0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}
    };
    std::vector<uint16_t> indexData = {
        0, 1, 2,
        2, 1, 3,
        4, 5, 6,
        6, 5, 7
    };

    vulkanVertexBuffer = new VulkanVertexBuffer(*vulkanDevice, vertexData);
    vulkanIndexBuffer = new VulkanIndexBuffer(*vulkanDevice, indexData);

    // create command buffers
    vulkanCommandBuffers = new VulkanCommandBuffers(*vulkanDevice, *vulkanRenderPass, *vulkanPipeline,
        *vulkanFramebuffers, *vulkanVertexBuffer, *vulkanIndexBuffer, *vulkanDescriptorSet);

    // create semaphores
    vulkanSemaphoreImageAquired = new VulkanSemaphore(*vulkanDevice);
    vulkanSemaphoreRenderFinished = new VulkanSemaphore(*vulkanDevice);
}

void App::run() {
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

        vulkanUniformBuffer->update(vulkanSwapchain->getVkExtent());

        vkQueueWaitIdle(vulkanDevice->getVkPresentQueue());
        
        uint32_t imageIndex = vulkanSwapchain->aquireNextImage(*vulkanSemaphoreImageAquired);
        if (imageIndex == -1) {
            recreateSwapchain();
            continue;
        }

        vulkanCommandBuffers->submit(imageIndex, *vulkanSemaphoreImageAquired, *vulkanSemaphoreRenderFinished);

        imageIndex = vulkanSwapchain->presentImage(imageIndex, *vulkanSemaphoreRenderFinished);
        if (imageIndex == -1) {
            recreateSwapchain();
            continue;
        }
    }
}

void App::cleanup() {
    vkDeviceWaitIdle(vulkanDevice->getVkDevice());

    delete vulkanSemaphoreRenderFinished;
    delete vulkanSemaphoreImageAquired;
    delete vulkanCommandBuffers;
    delete vulkanFramebuffers;
    delete vulkanDepthResource;
    delete vulkanPipeline;
    delete vulkanShader;
    delete vulkanRenderPass;
    delete vulkanSwapchain;
    
    delete vulkanDescriptorSet;
    delete vulkanUniformBuffer;
    delete vulkanTextureSampler;
    delete vulkanTexture;
    delete vulkanIndexBuffer;
    delete vulkanVertexBuffer;
    delete vulkanDevice;
    delete vulkanPhysicalDevice;
    delete vulkanInstance;
}

void App::recreateSwapchain() {
    vkDeviceWaitIdle(vulkanDevice->getVkDevice());

    delete vulkanCommandBuffers;
    delete vulkanFramebuffers;
    delete vulkanDepthResource;
    delete vulkanPipeline;
    delete vulkanRenderPass;
    delete vulkanSwapchain;

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    VkExtent2D actualExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

    vulkanSwapchain = new VulkanSwapchain(*vulkanPhysicalDevice, *vulkanDevice, surface, actualExtent);
    vulkanRenderPass = new VulkanRenderPass(*vulkanDevice, vulkanSwapchain->getVkFormat());
    vulkanPipeline = new VulkanPipeline(*vulkanDevice, *vulkanRenderPass, vulkanSwapchain->getVkExtent(), *vulkanShader, *vulkanDescriptorSet);
    vulkanDepthResource = new VulkanDepthResource(*vulkanDevice, vulkanSwapchain->getVkExtent());
    vulkanFramebuffers = new VulkanFramebuffers(*vulkanDevice, *vulkanSwapchain, *vulkanRenderPass, vulkanDepthResource->getImageView());
    vulkanCommandBuffers = new VulkanCommandBuffers(*vulkanDevice, *vulkanRenderPass, *vulkanPipeline,
        *vulkanFramebuffers, *vulkanVertexBuffer, *vulkanIndexBuffer, *vulkanDescriptorSet);
}

void App::onWindowResized(GLFWwindow* window, int width, int height) {
    if (width == 0 || height == 0)
        return;

    App* app = reinterpret_cast<App*>(glfwGetWindowUserPointer(window));
    app->recreateSwapchain();
}