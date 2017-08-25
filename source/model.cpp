#include "model.hpp"

Model::Model(const Device& device)
		: device(device), vertexBuffer(nullptr), indexBuffer(nullptr) {}

Model::Model(const Device& device, const std::vector<Vertex>& vertexData, const std::vector<uint16_t> indexData)
		: Model(device) {

	vertexBuffer = new VertexBuffer(device, vertexData);
	indexBuffer = new IndexBuffer(device, indexData);
}

Model::~Model() {
	delete vertexBuffer;
	delete indexBuffer;
}