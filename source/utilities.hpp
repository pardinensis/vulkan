#pragma once

#include "common_includes.hpp"

#include "device.hpp"

VkCommandBuffer beginSingleTimeCommands(const Device& device);
void endSingleTimeCommands(const Device& device, const VkCommandBuffer& commandBuffer);