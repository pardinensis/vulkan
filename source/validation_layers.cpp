#include "validation_layers.hpp"

bool ValidationLayers::isSupported() {
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const std::string& layerName : validationLayers) {
		bool layerFound = false;
		for (const VkLayerProperties& layerProperties : availableLayers) {
			if (layerName == layerProperties.layerName) {
				layerFound = true;
			}
		}
		if (!layerFound) {
			return false;
		}
	}
	return true;
}