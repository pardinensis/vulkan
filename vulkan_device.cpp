#include "vulkan_device.hpp"

#include "config.hpp"
#include "vulkan_validation_layers.hpp"

VulkanDevice::VulkanDevice(VulkanPhysicalDevice& physicalDevice) {
	// gather queue info
	std::vector<VkDeviceQueueCreateInfo> queueInfos;
	std::set<int> uniqueQueueFamilies = { physicalDevice.getGraphicsFamily(), physicalDevice.getPresentFamily() };
	float queuePriority = 1.0f;
	for (int queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo deviceQueueCreateInfo = {};
		deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		deviceQueueCreateInfo.queueFamilyIndex = queueFamily;
		deviceQueueCreateInfo.queueCount = 1;
		deviceQueueCreateInfo.pQueuePriorities = &queuePriority;
		queueInfos.push_back(deviceQueueCreateInfo);
	}

	// get device features (none)
	VkPhysicalDeviceFeatures physicalDeviceFeatures = {};

	// get enabled layers
	std::vector<const char*> enabledLayers;
	if (VALIDATION_LAYERS_ENABLED)
		enabledLayers = VulkanValidationLayers::validationLayers;

	// create device
	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueInfos.size());
	deviceCreateInfo.pQueueCreateInfos = queueInfos.data();
	deviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;
	deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(physicalDevice.DEVICE_EXTENSIONS.size());
	deviceCreateInfo.ppEnabledExtensionNames = physicalDevice.DEVICE_EXTENSIONS.data();
	deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(enabledLayers.size());
	deviceCreateInfo.ppEnabledLayerNames = enabledLayers.data();
	if (vkCreateDevice(physicalDevice.getVkPhysicalDevice(), &deviceCreateInfo, nullptr, &device) != VK_SUCCESS) {
		throw std::runtime_error("failed to create logical device");
	}

	vkGetDeviceQueue(device, physicalDevice.getGraphicsFamily(), 0, &graphicsQueue);
	vkGetDeviceQueue(device, physicalDevice.getPresentFamily(), 0, &presentQueue);

	VkCommandPoolCreateInfo commandPoolCreateInfo = {};
	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolCreateInfo.queueFamilyIndex = physicalDevice.getGraphicsFamily();
	commandPoolCreateInfo.flags = 0;
	if (vkCreateCommandPool(device, &commandPoolCreateInfo, nullptr, &commandPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create command pool");
	}
}

VulkanDevice::~VulkanDevice() {
	vkDestroyCommandPool(device, commandPool, nullptr);
	vkDestroyDevice(device, nullptr);
}