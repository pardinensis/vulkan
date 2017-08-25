#pragma once

#include "common_includes.hpp"

class Instance {
private:
	VkInstance instance;
	VkDebugReportCallbackEXT debugReportCallback;


public:
	Instance();
	~Instance();

	const VkInstance& getVkInstance() const { return instance; }
};