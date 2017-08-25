#pragma once

#include "device.hpp"
#include "model.hpp"

class ModelLoader {

public:
	static Model* loadObjFile(const Device& device, const std::string& filename);
};