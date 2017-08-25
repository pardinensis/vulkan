#include "model_loader.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

Model* ModelLoader::loadObjFile(const Device &device, const std::string &filename) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string error;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &error, filename.c_str())) {
		throw std::runtime_error(error);
	}

	std::vector<Vertex> vertices;
	std::vector<uint16_t> indices;

	for (const tinyobj::shape_t& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			glm::vec3 pos(
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			);
			glm::vec3 color(1.0f, 1.0f, 1.0f);
			glm::vec2 tc(
				attrib.vertices[2 * index.texcoord_index + 0],
				attrib.vertices[2 * index.texcoord_index + 1]
			);
			Vertex vertex(pos, color, tc);
			vertices.push_back(vertex);
			indices.push_back(indices.size());
		}
	}

	return new Model(device, vertices, indices);
}