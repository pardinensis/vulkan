#pragma once

#include "common_includes.hpp"

class VulkanInstance {
private:
	VkInstance instance;
	VkDebugReportCallbackEXT debugReportCallback;


public:
	VulkanInstance();
	~VulkanInstance();

	const VkInstance& getVkInstance() const { return instance; }
};