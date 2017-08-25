#pragma once

#include "common_includes.hpp"

#include "command_buffers.hpp"
#include "depth_resource.hpp"
#include "descriptor_set.hpp"
#include "device.hpp"
#include "framebuffers.hpp"
#include "index_buffer.hpp"
#include "instance.hpp"
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
private:
	GLFWwindow* window;
	VkSurfaceKHR surface;

	Instance* instance;
    PhysicalDevice* physicalDevice;
    Device* device;
    Swapchain* swapchain;
    RenderPass* renderPass;
    Shader* shader;
    Pipeline* pipeline;
    DepthResource* depthResource;
    Framebuffers* framebuffers;
    VertexBuffer* vertexBuffer;
    IndexBuffer* indexBuffer;
    Texture* texture;
    TextureSampler* textureSampler;
    UniformBuffer* uniformBuffer;
    DescriptorSet* descriptorSet;
    CommandBuffers* commandBuffers;
    Semaphore* semaphoreImageAquired;
    Semaphore* semaphoreRenderFinished;

private:
    void recreateSwapchain();
    static void onWindowResized(GLFWwindow* window, int width, int height);

public:
	void init(const std::string& app_name);
	void run();
    void cleanup();
};