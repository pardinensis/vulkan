#pragma once

#include "common_includes.hpp"

#include <glm/gtx/hash.hpp>

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;

	Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texCoord) : position(position), normal(normal), texCoord(texCoord) {}

	bool operator==(const Vertex& other) const {
		return position == other.position && normal == other.normal && texCoord == other.texCoord;
	}

	static VkVertexInputBindingDescription getBindingDescription() {
		VkVertexInputBindingDescription bindingDescription = {};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescription;
	}

	static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions() {
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions(3);
		
		// position
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, position);

		// normal
		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, normal);

		// texture coordinates
		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

		return attributeDescriptions;
	}
};

namespace std {
	template<> struct hash<Vertex> {
		size_t operator()(Vertex const& vertex) const {
			return ((hash<glm::vec3>()(vertex.position) ^
				   (hash<glm::vec3>()(vertex.normal) << 1)) >> 1) ^ 
				   (hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};
}