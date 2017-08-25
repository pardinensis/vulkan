#include "swapchain.hpp"

#include "config.hpp"

Swapchain::Swapchain(const PhysicalDevice& vulkanPhysicalDevice, const Device& vulkanDevice, const VkSurfaceKHR& surface, const VkExtent2D& preferredExtent)
		: vulkanPhysicalDevice(vulkanPhysicalDevice), vulkanDevice(vulkanDevice) {


	VkSurfaceFormatKHR surfaceFormat = chooseSurfaceFormat();
	VkPresentModeKHR presentMode = choosePresentMode();
	VkExtent2D extent = chooseExtent(preferredExtent);

	// set image count
	const VkSurfaceCapabilitiesKHR& capabilities = vulkanPhysicalDevice.getSurfaceCapabilities();
	uint32_t imageCount = capabilities.minImageCount + 1;
	if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) {
		imageCount = capabilities.maxImageCount;
	}

	// create swapchain
	VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
	swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainCreateInfo.surface = surface;
	swapchainCreateInfo.minImageCount = imageCount;
	swapchainCreateInfo.imageFormat = surfaceFormat.format;
	swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
	swapchainCreateInfo.imageExtent = extent;
	swapchainCreateInfo.imageArrayLayers = 1;
	swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	uint32_t queueFamilyIndices[] = { (uint32_t) vulkanPhysicalDevice.getGraphicsFamily(), (uint32_t) vulkanPhysicalDevice.getPresentFamily() };
	if (vulkanPhysicalDevice.getGraphicsFamily() != vulkanPhysicalDevice.getPresentFamily()) {
		swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		swapchainCreateInfo.queueFamilyIndexCount = 2;
		swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	swapchainCreateInfo.preTransform = capabilities.currentTransform;
	swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchainCreateInfo.presentMode = presentMode;
	swapchainCreateInfo.clipped = VK_TRUE;
	swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(vulkanDevice.getVkDevice(), &swapchainCreateInfo, nullptr, &swapchain) != VK_SUCCESS) {
		throw std::runtime_error("failed to create swap chain");
	}

	// retrieve swapchain images
	vkGetSwapchainImagesKHR(vulkanDevice.getVkDevice(), swapchain, &imageCount, nullptr);
	swapchainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(vulkanDevice.getVkDevice(), swapchain, &imageCount, swapchainImages.data());

	// store final swapchain parameters
	swapchainImageFormat = surfaceFormat.format;
	swapchainExtent = extent;

	// create image views
	for (size_t i = 0; i < swapchainImages.size(); ++i) {
		swapchainImageViews.push_back(new ImageView(vulkanDevice, swapchainImages[i], swapchainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT));
	}
}

Swapchain::~Swapchain() {
	for (size_t i = 0; i < swapchainImageViews.size(); ++i) {
		delete swapchainImageViews[i];
	};
	swapchainImageViews.clear();

	vkDestroySwapchainKHR(vulkanDevice.getVkDevice(), swapchain, nullptr);
}

VkSurfaceFormatKHR Swapchain::chooseSurfaceFormat() {
	const std::vector<VkSurfaceFormatKHR>& availableFormats = vulkanPhysicalDevice.getSurfaceFormats();
	if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
		return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
	}

	for (const VkSurfaceFormatKHR& format : availableFormats) {
		if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return format;
		}
	}

	return availableFormats[0];
}

VkPresentModeKHR Swapchain::choosePresentMode() {
	const std::vector<VkPresentModeKHR>& availableModes = vulkanPhysicalDevice.getPresentModes();
	for (const VkPresentModeKHR& mode : availableModes) {
		if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return mode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Swapchain::chooseExtent(const VkExtent2D& preferredExtent) {
	const VkSurfaceCapabilitiesKHR& capabilities = vulkanPhysicalDevice.getSurfaceCapabilities();
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		// return capabilities.currentExtent;
	}

	VkExtent2D actualExtent = preferredExtent;
	// actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
	// actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
	return actualExtent;
}

uint32_t Swapchain::aquireNextImage(const Semaphore& imageAquiredSemaphore) {
	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(vulkanDevice.getVkDevice(), swapchain,
		std::numeric_limits<uint64_t>::max(), imageAquiredSemaphore.getVkSemaphore(), VK_NULL_HANDLE, &imageIndex);

	switch (result) {
		case VK_SUBOPTIMAL_KHR:
		case VK_ERROR_OUT_OF_DATE_KHR:
			return -1;
		case VK_SUCCESS:
			return imageIndex;
		default:
			throw std::runtime_error("failed to aquire swapchain image");
	}
}

uint32_t Swapchain::presentImage(uint32_t imageIndex, const Semaphore &renderFinishedSemaphore) {
	VkSwapchainKHR swapchains[] = { swapchain };
	VkSemaphore signalSemaphores[] = { renderFinishedSemaphore.getVkSemaphore() };

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapchains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr;

	VkResult result = vkQueuePresentKHR(vulkanDevice.getVkPresentQueue(), &presentInfo);
	switch (result) {
		case VK_ERROR_OUT_OF_DATE_KHR:
			return -1;
		case VK_SUCCESS:
		case VK_SUBOPTIMAL_KHR:
			return imageIndex;
		default:
			throw std::runtime_error("failed to present swapchain image");
	}
}