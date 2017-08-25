#include "physical_device.hpp"

PhysicalDevice::PhysicalDevice(const VkPhysicalDevice& vkPhysicalDevice, const VkSurfaceKHR& vkSurface)
		: vkPhysicalDevice(vkPhysicalDevice), graphicsFamily(-1), presentFamily(-1) {

	// find queue families
	unsigned int queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &queueFamilyCount, 0);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &queueFamilyCount, queueFamilies.data());
	unsigned int i = 0;
	for (const VkQueueFamilyProperties& queueFamily : queueFamilies) {
		if (queueFamily.queueCount > 0 && (queueFamily.queueFlags | VK_QUEUE_GRAPHICS_BIT)) {
			graphicsFamily = i;
		}
		VkBool32 presentSupport = VK_FALSE;
		vkGetPhysicalDeviceSurfaceSupportKHR(vkPhysicalDevice, i, vkSurface, &presentSupport);
		if (queueFamily.queueCount > 0 && presentSupport) {
			presentFamily = i;
		}
		if (graphicsFamily >= 0 && presentFamily >= 0) {
			break;
		}
		++i;
	}

	// find device extensions
	unsigned int deviceExtensionCount = 0;
	vkEnumerateDeviceExtensionProperties(vkPhysicalDevice, nullptr, &deviceExtensionCount, nullptr);
	extensionProperties.resize(deviceExtensionCount);
	vkEnumerateDeviceExtensionProperties(vkPhysicalDevice, nullptr, &deviceExtensionCount, extensionProperties.data());

	// query surface capabilities
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vkPhysicalDevice, vkSurface, &surfaceCapabilities);

	// query surface formats
	unsigned int surfaceFormatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(vkPhysicalDevice, vkSurface, &surfaceFormatCount, nullptr);
	surfaceFormats.resize(surfaceFormatCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(vkPhysicalDevice, vkSurface, &surfaceFormatCount, surfaceFormats.data());

	// query present modes
	unsigned int presentModeCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(vkPhysicalDevice, vkSurface, &presentModeCount, nullptr);
	presentModes.resize(presentModeCount);
	vkGetPhysicalDeviceSurfacePresentModesKHR(vkPhysicalDevice, vkSurface, &presentModeCount, presentModes.data());
}

bool PhysicalDevice::isSuitable() {
	// check queue families
	if (graphicsFamily < 0 || presentFamily < 0) {
		return false;
	}

	// check device extensions
	std::set<std::string> requiredExtensions(DEVICE_EXTENSIONS.begin(), DEVICE_EXTENSIONS.end());
	for (const VkExtensionProperties& extension : extensionProperties) {
		requiredExtensions.erase(extension.extensionName);
	}
	if (!requiredExtensions.empty()) {
		return false;
	}

	// check surface formats
	if (surfaceFormats.empty()) {
		return false;
	}

	// check present modes
	if (presentModes.empty()) {
		return false;
	}

	// check for anisotropic filtering
	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(vkPhysicalDevice, &supportedFeatures);
	if (!supportedFeatures.samplerAnisotropy) {
		return false;
	}

	return true;
}

PhysicalDevice* PhysicalDevice::choosePhysicalDevice(const Instance& vulkanInstance, const VkSurfaceKHR& vkSurface) {
	unsigned int physicalDeviceCount = 0;
	vkEnumeratePhysicalDevices(vulkanInstance.getVkInstance(), &physicalDeviceCount, nullptr);

	if (physicalDeviceCount == 0) {
		throw std::runtime_error("failed to find a GPU with  support");
	}

	std::vector<VkPhysicalDevice> vkPhysicalDevices(physicalDeviceCount);
	vkEnumeratePhysicalDevices(vulkanInstance.getVkInstance(), &physicalDeviceCount, vkPhysicalDevices.data());

	for (const VkPhysicalDevice& vkPhysicalDevice : vkPhysicalDevices) {
		PhysicalDevice* physicalDevice = new PhysicalDevice(vkPhysicalDevice, vkSurface);

		if (physicalDevice->isSuitable()) {
			return physicalDevice;
		}

		delete physicalDevice;
	}

	throw std::runtime_error("failed to find a suitable GPU");
}
