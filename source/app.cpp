#include "app.hpp"

#include "config.hpp"

void App::init(const std::string& app_name) {
	// init glfw
	glfwInit();

	// create window
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr);

	glfwSetWindowUserPointer(window, this);
	glfwSetWindowSizeCallback(window, App::onWindowResized);
	glfwSetKeyCallback(window, App::onKeyAction);
	glfwSetMouseButtonCallback(window, App::onMouseButtonAction);
	glfwSetCursorPosCallback(window, App::onCursorMoved);

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

	// create camera
	camera = new Camera();

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

	// vertexBuffer = new VertexBuffer(*device, vertexData);
	// indexBuffer = new IndexBuffer(*device, indexData);
	model = ModelLoader::loadObjFile(*device, "../models/bunny2503.obj");

	// create command buffers
	commandBuffers = new CommandBuffers(*device, *renderPass, *pipeline,
		*framebuffers, model->getVertexBuffer(), model->getIndexBuffer(), *descriptorSet);

	// create semaphores
	semaphoreImageAquired = new Semaphore(*device);
	semaphoreRenderFinished = new Semaphore(*device);
}

void App::run() {
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		camera->update();
		uniformBuffer->update(swapchain->getVkExtent(), camera->getViewMatrix());

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
	// delete indexBuffer;
	// delete vertexBuffer;
	delete model;
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
		*framebuffers, model->getVertexBuffer(), model->getIndexBuffer(), *descriptorSet);
}

void App::onWindowResized(GLFWwindow* window, int width, int height) {
	if (width == 0 || height == 0)
		return;

	App* app = reinterpret_cast<App*>(glfwGetWindowUserPointer(window));
	app->recreateSwapchain();
}

void App::onKeyAction(GLFWwindow* window, int key, int scancode, int action, int mods) {
	App* app = reinterpret_cast<App*>(glfwGetWindowUserPointer(window));

	switch (action) {
		case GLFW_PRESS:
			app->camera->keyPressed(key);
			break;
		case GLFW_RELEASE:
			app->camera->keyReleased(key);
			break;
	}
}

void App::onMouseButtonAction(GLFWwindow* window, int button, int action, int mods) {
	App* app = reinterpret_cast<App*>(glfwGetWindowUserPointer(window));

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	switch (action) {
		case GLFW_PRESS:
			app->camera->mouseButtonPressed(button, static_cast<int>(xpos), static_cast<int>(ypos));
			break;
		case GLFW_RELEASE:
			app->camera->mouseButtonReleased(button);
			break;
	}
}

void App::onCursorMoved(GLFWwindow* window, double xpos, double ypos) {
	App* app = reinterpret_cast<App*>(glfwGetWindowUserPointer(window));

	app->camera->mouseMoved(static_cast<int>(xpos), static_cast<int>(ypos));
}