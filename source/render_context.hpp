#pragma once


#include "common_includes.hpp"
#include "device.hpp"
#include "physical_device.hpp"


class RenderContext {
private:
	VkInstance instance;
	VkDebugReportCallbackEXT debugReportCallback;

	Device* device;

public:
	RenderContext();
	~RenderContext();

	// prevent copying and moving
	RenderContext(const RenderContext&) = delete;
	RenderContext(RenderContext&&) = delete;
	void operator=(const RenderContext&) = delete;
	void operator=(RenderContext&&) = delete;

private:
	void createInstance();
	void createLogicalDevice(const PhysicalDevice& physicalDevice);

public:
	void chooseDevice(const VkSurfaceKHR& surface);

public:
	static RenderContext& getRenderContext();

	static VkInstance getVkInstance();
	static VkDevice getVkDevice();
};