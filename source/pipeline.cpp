#include "pipeline.hpp"

#include "vertex.hpp"

Pipeline::Pipeline(const Device& device, const RenderPass& renderPass, const VkExtent2D& vkExtent,
	const Shader& shader, const DescriptorSet& descriptorSet) 
		: device(device) {

	// setup vertex input format
	auto bindingDescription = Vertex::getBindingDescription();
	auto attributeDescriptions = Vertex::getAttributeDescriptions();

	// setup instance input format
	VkVertexInputBindingDescription instanceBindingDescription = {};
	instanceBindingDescription.binding = 1;
	instanceBindingDescription.stride = sizeof(glm::vec3);
	instanceBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;
	std::vector<VkVertexInputBindingDescription> bindingDescriptions = { bindingDescription, instanceBindingDescription };

	VkVertexInputAttributeDescription instanceModelAttributeDescription = {};
	instanceModelAttributeDescription.binding = 1;
	instanceModelAttributeDescription.location = 3;
	instanceModelAttributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
	instanceModelAttributeDescription.offset = 0;
	attributeDescriptions.push_back(instanceModelAttributeDescription);

	VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {};
	vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputStateCreateInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
	vertexInputStateCreateInfo.pVertexBindingDescriptions = bindingDescriptions.data();
	vertexInputStateCreateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());;
	vertexInputStateCreateInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = {};
	inputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;


	// setup viewport
	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float) vkExtent.width;
	viewport.height = (float) vkExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent = vkExtent;

	VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {};
	viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportStateCreateInfo.viewportCount = 1;
	viewportStateCreateInfo.pViewports = &viewport;
	viewportStateCreateInfo.scissorCount = 1;
	viewportStateCreateInfo.pScissors = &scissor;

	// setup rasterizer
	VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = {};
	rasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
	rasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
	rasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL; // VK_POLYGON_MODE_FILL
	rasterizationStateCreateInfo.lineWidth = 1.0f;
	rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT; // VK_CULL_MODE_BACK_BIT
	rasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;

	// setup multisampling (disabled)
	VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo = {};
	multisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
	multisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampleStateCreateInfo.minSampleShading = 1.0f;
	multisampleStateCreateInfo.pSampleMask = nullptr;
	multisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
	multisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;

	// setup blending
	VkPipelineColorBlendAttachmentState colorBlendAttachmentState = {};
	colorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachmentState.blendEnable = VK_FALSE;
	colorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;

	VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = {};
	colorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
	colorBlendStateCreateInfo.logicOp = VK_LOGIC_OP_COPY;
	colorBlendStateCreateInfo.attachmentCount = 1;
	colorBlendStateCreateInfo.pAttachments = &colorBlendAttachmentState;
	colorBlendStateCreateInfo.blendConstants[0] = 0.0f;
	colorBlendStateCreateInfo.blendConstants[1] = 0.0f;
	colorBlendStateCreateInfo.blendConstants[2] = 0.0f;
	colorBlendStateCreateInfo.blendConstants[3] = 0.0f;

	// setup depth and stencil state
	VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo = {};
	depthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencilStateCreateInfo.depthTestEnable = VK_TRUE;
	depthStencilStateCreateInfo.depthWriteEnable = VK_TRUE;
	depthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
	depthStencilStateCreateInfo.minDepthBounds = 0.0f;
	depthStencilStateCreateInfo.maxDepthBounds = 1.0f;
	depthStencilStateCreateInfo.stencilTestEnable = VK_FALSE;
	depthStencilStateCreateInfo.front = {};
	depthStencilStateCreateInfo.back = {};

	// create pipeline layout
	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutCreateInfo.setLayoutCount = 1;
	pipelineLayoutCreateInfo.pSetLayouts = &descriptorSet.getVkDescriptorSetLayout();
	pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
	pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;
	if (vkCreatePipelineLayout(device.getVkDevice(), &pipelineLayoutCreateInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout");
	}

	VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {};
	graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	graphicsPipelineCreateInfo.stageCount = static_cast<uint32_t>(shader.getVkShaderStages().size());
	graphicsPipelineCreateInfo.pStages = shader.getVkShaderStages().data();
	graphicsPipelineCreateInfo.pVertexInputState = &vertexInputStateCreateInfo;
	graphicsPipelineCreateInfo.pInputAssemblyState = &inputAssemblyStateCreateInfo;
	graphicsPipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
	graphicsPipelineCreateInfo.pRasterizationState = &rasterizationStateCreateInfo;
	graphicsPipelineCreateInfo.pMultisampleState = &multisampleStateCreateInfo;
	graphicsPipelineCreateInfo.pDepthStencilState = &depthStencilStateCreateInfo;
	graphicsPipelineCreateInfo.pColorBlendState = &colorBlendStateCreateInfo;
	graphicsPipelineCreateInfo.pDynamicState = nullptr;
	graphicsPipelineCreateInfo.layout = pipelineLayout;
	graphicsPipelineCreateInfo.renderPass = renderPass.getVkRenderPass();
	graphicsPipelineCreateInfo.subpass = 0;
	graphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
	graphicsPipelineCreateInfo.basePipelineIndex = -1;

	if (vkCreateGraphicsPipelines(device.getVkDevice(), VK_NULL_HANDLE, 1, &graphicsPipelineCreateInfo, nullptr, &pipeline) != VK_SUCCESS) {
		throw std::runtime_error("failed to create graphics pipeline");
	}
}


Pipeline::~Pipeline() {
	vkDestroyPipeline(device.getVkDevice(), pipeline, nullptr);
	vkDestroyPipelineLayout(device.getVkDevice(), pipelineLayout, nullptr);
}