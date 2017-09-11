#pragma once

#include "common_includes.hpp"

#include "descriptor_set.hpp"
#include "device.hpp"
#include "framebuffers.hpp"
#include "index_buffer.hpp"
#include "instance_buffer.hpp"
#include "pipeline.hpp"
#include "vertex_buffer.hpp"

class CommandBuffers {
private:
	const Device& device;

	std::vector<VkCommandBuffer> commandBuffers;

public:
	CommandBuffers(const Device& device, const RenderPass& renderPass, const Pipeline& pipeline,
		const Framebuffers& framebuffers, const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer,
		const InstanceBuffer& instanceBuffer, const DescriptorSet& descriptorSet);
	~CommandBuffers();

	void submit(uint32_t imageIndex, const Semaphore& imageAquiredSemaphore, const Semaphore& renderFinishedSemaphore);
};