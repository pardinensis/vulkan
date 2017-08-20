#pragma once

#include "common_includes.hpp"


namespace VulkanValidationLayers {
	const std::vector<const char*> validationLayers = {
		"VK_LAYER_LUNARG_standard_validation"
	};
	
	bool isSupported();
};
