#include "model_loader.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <unordered_map>

void ModelLoader::generateNormals(std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices) {
	for (Vertex& v : vertices) {
		v.normal = glm::vec3(0, 0, 0);
	}

	for (unsigned int i = 0; i < indices.size(); i += 3) {
		Vertex& a = vertices[indices[i]];
		Vertex& b = vertices[indices[i + 1]];
		Vertex& c = vertices[indices[i + 2]];
		glm::vec3 partialNormal = glm::cross(b.position - a.position, c.position - a.position);
		a.normal += partialNormal;
		b.normal += partialNormal;
		c.normal += partialNormal;
	}

	for (Vertex& v : vertices) {
		v.normal = glm::normalize(v.normal);
	}
}


Model* ModelLoader::loadObjFile(const Device &device, const std::string &filename, const bool smoothShading) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string error;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &error, filename.c_str())) {
		throw std::runtime_error(error);
	}

	const bool hasNormals = !attrib.normals.empty();
	const bool hasTexcoords = !attrib.texcoords.empty();

	std::unordered_map<Vertex, uint16_t> uniqueVertices;
	std::vector<Vertex> vertices;
	std::vector<uint16_t> indices;

	for (const tinyobj::shape_t& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec2 texcoord;

			position = glm::vec3(
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			);

			if (hasNormals) {
				normal = glm::vec3(
					attrib.normals[3 * index.normal_index + 0],
					attrib.normals[3 * index.normal_index + 1],
					attrib.normals[3 * index.normal_index + 2]
				);
			}

			if (hasTexcoords) {
				texcoord = glm::vec2(
					attrib.texcoords[2 * index.texcoord_index + 0],
					attrib.texcoords[2 * index.texcoord_index + 1]
				);
			}

			Vertex vertex(position, normal, texcoord);

			if (!smoothShading || uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = vertices.size();
				vertices.push_back(vertex);
			}
			indices.push_back(uniqueVertices[vertex]);
		}
	}

	if (!hasNormals) {
		generateNormals(vertices, indices);
	}

	return new Model(device, vertices, indices);
}