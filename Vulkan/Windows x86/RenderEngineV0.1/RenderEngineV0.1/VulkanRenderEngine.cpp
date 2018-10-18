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

#pragma region Main methods POR HACER --> INTENTAR LLEVAR initwintdow A VIEWPORT CLASS
/// <summary>
/// Method to launch the graphyc application.
/// </summary>
void VulkanRenderEngine::run(){
	initWindow();
	initVulkan();
	mainLoop();
	cleanup();
}
/// <summary>
/// Initialization of Vulkan elements.
/// </summary>
void VulkanRenderEngine::initVulkan(){
	createVulkanInstance();
	vulkanHelper.setupDebugCallback(instance);
	createSurface();
	pickPhyshicalDevice();
	createLogicalDevice();
	createSwapChain();
	createImageViews();
	createRenderPass();
	createGraphicsPipeline();
	createFramebuffers();
	createCommandPool();
	createVertexBuffer();
	createCommandBuffers();
	createSyncObjects();
}
/// <summary>
/// Initalize window elements.
/// </summary>
void VulkanRenderEngine::initWindow(){
	glfwInit();
	//Avoid set OpenGL as default render API and the resizable option for the window. 
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); //Window resize
	viewport.window = glfwCreateWindow(viewport.WIDTH, viewport.HEIGHT, "Vulkan", nullptr, nullptr);
	glfwSetWindowUserPointer(viewport.window, this);
	glfwSetFramebufferSizeCallback(viewport.window, VulkanRenderEngine::framebufferResizeCallback);
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
	VkApplicationInfo appInfo ={};
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

	//GLFW extensions. POR HACER --> METODO GETEXTENSIONS EN VULKANHELPER parameter createInfo
	auto extensions = vulkanHelper.getRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();
	createInfo.enabledLayerCount = 0;
	//Print extensions.
	vulkanHelper.printExtensions();

	//ValidationLayers. POR HACER --> METODO GETVALIDATIONLAYERS EN VULKANHELPER parameter createInfo
	if(vulkanHelper.isDebugging() && !vulkanHelper.checkValidationLayerSupport())
		throw std::runtime_error("validation layers requested, but not available!");
	if(vulkanHelper.isDebugging()) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(vulkanHelper.validationLayers.size());
		createInfo.ppEnabledLayerNames = vulkanHelper.validationLayers.data();
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
void VulkanRenderEngine::createLogicalDevice(){ // POR HACER --> REVISAR SI PODEMOS LLEVAR ALGO A LA CLASE VULKANHELPER
	VkPhysicalDeviceFeatures deviceFeatures = {};

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
	createInfo.enabledExtensionCount = static_cast<uint32_t>(vulkanHelper.deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = vulkanHelper.deviceExtensions.data();
	//Validation layers.
	if(vulkanHelper.isDebugging()){
		createInfo.enabledLayerCount = static_cast<uint32_t>(vulkanHelper.validationLayers.size());
		createInfo.ppEnabledLayerNames = vulkanHelper.validationLayers.data();
	}
	else
		createInfo.enabledLayerCount = 0;
	//Logical device creation.
	if(vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS)
		throw std::runtime_error("failed to create logical device!");

	//Getting queues.
	vkGetDeviceQueue(logicalDevice, indices.graphicsFamily, 0, &graphicsQueue);
	vkGetDeviceQueue(logicalDevice, indices.presentFamily, 0, &presentQueue);
}
/// <summary>
/// Creation of surface.
/// </summary>
void VulkanRenderEngine::createSurface(){ // POR HACER --> REVISAR SI PODEMOS LLEVAR ALGO A LA CLASE VULKANHELPER
	//Surface creation.
	if(glfwCreateWindowSurface(instance, viewport.window, nullptr, &surface) != VK_SUCCESS)
		throw std::runtime_error("failed to create window surface!");
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
	int width = 0, height = 0;
	while(width == 0 || height == 0) {
		glfwGetFramebufferSize(viewport.window, &width, &height);
		glfwWaitEvents();
	}
	//Synchronization.
	vkDeviceWaitIdle(logicalDevice);
	//Cleanup.
	cleanupSwapChain();
	//Recreation.
	createSwapChain();
	createImageViews();
	createRenderPass();
	createGraphicsPipeline();
	createFramebuffers();
	createCommandBuffers();
}
/// <summary>
/// Creation of image views.
/// </summary>
void VulkanRenderEngine::createImageViews(){
	swapChainImageViews.resize(swapChainImages.size());
	//Image views creation.
	for(uint32_t i=0; i<swapChainImages.size(); i++){
		//Image view creation data.
		VkImageViewCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = swapChainImages[i];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = swapChainImageFormat;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;
		//Image view creation.
		if(vkCreateImageView(logicalDevice, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS)
			throw std::runtime_error("failed to create image views!");
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
	vertShaderModule = createShaderModule(vertShaderCode);
	fragShaderModule = createShaderModule(fragShaderCode);

	//Vertex shader.
	VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";
	//Fragment shader.
	VkPipelineShaderStageCreateInfo fragShaderStageInfo ={};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";
	//Pipeline shaders array data.
	VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

	//Vertex shader input. POR HACER --> GESTIONAR A TRAVÉS DE LA CLASE SCENE
	auto bindingDescription = Vertex::getBindingDescription();
	auto attributeDescriptions = Vertex::getAttributeDescriptions();
	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
	//Input assembly.
	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	//Viewport.
	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)swapChainExtent.width;
	viewport.height = (float)swapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	//Scissor.
	VkRect2D scissor = {};
	scissor.offset = {0, 0};
	scissor.extent = swapChainExtent;
	//Viewport state.
	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	//Rasterizer.
	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f; // Optional
	rasterizer.depthBiasClamp = 0.0f; // Optional
	rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

	//Multisampling.
	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampling.minSampleShading = 1.0f; // Optional
	multisampling.pSampleMask = nullptr; // Optional
	multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
	multisampling.alphaToOneEnable = VK_FALSE; // Optional

	//Blend attachment.
	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_TRUE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
	//Color blending.
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

	//Dynamic state.
	VkDynamicState dynamicStates[] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_LINE_WIDTH};
	VkPipelineDynamicStateCreateInfo dynamicState ={};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = 2;
	dynamicState.pDynamicStates = dynamicStates;

	//Pipeline layuout creation data.
	VkPipelineLayoutCreateInfo pipelineLayoutInfo ={};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0; // Optional
	pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
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
	pipelineInfo.pDepthStencilState = nullptr; // Optional
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
	//Color attachment reference.
	VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	//Subpass data.
	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	//Subpass dependency data.
	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	//Render pass creation data.
	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
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
		VkImageView attachments[] = {swapChainImageViews[i]};
		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = swapChainExtent.width;
		framebufferInfo.height = swapChainExtent.height;
		framebufferInfo.layers = 1;
		//Framebuffer creation.
		if(vkCreateFramebuffer(logicalDevice, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS)
			throw std::runtime_error("failed to create framebuffer!");
	}
}
/// <summary>
/// Creation of vertex buffer.
/// </summary>
void VulkanRenderEngine::createVertexBuffer(){ //POR HACER --> GESTIONAR A TRAVÉS DE LA CLASE SCENE
	//Buffer creation data.
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = sizeof(vertices[0]) * vertices.size();
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	//Buffer creation.
	if(vkCreateBuffer(logicalDevice, &bufferInfo, nullptr, &vertexBuffer) != VK_SUCCESS)
		throw std::runtime_error("failed to create vertex buffer!");

	//Getting necessary memory requirements.
	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(logicalDevice, vertexBuffer, &memRequirements);

	//Buffer memory allocation data.
	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	//Buffer memory allocation.
	if(vkAllocateMemory(logicalDevice, &allocInfo, nullptr, &vertexBufferMemory) != VK_SUCCESS)
		throw std::runtime_error("failed to allocate vertex buffer memory!");
	//Bind buffer memory.
	vkBindBufferMemory(logicalDevice, vertexBuffer, vertexBufferMemory, 0);

	//Copy vertex data to buffer.
	void* data;
	vkMapMemory(logicalDevice, vertexBufferMemory, 0, bufferInfo.size, 0, &data);
	memcpy(data, vertices.data(), (size_t)bufferInfo.size);
	vkUnmapMemory(logicalDevice, vertexBufferMemory);
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

		//Clear color.
		VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		//Commands execution.
		vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
		VkBuffer vertexBuffers[] ={vertexBuffer};
		VkDeviceSize offsets[] ={0};
		//Bind vertex buffer.
		vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);
		//Draw.
		vkCmdDraw(commandBuffers[i], static_cast<uint32_t>(vertices.size()), 1, 0, 0);
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
/// Creation of shader module. POR HACER --> REVISAR SI LLEVAR A VulkanHelper
/// 
/// </summary>
VkShaderModule VulkanRenderEngine::createShaderModule(const std::vector<char>& code){
	//Shader module creation data.
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
	//Shader module creation.
	VkShaderModule shaderModule;
	if(vkCreateShaderModule(logicalDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
		throw std::runtime_error("failed to create shader module!");

	return shaderModule;
}
#pragma endregion

#pragma region Query methods POR HACER --> AÑADIR PARAMS DE DOCUMENTACIÓN
/// <summary>
/// Checks if our device can be used to our application requeriments.
/// 
/// </summary>
bool VulkanRenderEngine::isDeviceSuitable(VkPhysicalDevice device){
	//Checking queue families
	QueueFamilyIndices indices = findQueueFamilies(device);
	//Checking extensions
	bool extensionsSupported = vulkanHelper.checkDeviceExtensionSupport(device);
	//Cheking swap chain	
	bool swapChainAdequate = false;
	if(extensionsSupported){
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}
	/*VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(device, &supportedFeatures);*/

	return indices.isComplete() && extensionsSupported && swapChainAdequate;// && supportedFeatures.samplerAnisotropy;
}
/// <summary>
/// Checks if our device support some indicated queue families.
/// 
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
/// Checks our device memory.
/// 
/// 
/// </summary>
uint32_t VulkanRenderEngine::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties){
	//Getting data.
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);
	//Check device memorie properties.
	for(uint32_t i = 0; i < memProperties.memoryTypeCount; i++){
		if((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
			return i;
	}
	throw std::runtime_error("failed to find suitable memory type!");
}
/// <summary>
/// Checks if our device support swap chain with surface formats & present modes.
/// 
/// 
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
#pragma endregion

#pragma region Vulkan configuration methods POR HACER --> Improve hardware rating and hardware selected. POR HACER --> AÑADIR PARAMS DE DOCUMENTACIÓN
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
	//vkQueueWaitIdle(presentQueue);

	//Synchronization operations.
	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
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
/// 
/// 
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
/// 
/// 
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
/// 
/// 
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
	//Buffers.
	vkDestroyBuffer(logicalDevice, vertexBuffer, nullptr);
	vkFreeMemory(logicalDevice, vertexBufferMemory, nullptr);
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
	vulkanHelper.cleanup(instance);
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