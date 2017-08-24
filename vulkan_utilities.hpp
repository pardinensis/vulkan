#pragma once

#include "common_includes.hpp"
#include "vulkan_device.hpp"

VkCommandBuffer beginSingleTimeCommands(const VulkanDevice& device);
void endSingleTimeCommands(const VulkanDevice& device, const VkCommandBuffer& commandBuffer);