#include "vulkan_command_buffers.hpp"


VulkanCommandBuffers::VulkanCommandBuffers(const VulkanDevice& device, const VulkanRenderPass& renderPass, const VulkanPipeline& pipeline,
		const VulkanFramebuffers& framebuffers, const VulkanVertexBuffer& vertexBuffer, const VulkanIndexBuffer& indexBuffer, const VulkanDescriptorSet& descriptorSet)
		: device(device) {
	const VkDevice& vkDevice = device.getVkDevice();

	// create command buffers
	commandBuffers.resize(framebuffers.getFramebufferCount());

	VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.commandPool = device.getVkCommandPool();
	commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	commandBufferAllocateInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
	if (vkAllocateCommandBuffers(vkDevice, &commandBufferAllocateInfo, commandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers");
	}

	// record command buffers
	for (size_t i = 0; i < commandBuffers.size(); ++i) {
		VkCommandBufferBeginInfo commandBufferBeginInfo = {};
		commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		commandBufferBeginInfo.pInheritanceInfo = nullptr;
		vkBeginCommandBuffer(commandBuffers[i], &commandBufferBeginInfo);

		VkClearValue clearValue = { 0.0f, 0.0f, 0.0f, 0.0f };

		VkRenderPassBeginInfo renderPassBeginInfo = {};
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.renderPass = renderPass.getVkRenderPass();
		renderPassBeginInfo.framebuffer = framebuffers.getVkFramebuffers()[i];
		renderPassBeginInfo.renderArea.offset = { 0, 0 };
		renderPassBeginInfo.renderArea.extent = framebuffers.getVkExtent();
		renderPassBeginInfo.clearValueCount = 1;
		renderPassBeginInfo.pClearValues = &clearValue;
		
		vkCmdBeginRenderPass(commandBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getVkPipeline());

		VkBuffer vertexBuffers[] = { vertexBuffer.getBuffer().getVkBuffer() };
		VkDeviceSize vertexOffsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, vertexOffsets);
		vkCmdBindIndexBuffer(commandBuffers[i], indexBuffer.getBuffer().getVkBuffer(), 0, VK_INDEX_TYPE_UINT16);
		vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getVkPipelineLayout(), 0, 1, &descriptorSet.getVkDescriptorSet(), 0, nullptr);
		vkCmdDrawIndexed(commandBuffers[i], indexBuffer.getIndexCount(), 1, 0, 0, 0);
		
		vkCmdEndRenderPass(commandBuffers[i]);

		if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer");
		}
	}
}


void VulkanCommandBuffers::submit(uint32_t imageIndex, const VulkanSemaphore& imageAquiredSemaphore, const VulkanSemaphore& renderFinishedSemaphore) {
	VkSemaphore waitSemaphores[] = { imageAquiredSemaphore.getVkSemaphore() };
	VkSemaphore signalSemaphores[] = { renderFinishedSemaphore.getVkSemaphore() };
	VkPipelineStageFlags pipelineStageFlags[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = pipelineStageFlags;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[imageIndex];
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;
	if (vkQueueSubmit(device.getVkGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer");
	}
}

VulkanCommandBuffers::~VulkanCommandBuffers() {
	vkFreeCommandBuffers(device.getVkDevice(), device.getVkCommandPool(), commandBuffers.size(), commandBuffers.data());
}