#include "VulkanRenderEngine.h"

#pragma region Constructor & destructor
/// <summary>
/// Constructor of <c>VulkanRenderEngine</c> class.
/// </summary>
VulkanRenderEngine::VulkanRenderEngine(){
}
/// <summary>
/// Destructor of <c>VulkanRenderEngine</c> class.
/// </summary>
VulkanRenderEngine::~VulkanRenderEngine(){
}
#pragma endregion

#pragma region Main methods
/// <summary>
/// Method to launch the graphycs application.
/// </summary>
void VulkanRenderEngine::run(){
	viewport.initWindow(framebufferResizeCallback);
	initVulkan();
	mainLoop();
	cleanup();
}
/// <summary>
/// Initialization of Vulkan elements.
/// </summary>
void VulkanRenderEngine::initVulkan(){
	createVulkanInstance();
	vulkanHelper->setupDebugCallback(instance);
	viewport.createSurface(instance, surface);
	pickPhyshicalDevice();
	createLogicalDevice();
	createSwapChain();
	createImageViews();
	createRenderPass();
	createDescriptorSetLayout();
	createGraphicsPipeline();
	createCommandPool();
	createColorResources();
	createDepthResources();
	createFramebuffers();
	//ENGLOBAR EN METODO CREATESCENE.
	scene.createDefaultScene();
	createUniformBuffer();
	createDescriptorPool();
	createDescriptorSets();
	//ENGLOBAR EN METODO CREATESCENE.
	createCommandBuffers();
	createSyncObjects();
}
/// <summary>
/// Main loop of our application.
/// </summary>
void VulkanRenderEngine::mainLoop(){
	while(!glfwWindowShouldClose(viewport.window)) {
		glfwPollEvents();
		drawFrame();
	}
	vkDeviceWaitIdle(logicalDevice);
}
#pragma endregion

#pragma region Creation methods
/// <summary>
/// Creation of VulkanInstance.
/// </summary>
void VulkanRenderEngine::createVulkanInstance(){
	//Application data.
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello VR";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	//Instance creation data.
	VkInstanceCreateInfo createInfo ={};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	//GLFW extensions.
	auto extensions = vulkanHelper->getRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();
	createInfo.enabledLayerCount = 0;
	//Print extensions.
	vulkanHelper->printExtensions();

	//ValidationLayers.
	if(vulkanHelper->isDebugging() && !vulkanHelper->checkValidationLayerSupport())
		throw std::runtime_error("validation layers requested, but not available!");
	if(vulkanHelper->isDebugging()) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(vulkanHelper->validationLayers.size());
		createInfo.ppEnabledLayerNames = vulkanHelper->validationLayers.data();
	}
	else
		createInfo.enabledLayerCount = 0;

	//Instance creation.
	if(vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
		throw std::runtime_error("failed to create instance!");
}
/// <summary>
/// Creation of logical device.
/// </summary>
void VulkanRenderEngine::createLogicalDevice(){
	VkPhysicalDeviceFeatures deviceFeatures = {};
	//Enable anisotropy (for textures).
	deviceFeatures.samplerAnisotropy = VK_TRUE;

	//Queues creation data.
	QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<int> uniqueQueueFamilies = {indices.graphicsFamily, indices.presentFamily};
	
	float queuePriority = 1.0f;
	for(int queueFamily : uniqueQueueFamilies){
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	//Logical device creation data.
	VkDeviceCreateInfo createInfo = {};
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pEnabledFeatures = &deviceFeatures;
	//Extensions.
	createInfo.enabledExtensionCount = static_cast<uint32_t>(vulkanHelper->deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = vulkanHelper->deviceExtensions.data();
	//Validation layers.
	if(vulkanHelper->isDebugging()){
		createInfo.enabledLayerCount = static_cast<uint32_t>(vulkanHelper->validationLayers.size());
		createInfo.ppEnabledLayerNames = vulkanHelper->validationLayers.data();
	}
	else
		createInfo.enabledLayerCount = 0;
	//Logical device creation.
	if(vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS)
		throw std::runtime_error("failed to create logical device!");

	//Getting queues.
	vkGetDeviceQueue(logicalDevice, indices.graphicsFamily, 0, &graphicsQueue);
	vkGetDeviceQueue(logicalDevice, indices.presentFamily, 0, &presentQueue);

	//Pass data to vulkan helper.
	vulkanHelper->setDeviceData(physicalDevice, logicalDevice, graphicsQueue);
}
/// <summary>
/// Creation of swap chain.
/// </summary>
void VulkanRenderEngine::createSwapChain(){
	//Checking that swap chain is supported.
	SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);
	//Choosing more suitable swap chain settings.
	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

	//Getting count for tripple buffering.
	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if(swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
		imageCount = swapChainSupport.capabilities.maxImageCount;

	//Swap chain creation data.
	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	//Queue family indices data.
	QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
	uint32_t queueFamilyIndices[] ={(uint32_t)indices.graphicsFamily, (uint32_t)indices.presentFamily};
	if(indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0; // Optional
		createInfo.pQueueFamilyIndices = nullptr; // Optional
	}
	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;
	//Swap chain creation.
	if(vkCreateSwapchainKHR(logicalDevice, &createInfo, nullptr, &swapChain) != VK_SUCCESS)
		throw std::runtime_error("failed to create swap chain!");

	//Getting swap chain images.
	vkGetSwapchainImagesKHR(logicalDevice, swapChain, &imageCount, nullptr);
	swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(logicalDevice, swapChain, &imageCount, swapChainImages.data());
	//Getting swap chain surface format and extent.
	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extent;
}
/// <summary>
/// Recreation of swap chain.
/// </summary>
void VulkanRenderEngine::recreateSwapChain(){
	//Synchronization.
	vkDeviceWaitIdle(logicalDevice);
	//Cleanup.
	cleanupSwapChain();
	//Recreation.
	createSwapChain();
	createImageViews();
	createRenderPass();
	createGraphicsPipeline();
	createColorResources();
	createDepthResources();
	createFramebuffers();
	createCommandBuffers();
}
/// <summary>
/// Creation of image views.
/// </summary>
void VulkanRenderEngine::createImageViews(){
	swapChainImageViews.resize(swapChainImages.size());

	//Image views creation.
	for(uint32_t i = 0; i < swapChainImages.size(); i++)
		swapChainImageViews[i] = vulkanHelper->createImageView(swapChainImages[i], swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1, logicalDevice);
}
/// <summary>
/// Creation of descriptor set layout (UBO & samplers).
/// </summary>
void VulkanRenderEngine::createDescriptorSetLayout(){
	//Descriptor set layout binding data: UBO.
	VkDescriptorSetLayoutBinding uboLayoutBinding = {};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
	//Descriptor set layout binding data: sampler.
	VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
	samplerLayoutBinding.binding = 1;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.pImmutableSamplers = nullptr;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	//Descriptor set layout creation data.
	std::array<VkDescriptorSetLayoutBinding, 2> bindings = {uboLayoutBinding, samplerLayoutBinding};
	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();
	//Descriptor set layout creation.
	if(vkCreateDescriptorSetLayout(logicalDevice, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
		throw std::runtime_error("failed to create descriptor set layout!");
}
/// <summary>
/// Creation of descriptor pool.
/// </summary>
void VulkanRenderEngine::createDescriptorPool(){
	//Descriptor pool size data.
	std::array<VkDescriptorPoolSize, 2> poolSizes = {};
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[0].descriptorCount = static_cast<uint32_t>(swapChainImages.size());
	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	//POR HACER --> Si queremos que tengan la misma textura indicamos el valor swapChainImages.size()
	poolSizes[1].descriptorCount = /*static_cast<uint32_t>(scene.models.size());*/ static_cast<uint32_t>(swapChainImages.size());

	//Descriptor pool creation data.
	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = /*static_cast<uint32_t>(scene.models.size());*/ static_cast<uint32_t>(swapChainImages.size());
	//Descriptor pool creation.
	if(vkCreateDescriptorPool(logicalDevice, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
		throw std::runtime_error("failed to create descriptor pool!");
}
/// <summary>
/// Creation of descriptor sets.
/// </summary>
void VulkanRenderEngine::createDescriptorSets(){
	//Descriptor sets allocation info.
	std::vector<VkDescriptorSetLayout> layouts(swapChainImages.size(), descriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = 1;//static_cast<uint32_t>(swapChainImages.size());
	allocInfo.pSetLayouts = layouts.data();
	descriptorSets.resize(swapChainImages.size());
	//Descriptor sets allocation.
	if(vkAllocateDescriptorSets(logicalDevice, &allocInfo, &descriptorSets[0]) != VK_SUCCESS)
		throw std::runtime_error("failed to allocate descriptor sets!");

	for(size_t i = 0; i < swapChainImages.size(); i++) {
	//Descriptor buffer data. //POR HACER --> Cambier esto por el del modelo &cube.uniformBuffer.descriptor;
	VkDescriptorBufferInfo bufferInfo = {};
	bufferInfo.buffer = uniformBuffers[i];
	bufferInfo.offset = 0;
	bufferInfo.range = sizeof(ModelViewProjectionUBO);

	//Write descriptor data: UBO.
	std::array<VkWriteDescriptorSet, 2> descriptorWrites = {};
	descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[0].dstSet = descriptorSets[i];//POR HACER --> Cambiar esto por modelos. cube.descriptorSet;
	descriptorWrites[0].dstBinding = 0;
	descriptorWrites[0].dstArrayElement = 0;
	descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrites[0].descriptorCount = 1;
	descriptorWrites[0].pBufferInfo = &bufferInfo;//POR HACER --> Cambier esto por el del modelo &cube.uniformBuffer.descriptor;

	//Image descriptor data.
	VkDescriptorImageInfo imageInfo = {};
	//POR HACER --> Cambier esto por el del modelo &cube.uniformBuffer.descriptor;
	imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageInfo.imageView = scene.models[1].textureImageView;
	imageInfo.sampler = scene.models[1].textureSampler;
	//Write descriptor data: sampler.
	descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[1].dstSet = descriptorSets[i];//POR HACER --> Cambiar esto por modelos. cube.descriptorSet;
	descriptorWrites[1].dstBinding = 1;
	descriptorWrites[1].dstArrayElement = 0;
	descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descriptorWrites[1].descriptorCount = 1;
	descriptorWrites[1].pImageInfo = &imageInfo;//POR HACER --> Cambier esto por el del modelo &cube.uniformBuffer.descriptor;
	//descriptorWrite.pTexelBufferView = nullptr; // Optional
	//Update descriptor buffers.
	vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}
/// <summary>
/// Creation of graphics pipeline.
/// </summary>
void VulkanRenderEngine::createGraphicsPipeline(){
	//Shaders creation.
	VkShaderModule vertShaderModule;
	VkShaderModule fragShaderModule;
	auto vertShaderCode = readFile("shaders/vert.spv");
	auto fragShaderCode = readFile("shaders/frag.spv");
	vertShaderModule = vulkanHelper->createShaderModule(vertShaderCode, logicalDevice);
	fragShaderModule = vulkanHelper->createShaderModule(fragShaderCode, logicalDevice);

	//Vertex shader data.
	VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";
	//Fragment shader data.
	VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";
	//Pipeline shaders array data.
	VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

	//Vertex shader input data. POR HACER --> GESTIONAR A TRAVÉS DE LA CLASE SCENE
	auto bindingDescription = Vertex::getBindingDescription();
	auto attributeDescriptions = Vertex::getAttributeDescriptions();
	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
	//Input assembly data.
	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	//Viewport data.
	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)swapChainExtent.width;
	viewport.height = (float)swapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	//Scissor data.
	VkRect2D scissor = {};
	scissor.offset = {0, 0};
	scissor.extent = swapChainExtent;
	//Viewport state data.
	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	//Rasterizer data.
	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f; // Optional
	rasterizer.depthBiasClamp = 0.0f; // Optional
	rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

	//Multisampling data.
	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = msaaSamples;
	multisampling.minSampleShading = 1.0f; // Optional
	multisampling.pSampleMask = nullptr; // Optional
	multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
	multisampling.alphaToOneEnable = VK_FALSE; // Optional

	//Blend attachment data.
	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_TRUE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
	//Color blending data.
	VkPipelineColorBlendStateCreateInfo colorBlending = {};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f; // Optional
	colorBlending.blendConstants[1] = 0.0f; // Optional
	colorBlending.blendConstants[2] = 0.0f; // Optional
	colorBlending.blendConstants[3] = 0.0f; // Optional

	//Dynamic statedata .
	VkDynamicState dynamicStates[] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_LINE_WIDTH};
	VkPipelineDynamicStateCreateInfo dynamicState ={};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = 2;
	dynamicState.pDynamicStates = dynamicStates;

	//Depth stencil data.
	VkPipelineDepthStencilStateCreateInfo depthStencil = {};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.minDepthBounds = 0.0f; // Optional
	depthStencil.maxDepthBounds = 1.0f; // Optional
	depthStencil.stencilTestEnable = VK_FALSE;
	depthStencil.front = {}; // Optional
	depthStencil.back = {}; // Optional

	//Pipeline layout creation data.
	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
	pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
	pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional
	//Pipeline layout creation.
	if(vkCreatePipelineLayout(logicalDevice, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
		throw std::runtime_error("failed to create pipeline layout!");

	//Pipeline creation data.
	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = &depthStencil;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = nullptr; // Optional
	pipelineInfo.layout = pipelineLayout;
	pipelineInfo.renderPass = renderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
	pipelineInfo.basePipelineIndex = -1; // Optional
	//Pipeline creation.
	if(vkCreateGraphicsPipelines(logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS)
		throw std::runtime_error("failed to create graphics pipeline!");

	//Shader destruction.
	vkDestroyShaderModule(logicalDevice, fragShaderModule, nullptr);
	vkDestroyShaderModule(logicalDevice, vertShaderModule, nullptr);
}
/// <summary>
/// Creation of render pass.
/// </summary>
void VulkanRenderEngine::createRenderPass(){
	//Color attachment.
	VkAttachmentDescription colorAttachment = {};
	colorAttachment.format = swapChainImageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	colorAttachment.samples = msaaSamples;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	//Color attachment reference.
	VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	//Depth attachment.
	VkAttachmentDescription depthAttachment = {};
	depthAttachment.format = findDepthFormat();
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.samples = msaaSamples;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	//Depth attachment reference.
	VkAttachmentReference depthAttachmentRef = {};
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	//Multisampling color attachment.
	VkAttachmentDescription colorAttachmentResolve = {};
	colorAttachmentResolve.format = swapChainImageFormat;
	colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	//Multisampling color attachment reference.
	VkAttachmentReference colorAttachmentResolveRef = {};
	colorAttachmentResolveRef.attachment = 2;
	colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	//Subpass data.
	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthAttachmentRef;
	subpass.pResolveAttachments = &colorAttachmentResolveRef;
	//Subpass dependency data.
	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	//Render pass creation data.
	std::array<VkAttachmentDescription, 3> attachments = {colorAttachment, depthAttachment, colorAttachmentResolve};
	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;
	//Render pass creation.
	if(vkCreateRenderPass(logicalDevice, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS)
		throw std::runtime_error("failed to create render pass!");
}
/// <summary>
/// Creation of framebuffers.
/// </summary>
void VulkanRenderEngine::createFramebuffers(){
	swapChainFramebuffers.resize(swapChainImageViews.size());
	for(size_t i = 0; i < swapChainImageViews.size(); i++){
		//Framebuffer creation data.
		std::array<VkImageView, 3> attachments = {colorImageView, depthImageView, swapChainImageViews[i]};
		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = swapChainExtent.width;
		framebufferInfo.height = swapChainExtent.height;
		framebufferInfo.layers = 1;
		//Framebuffer creation.
		if(vkCreateFramebuffer(logicalDevice, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS)
			throw std::runtime_error("failed to create framebuffer!");
	}
}
/// <summary>
/// Creation of uniform buffer.
/// </summary>
void VulkanRenderEngine::createUniformBuffer(){
	//Getting size and resizing.
	VkDeviceSize bufferSize = sizeof(ModelViewProjectionUBO);
	uniformBuffers.resize(swapChainImages.size());
	uniformBuffersMemory.resize(swapChainImages.size());
	//Uniform buffers creation.
	for(size_t i = 0; i < swapChainImages.size(); i++)
		vulkanHelper->createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i], logicalDevice, physicalDevice);
}
/// <summary>
/// Creation of depth resources.
/// </summary>
void VulkanRenderEngine::createDepthResources(){
	//Searching de depth format.
	VkFormat depthFormat = findDepthFormat();

	//Creating depth image & depth image view.
	vulkanHelper->createImage(swapChainExtent.width, swapChainExtent.height, 1, msaaSamples, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory, logicalDevice, physicalDevice);
	depthImageView = vulkanHelper->createImageView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1, logicalDevice);

	//Put depth image on a layout.
	vulkanHelper->transitionImageLayout(depthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 1);
}
/// <summary>
/// Creation of command pool.
/// </summary>
void VulkanRenderEngine::createCommandPool(){
	QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice);
	//Command pool creation data.
	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
	poolInfo.flags = 0; // Optional
	//Command pool creation.
	if(vkCreateCommandPool(logicalDevice, &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
		throw std::runtime_error("failed to create command pool!");

	//Pass data to vulkan helper.
	vulkanHelper->setCommandsData(commandPool);
}
/// <summary>
/// Creation of command buffers.
/// </summary>
void VulkanRenderEngine::createCommandBuffers(){
	commandBuffers.resize(swapChainFramebuffers.size());
	//Command buffers allocation data.
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();
	//Command buffers allocation.
	if(vkAllocateCommandBuffers(logicalDevice, &allocInfo, commandBuffers.data()) != VK_SUCCESS)
		throw std::runtime_error("failed to allocate command buffers!");

	for(size_t i = 0; i < commandBuffers.size(); i++){
		//Command buffer recording (begin) data.
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		beginInfo.pInheritanceInfo = nullptr; // Optional
		//Command buffer recording(begin).
		if(vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
			throw std::runtime_error("failed to begin recording command buffer!");

		//Render pass link data.
		VkRenderPassBeginInfo renderPassInfo ={};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = renderPass;
		renderPassInfo.framebuffer = swapChainFramebuffers[i];
		renderPassInfo.renderArea.offset ={0, 0};
		renderPassInfo.renderArea.extent = swapChainExtent;

		//Clear colors.
		std::array<VkClearValue, 2> clearValues = {};
		clearValues[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
		clearValues[1].depthStencil = {1.0f, 0};
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		//Commands execution.
		vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
		//Bind vertex & index buffer.
		VkBuffer vertexBuffers[] = {scene.vertexBuffer};
		VkDeviceSize offsets[] = {0};
		vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets); 
		vkCmdBindIndexBuffer(commandBuffers[i], scene.indexBuffer, 0, VK_INDEX_TYPE_UINT32);
		//Bind uniform buffers.
		vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[i], 0, nullptr);
		//Draw.
		vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(scene.getNumIndices()), 1, 0, 0, 0);
		vkCmdEndRenderPass(commandBuffers[i]);

		//Command buffer recording (end).
		if(vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
			throw std::runtime_error("failed to record command buffer!");
	}
}
/// <summary>
/// Creation of synchronization elements.
/// </summary>
void VulkanRenderEngine::createSyncObjects(){
	//Semaphores creation data.
	imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	VkSemaphoreCreateInfo semaphoreInfo ={};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	//Fences creation data.
	inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
	//Semaphores & fences creation.
	for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
		if(vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS || vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS || vkCreateFence(logicalDevice, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS)
			throw std::runtime_error("failed to create synchronization objects for a frame!");
	}
}
/// <summary>
/// Creation of multisampling color elements.
/// </summary>
void VulkanRenderEngine::createColorResources(){
	VkFormat colorFormat = swapChainImageFormat;

	//Creating multisampling image & multisampling image view.
	vulkanHelper->createImage(swapChainExtent.width, swapChainExtent.height, 1, msaaSamples, colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, colorImage, colorImageMemory, logicalDevice, physicalDevice);
	colorImageView = vulkanHelper->createImageView(colorImage, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1, logicalDevice);

	//Put multisampling image on a layout.
	vulkanHelper->transitionImageLayout(colorImage, colorFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 1);
}
#pragma endregion

#pragma region Query methods
/// <summary>
/// Checks if our device can be used to our application requeriments.
/// <param name="device">Physical device we want to check.</param>
/// <returns>True if the device meet some requirements: graphycs and presentation queue families, extensions, swap chain support and anisotropy feature; false otherwise.</returns> 
/// </summary>
bool VulkanRenderEngine::isDeviceSuitable(VkPhysicalDevice device){
	//Checking queue families
	QueueFamilyIndices indices = findQueueFamilies(device);
	//Checking extensions
	bool extensionsSupported = vulkanHelper->checkDeviceExtensionSupport(device);
	//Cheking swap chain	
	bool swapChainAdequate = false;
	if(extensionsSupported){
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}
	//Check if some features are available.
	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

	return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}
/// <summary>
/// Checks if our device support some indicated queue families and it returns their indices.
/// <param name="device">Physical device we want to check.</param>
/// <returns>The indices of the queue families.</returns> 
/// </summary>
QueueFamilyIndices VulkanRenderEngine::findQueueFamilies(VkPhysicalDevice device){
	//Getting queue families properties.
	QueueFamilyIndices indices;
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for(const auto & queueFamily:queueFamilies){
		//Graphyc checks.
		if(queueFamily.queueCount > 0 && queueFamily.queueFlags &VK_QUEUE_GRAPHICS_BIT)
			indices.graphicsFamily = i;
		//Presentation checks.
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
		if(queueFamily.queueCount > 0 && presentSupport)
			indices.presentFamily = i;

		if(indices.isComplete())
			break;

		i++;
	}

	return indices;
}
/// <summary>
/// Checks if our device support swap chain with surface formats and present modes, furthermore it returns swap chain support details.
/// <param name="device">Physical device we want to check.</param>
/// <returns>The swap chain support details.</returns>
/// </summary>
SwapChainSupportDetails VulkanRenderEngine::querySwapChainSupport(VkPhysicalDevice device){
	//Getting swap chain support details.
	SwapChainSupportDetails details;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	//Getting supported surface formats.
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
	if(formatCount != 0){
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
	}
	//Getting supported present modes.
	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
	if(presentModeCount != 0){
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}
/// <summary>
/// Checks if the format is available and returns it.
/// <param name="candidates">The formats that we accept to use.</param>
/// <param name="tiling">The way we dispose the image texel data.</param>
/// <param name="features">Flags for the format features we want to get supporting.</param>
/// <returns>The format itself.</returns>
/// </summary>
VkFormat VulkanRenderEngine::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features){
	for(VkFormat format : candidates){
		//Getting format properties.
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);
		//Checking available format.
		if(tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) 
			return format;
		else if(tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
			return format;
		else
			throw std::runtime_error("failed to find supported format!");
	}
}
/// <summary>
/// Checks if depth format is available and returns it.
/// <returns>The depth format itself.</returns>
/// </summary>
VkFormat VulkanRenderEngine::findDepthFormat(){
	return findSupportedFormat({VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT}, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}
/// <summary>
/// Get the max number of multisampling samples supported.
/// <returns>The max multisampling samples supported.</returns> 
/// </summary>
VkSampleCountFlagBits VulkanRenderEngine::getMaxUsableSampleCount(){
	VkPhysicalDeviceProperties physicalDeviceProperties;
	vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);
	//Decide the sample count to use.
	VkSampleCountFlags counts = std::min(physicalDeviceProperties.limits.framebufferColorSampleCounts, physicalDeviceProperties.limits.framebufferDepthSampleCounts);
	if(counts & VK_SAMPLE_COUNT_64_BIT)
		return VK_SAMPLE_COUNT_64_BIT;
	if(counts & VK_SAMPLE_COUNT_32_BIT)
		return VK_SAMPLE_COUNT_32_BIT;
	if(counts & VK_SAMPLE_COUNT_16_BIT)
		return VK_SAMPLE_COUNT_16_BIT;
	if(counts & VK_SAMPLE_COUNT_8_BIT)
		return VK_SAMPLE_COUNT_8_BIT;
	if(counts & VK_SAMPLE_COUNT_4_BIT)
		return VK_SAMPLE_COUNT_4_BIT;
	if(counts & VK_SAMPLE_COUNT_2_BIT)
		return VK_SAMPLE_COUNT_2_BIT;
	return VK_SAMPLE_COUNT_1_BIT;
}
#pragma endregion

#pragma region Vulkan configuration methods
/// <summary>
/// Drawing method.
/// </summary>
void VulkanRenderEngine::drawFrame(){
	//Senchronization.
	vkWaitForFences(logicalDevice, 1, &inFlightFences[currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());

	//Getting next frame.
	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(logicalDevice, swapChain, std::numeric_limits<uint64_t>::max(), imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);
	if(result == VK_ERROR_OUT_OF_DATE_KHR){
		recreateSwapChain();
		return;
	}
	else if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		throw std::runtime_error("failed to acquire swap chain image!");
	//Updatting uniform buffers.
	updateUniformBuffer(imageIndex);

	//Submit data.
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	//Wait semaphores data.
	VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
	VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[imageIndex];
	//Signal semaphores data.
	VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	//Synchronization.
	vkResetFences(logicalDevice, 1, &inFlightFences[currentFrame]);

	//Submit.
	if(vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS)
		throw std::runtime_error("failed to submit draw command buffer!");

	//Presentation data.
	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	VkSwapchainKHR swapChains[] = {swapChain};
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr; // Optional
	//Presentation.
	result = vkQueuePresentKHR(presentQueue, &presentInfo);
	if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
		framebufferResized = false;
		recreateSwapChain();
	}
	else if(result != VK_SUCCESS)
		throw std::runtime_error("failed to present swap chain image!");

	//Synchronization operations.
	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}
/// <summary>
/// Update the pipeline uniform buffer with and image.
/// <param name="currentImage">The image we want to include on the updating.</param>
/// </summary>
void VulkanRenderEngine::updateUniformBuffer(uint32_t currentImage){
	//Getting times to update scene.
	static auto startTime = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	//Updating uniform buffer data: model, view & projection matrix.
	ModelViewProjectionUBO ubo = {};
	ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.proj = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f);
	ubo.proj[1][1] *= -1;

	//Copy vertex data to buffer.
	void* data;
	vkMapMemory(logicalDevice, uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
	memcpy(data, &ubo, sizeof(ubo));
	vkUnmapMemory(logicalDevice, uniformBuffersMemory[currentImage]);
}
/// <summary>
/// Decide the hardware selected to use on render.
/// </summary>
void VulkanRenderEngine::pickPhyshicalDevice(){
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
	if(deviceCount == 0)
		throw std::runtime_error("failed to find GPUs with Vulkan support!");

	//Listing & checking physical devices.
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
	for(const auto & device : devices){
		if(isDeviceSuitable(device)){
			physicalDevice = device;
			msaaSamples = getMaxUsableSampleCount();
			break;
		}
	}
	//Check if we can use any of our hardware devices.
	if(physicalDevice == VK_NULL_HANDLE)
		throw std::runtime_error("failed to find a suitable GPU!");

	/*Optional: scoring the graphyc cards.*/
	/* Use an ordered map to automatically sort candidates byincreasing score
	std::multimap<int, VkPhysicalDevice> candidates;
	for(const auto & device:devices){
	int score = rateDeviceSuitability(device);
	candidates.insert(std::make_pair(score, device));
	}
	// Check if the best candidate is suitable at all
	if(candidates.rbegin()->first > 0)
	physicalDevice = candidates.rbegin()->second;
	else
	throw std::runtime_error("failed to find a suitable GPU!");*/
}
/// <summary>
/// Chose swap chain surface format.
/// <param name="availableFormats">The formats that we can use.</param>
/// <returns>The surface format selected.</returns> 
/// </summary>
VkSurfaceFormatKHR VulkanRenderEngine::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats){
	//If we have no surface formats on parameter.
	if(availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED)
		return{VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
	//Check if we have a kind of surface format available and select it.
	for(const auto & availableFormat:availableFormats){
		if(availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			return availableFormat;
	}

	return availableFormats[0];
}
/// <summary>
/// Chose swap chain present mode.
/// <param name="availablePresentModes">The present modes that we can use.</param>
/// <returns>The present mode selected.</returns> 
/// </summary>
VkPresentModeKHR VulkanRenderEngine::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes){
	//Present mode selected by default.
	VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;
	//Check if we have a kind of present mode available and select it.
	for(const auto & availablePresentMode:availablePresentModes) {
		if(availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			return availablePresentMode;
		else if(availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
			bestMode = availablePresentMode;
	}

	return bestMode;
}
/// <summary>
/// Chose swap chain extent.
/// <param name="capabilities">The surface capabilities available.</param>
/// <returns>The swap extent (2D) selected.</returns> 
/// </summary>
VkExtent2D VulkanRenderEngine::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities){
	//Put maximum width allowed extent.
	if(capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		return capabilities.currentExtent;
	else{
		int width, height;
		glfwGetFramebufferSize(viewport.window, &width, &height);
		//Put extent with maximum allowed framebuffer sizes.
		VkExtent2D actualExtent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
		actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

		return actualExtent;
	}
}
#pragma endregion

#pragma region Cleanup methods
/// <summary>
/// Cleanup of Vulkan elements.
/// </summary>
void VulkanRenderEngine::cleanup(){
	//Swap chain elements.
	cleanupSwapChain();
	//Descriptor pool.
	vkDestroyDescriptorPool(logicalDevice, descriptorPool, nullptr);
	//Descriptor set layout and UBOs.
	vkDestroyDescriptorSetLayout(logicalDevice, descriptorSetLayout, nullptr);
	for(size_t i = 0; i < swapChainImages.size(); i++){
		vkDestroyBuffer(logicalDevice, uniformBuffers[i], nullptr);
		vkFreeMemory(logicalDevice, uniformBuffersMemory[i], nullptr);
	}
	//Buffers and textures.
	scene.cleanupTextures();
	scene.cleanupBuffers();
	//Semaphores & fences.
	for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
		vkDestroySemaphore(logicalDevice, renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(logicalDevice, imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(logicalDevice, inFlightFences[i], nullptr);
	}
	//Command pool.
	vkDestroyCommandPool(logicalDevice, commandPool, nullptr);
	//Logical device.
	vkDestroyDevice(logicalDevice, nullptr);
	//VulkanHelper elements: Validation layers.
	vulkanHelper->cleanup(instance);
	//Surface.
	vkDestroySurfaceKHR(instance, surface, nullptr);
	//Vulkan instance.
	vkDestroyInstance(instance, nullptr);
	//Glfw Window.
	viewport.cleanup();
}
/// <summary>
/// Cleanup of Vulkan swap chain.
/// </summary>
void VulkanRenderEngine::cleanupSwapChain(){
	//Multisampling.
	vkDestroyImageView(logicalDevice, colorImageView, nullptr);
	vkDestroyImage(logicalDevice, colorImage, nullptr);
	vkFreeMemory(logicalDevice, colorImageMemory, nullptr);
	//Depth.
	vkDestroyImageView(logicalDevice, depthImageView, nullptr);
	vkDestroyImage(logicalDevice, depthImage, nullptr);
	vkFreeMemory(logicalDevice, depthImageMemory, nullptr);
	//Framebuffers.
	for(size_t i = 0; i < swapChainFramebuffers.size(); i++)
		vkDestroyFramebuffer(logicalDevice, swapChainFramebuffers[i], nullptr);
	//Command buffers.
	vkFreeCommandBuffers(logicalDevice, commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
	//Pipeline.
	vkDestroyPipeline(logicalDevice, graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(logicalDevice, pipelineLayout, nullptr);
	//Render pass.
	vkDestroyRenderPass(logicalDevice, renderPass, nullptr);
	//Image views.
	for(size_t i = 0; i < swapChainImageViews.size(); i++)
		vkDestroyImageView(logicalDevice, swapChainImageViews[i], nullptr);
	//Swap chain.
	vkDestroySwapchainKHR(logicalDevice, swapChain, nullptr);
}
#pragma endregion