#pragma once

#include "device.hpp"
#include "index_buffer.hpp"
#include "vertex_buffer.hpp"

class Model {
private:
	const Device& device;

	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

	glm::mat4 modelMatrix;	


public:
	Model(const Device& device);
	Model(const Device& device, const std::vector<Vertex>& vertexData, const std::vector<uint16_t> indexData);

	~Model();

	const VertexBuffer& getVertexBuffer() const { return *vertexBuffer; }
	const IndexBuffer& getIndexBuffer() const { return *indexBuffer; }
	const glm::mat4& getModelMatrix() const { return modelMatrix; }

	void setModelMatrix(const glm::mat4& modelMatrix) { this->modelMatrix = modelMatrix; }

};