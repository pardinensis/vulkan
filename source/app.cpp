#include "app.hpp"

#include "config.hpp"

void App::init(const std::string& app_name) {
	// init glfw
	glfwInit();

	// create vulkan instance
	instance = new Instance();

	mainWindow = new MainWindow(*instance);

	// choose physical device
	physicalDevice = PhysicalDevice::choosePhysicalDevice(*instance, mainWindow->getVkSurface());

	// create logical device
	device = new Device(*physicalDevice);

	// create swapchain
	VkExtent2D preferredExtent = { mainWindow->getWidth(), mainWindow->getHeight() };
	swapchain = new Swapchain(*physicalDevice, *device, mainWindow->getVkSurface(), preferredExtent);

	// create render pass
	renderPass = new RenderPass(*device, swapchain->getVkFormat());

	// create shader
	shader = new Shader(*device);
	shader->addShaderModule("shader/triangle_vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
	shader->addShaderModule("shader/triangle_frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);

	// create camera
	camera = new Camera();
	mainWindow->registerKeyPressedHandler([this](KeyCode keyCode) -> bool { return this->camera->keyPressed(keyCode); });
	mainWindow->registerKeyReleasedHandler([this](KeyCode keyCode) -> bool { return this->camera->keyReleased(keyCode); });
	mainWindow->registerMousePressedHandler([this](MouseButton button, int x, int y) -> bool { return this->camera->mousePressed(button, x, y); });
	mainWindow->registerMouseReleasedHandler([this](MouseButton button, int x, int y) -> bool { return this->camera->mouseReleased(button); });
	mainWindow->registerMouseMovedHandler([this](int x, int y) -> bool { return this->camera->mouseMoved(x, y); });

	// create projection
	projection = new Projection(swapchain->getVkExtent());

	// create uniform buffer
	uniformBuffer = new UniformBuffer(*device);

	std::vector<glm::vec3> instanceData = { glm::vec3(0, 0, 2), glm::vec3(0, 0, -2) };
	instanceBuffer = new InstanceBuffer(*device, instanceData);

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

	// model = ModelLoader::loadObjFile(*device, "../models/bunny2503.obj", true);
	model = ModelLoader::loadObjFile(*device, "../models/bunny34817.obj", true);

	// create command buffers
	commandBuffers = new CommandBuffers(*device, *renderPass, *pipeline,
		*framebuffers, model->getVertexBuffer(), model->getIndexBuffer(), *instanceBuffer, *descriptorSet);

	// create semaphores
	semaphoreImageAquired = new Semaphore(*device);
	semaphoreRenderFinished = new Semaphore(*device);
}


void App::run() {
	while (mainWindow->isAlive()) {
		glfwPollEvents();

		camera->update();
		uniformBuffer->update(swapchain->getVkExtent(), camera->getViewMatrix(), projection->getProjectionMatrix());

		vkQueueWaitIdle(device->getVkPresentQueue());
		
		uint32_t imageIndex = swapchain->aquireNextImage(*semaphoreImageAquired);
		if (imageIndex == -1) {
			// recreateSwapchain();
			continue;
		}

		commandBuffers->submit(imageIndex, *semaphoreImageAquired, *semaphoreRenderFinished);


		imageIndex = swapchain->presentImage(imageIndex, *semaphoreRenderFinished);
		if (imageIndex == -1) {
			// recreateSwapchain();
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
	delete camera;
	delete projection;
	delete uniformBuffer;
	delete instanceBuffer;
	delete textureSampler;
	delete texture;
	delete model;
	delete device;
	delete physicalDevice;
	delete instance;
}

// void App::recreateSwapchain() {
// 	vkDeviceWaitIdle(device->getVkDevice());

// 	delete commandBuffers;
// 	delete framebuffers;
// 	delete depthResource;
// 	delete pipeline;
// 	delete renderPass;
// 	delete swapchain;

// 	int width, height;
// 	glfwGetWindowSize(window, &width, &height);
// 	VkExtent2D actualExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

// 	swapchain = new Swapchain(*physicalDevice, *device, surface, actualExtent);
// 	renderPass = new RenderPass(*device, swapchain->getVkFormat());
// 	pipeline = new Pipeline(*device, *renderPass, swapchain->getVkExtent(), *shader, *descriptorSet);
// 	depthResource = new DepthResource(*device, swapchain->getVkExtent());
// 	framebuffers = new Framebuffers(*device, *swapchain, *renderPass, depthResource->getImageView());
// 	commandBuffers = new CommandBuffers(*device, *renderPass, *pipeline,
// 		*framebuffers, model->getVertexBuffer(), model->getIndexBuffer(), *descriptorSet);
// }

// void App::onWindowResized(GLFWwindow* window, int width, int height) {
// 	if (width == 0 || height == 0)
// 		return;

// 	App* app = reinterpret_cast<App*>(glfwGetWindowUserPointer(window));
// 	app->recreateSwapchain();
// }