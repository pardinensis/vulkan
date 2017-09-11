#pragma once

#include "device.hpp"
#include "index_buffer.hpp"
#include "vertex_buffer.hpp"


class Model {
private:
	const Device& device;

	std::vector<Vertex> vertexData;
	std::vector<uint32_t> indexData;

	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

	glm::mat4 modelMatrix;	


public:
	Model(const Device& device);
	Model(const Device& device, const std::vector<Vertex>& vertexData, const std::vector<uint32_t> indexData);

	~Model();

	const std::vector<Vertex>& getVertexData() const { return vertexData; }
	const std::vector<uint32_t>& getIndexData() const { return indexData; }

	const VertexBuffer& getVertexBuffer() const { return *vertexBuffer; }
	const IndexBuffer& getIndexBuffer() const { return *indexBuffer; }
	const glm::mat4& getModelMatrix() const { return modelMatrix; }

	void setModelMatrix(const glm::mat4& modelMatrix) { this->modelMatrix = modelMatrix; }

};