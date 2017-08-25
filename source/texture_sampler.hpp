#pragma once

#include "common_includes.hpp"

#include "device.hpp"

class TextureSampler {

private:
	const Device& device;

	VkSampler sampler;

public:
	TextureSampler(const Device& device);
	~TextureSampler();

	const VkSampler& getVkSampler() const { return sampler; }
};