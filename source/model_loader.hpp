#pragma once

#include "device.hpp"
#include "model.hpp"

class ModelLoader {
private:
	static void generateNormals(std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

public:
	static Model* loadObjFile(const Device& device, const std::string& filename, const bool smoothShading);
};