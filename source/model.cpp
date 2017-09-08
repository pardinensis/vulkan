#include "model.hpp"

Model::Model(const Device& device)
		: device(device), vertexBuffer(nullptr), indexBuffer(nullptr) {}

Model::Model(const Device& device, const std::vector<Vertex>& vertexData, const std::vector<uint32_t> indexData)
		: Model(device) {
	this->vertexData = vertexData;
	this->indexData = indexData;

	vertexBuffer = new VertexBuffer(device, vertexData);
	indexBuffer = new IndexBuffer(device, indexData);
}

Model::~Model() {
	delete vertexBuffer;
	delete indexBuffer;
}