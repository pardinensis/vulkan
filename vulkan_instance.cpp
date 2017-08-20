#include "vulkan_instance.hpp"

#include "config.hpp"
#include "vulkan_validation_layers.hpp"

VulkanInstance::VulkanInstance() {
	if (VALIDATION_LAYERS_ENABLED && !VulkanValidationLayers::isSupported()) {
		throw std::runtime_error("validation layers requested but not supported");
	}

	// basic application info (see config.hpp)
	VkApplicationInfo applicationInfo = {};
	applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInfo.pApplicationName = APPLICATION_NAME;
	applicationInfo.applicationVersion = VK_MAKE_VERSION(APPLICATION_VERSION_MAJOR,
		APPLICATION_VERSION_MINOR, APPLICATION_VERSION_PATCH);
	applicationInfo.pEngineName = ENGINE_NAME;
	applicationInfo.engineVersion = VK_MAKE_VERSION(ENGINE_VERSION_MAJOR, ENGINE_VERSION_MINOR, ENGINE_VERSION_PATCH);
	applicationInfo.apiVersion = VK_API_VERSION_1_0;

	// get required extensions
	std::vector<const char*> requiredExtensions;
	unsigned int requiredExtensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&requiredExtensionCount);
	for (unsigned int i = 0; i < requiredExtensionCount; ++i)
		requiredExtensions.push_back(glfwExtensions[i]);
	if (VALIDATION_LAYERS_ENABLED)
		requiredExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);

	// get enabled layers
	std::vector<const char*> enabledLayers;
	if (VALIDATION_LAYERS_ENABLED)
		enabledLayers = VulkanValidationLayers::validationLayers;

	// create instance
	VkInstanceCreateInfo instanceCreateInfo = {};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pApplicationInfo = &applicationInfo;
	instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
	instanceCreateInfo.ppEnabledExtensionNames = requiredExtensions.data();
	instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(enabledLayers.size());
	instanceCreateInfo.ppEnabledLayerNames = enabledLayers.data();
	if (vkCreateInstance(&instanceCreateInfo, nullptr, &instance) != VK_SUCCESS) {
		throw std::runtime_error("failed to create vulkan instance");
	}

	// setup debug callback
	if (VALIDATION_LAYERS_ENABLED) {
		VkDebugReportCallbackCreateInfoEXT debugReportCallbackCreateInfo = {};
		debugReportCallbackCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
		debugReportCallbackCreateInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
		debugReportCallbackCreateInfo.pfnCallback = [](VkDebugReportFlagsEXT, VkDebugReportObjectTypeEXT, uint64_t,
				size_t, int32_t, const char*, const char* msg, void*) -> VkBool32 {
			std::cerr << "validation layer: " << msg << std::endl;
			return VK_FALSE;
		};
		auto createDebugReportCallbackFunction = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, 
			"vkCreateDebugReportCallbackEXT");
		if (createDebugReportCallbackFunction == nullptr || createDebugReportCallbackFunction(instance, 
				&debugReportCallbackCreateInfo, nullptr, &debugReportCallback) != VK_SUCCESS) {
			throw std::runtime_error("failed to setup debug callback");
		}
	}
}

VulkanInstance::~VulkanInstance() {
	// destroy debug callback
	auto destroyDebugReportCallbackFunction = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
    if (destroyDebugReportCallbackFunction != nullptr) {
        destroyDebugReportCallbackFunction(instance, debugReportCallback, nullptr);
    }

    // destroy instance
    vkDestroyInstance(instance, nullptr);
}

std::shared_ptr<VulkanInstance> VulkanInstance::create() {
	return std::make_shared<VulkanInstance>();
}