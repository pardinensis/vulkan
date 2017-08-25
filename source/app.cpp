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
    instance = new Instance();

    // create surface
    if(glfwCreateWindowSurface(instance->getVkInstance(), window, nullptr, &surface) != VK_SUCCESS) {
    	throw std::runtime_error("failed to create surface");
    }

    // choose physical device
    physicalDevice = PhysicalDevice::choosePhysicalDevice(*instance, surface);

    // create logical device
    device = new Device(*physicalDevice);

    // create swapchain
    VkExtent2D preferredExtent = { WINDOW_WIDTH, WINDOW_HEIGHT };
    swapchain = new Swapchain(*physicalDevice, *device, surface, preferredExtent);

    // create render pass
    renderPass = new RenderPass(*device, swapchain->getVkFormat());

    // create shader
    shader = new Shader(*device);
    shader->addShaderModule("shader/triangle_vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
    shader->addShaderModule("shader/triangle_frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);

    // create uniform buffer
    uniformBuffer = new UniformBuffer(*device);

    // create texture
    texture = new Texture(*device, "../texture/og.jpg");
    textureSampler = new TextureSampler(*device);

    // create descriptor set
    descriptorSet = new DescriptorSet(*device, *uniformBuffer, texture->getImageView(), *textureSampler);

    // create pipeline
    pipeline = new Pipeline(*device, *renderPass, swapchain->getVkExtent(), *shader, *descriptorSet);

    // create depth resource
    depthResource = new DepthResource(*device, swapchain->getVkExtent());

    // create framebuffers
    framebuffers = new Framebuffers(*device, *swapchain, *renderPass, depthResource->getImageView());

    // create vertex buffer
    std::vector<Vertex> vertexData = {
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

    vertexBuffer = new VertexBuffer(*device, vertexData);
    indexBuffer = new IndexBuffer(*device, indexData);

    // create command buffers
    commandBuffers = new CommandBuffers(*device, *renderPass, *pipeline,
        *framebuffers, *vertexBuffer, *indexBuffer, *descriptorSet);

    // create semaphores
    semaphoreImageAquired = new Semaphore(*device);
    semaphoreRenderFinished = new Semaphore(*device);
}

void App::run() {
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

        uniformBuffer->update(swapchain->getVkExtent());

        vkQueueWaitIdle(device->getVkPresentQueue());
        
        uint32_t imageIndex = swapchain->aquireNextImage(*semaphoreImageAquired);
        if (imageIndex == -1) {
            recreateSwapchain();
            continue;
        }

        commandBuffers->submit(imageIndex, *semaphoreImageAquired, *semaphoreRenderFinished);

        imageIndex = swapchain->presentImage(imageIndex, *semaphoreRenderFinished);
        if (imageIndex == -1) {
            recreateSwapchain();
            continue;
        }
    }
}

void App::cleanup() {
    vkDeviceWaitIdle(device->getVkDevice());

    delete semaphoreRenderFinished;
    delete semaphoreImageAquired;
    delete commandBuffers;
    delete framebuffers;
    delete depthResource;
    delete pipeline;
    delete shader;
    delete renderPass;
    delete swapchain;
    
    delete descriptorSet;
    delete uniformBuffer;
    delete textureSampler;
    delete texture;
    delete indexBuffer;
    delete vertexBuffer;
    delete device;
    delete physicalDevice;
    delete instance;
}

void App::recreateSwapchain() {
    vkDeviceWaitIdle(device->getVkDevice());

    delete commandBuffers;
    delete framebuffers;
    delete depthResource;
    delete pipeline;
    delete renderPass;
    delete swapchain;

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    VkExtent2D actualExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

    swapchain = new Swapchain(*physicalDevice, *device, surface, actualExtent);
    renderPass = new RenderPass(*device, swapchain->getVkFormat());
    pipeline = new Pipeline(*device, *renderPass, swapchain->getVkExtent(), *shader, *descriptorSet);
    depthResource = new DepthResource(*device, swapchain->getVkExtent());
    framebuffers = new Framebuffers(*device, *swapchain, *renderPass, depthResource->getImageView());
    commandBuffers = new CommandBuffers(*device, *renderPass, *pipeline,
        *framebuffers, *vertexBuffer, *indexBuffer, *descriptorSet);
}

void App::onWindowResized(GLFWwindow* window, int width, int height) {
    if (width == 0 || height == 0)
        return;

    App* app = reinterpret_cast<App*>(glfwGetWindowUserPointer(window));
    app->recreateSwapchain();
}