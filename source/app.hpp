#pragma once

#include "common_includes.hpp"

#include "camera.hpp"
#include "command_buffers.hpp"
#include "depth_resource.hpp"
#include "descriptor_set.hpp"
#include "device.hpp"
#include "framebuffers.hpp"
#include "index_buffer.hpp"
#include "instance.hpp"
#include "main_window.hpp"
#include "model.hpp"
#include "model_loader.hpp"
#include "physical_device.hpp"
#include "pipeline.hpp"
#include "render_pass.hpp"
#include "semaphore.hpp"
#include "shader.hpp"
#include "swapchain.hpp"
#include "texture.hpp"
#include "texture_sampler.hpp"
#include "uniform_buffer.hpp"
#include "vertex.hpp"
#include "vertex_buffer.hpp"

class App {
public:
	MainWindow* mainWindow;

	Instance* instance;
	PhysicalDevice* physicalDevice;
	Device* device;
	Swapchain* swapchain;
	RenderPass* renderPass;
	Shader* shader;
	Pipeline* pipeline;
	DepthResource* depthResource;
	Framebuffers* framebuffers;
	Model* model;

	Texture* texture;
	TextureSampler* textureSampler;
	Camera* camera;
	UniformBuffer* uniformBuffer;
	DescriptorSet* descriptorSet;
	CommandBuffers* commandBuffers;
	Semaphore* semaphoreImageAquired;
	Semaphore* semaphoreRenderFinished;

private:
	// void recreateSwapchain();
	static void onWindowResized(GLFWwindow* window, int width, int height);
	static void onKeyAction(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void onMouseButtonAction(GLFWwindow* window, int button, int action, int mods);
	static void onCursorMoved(GLFWwindow* window, double xpos, double ypos);

public:
	void init(const std::string& app_name);
	void run();
	void cleanup();
};