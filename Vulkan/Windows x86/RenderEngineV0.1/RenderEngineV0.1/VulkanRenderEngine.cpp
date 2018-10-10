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
/// Method to launch the graphyc application.
/// </summary>
void VulkanRenderEngine::run(){
	viewport.initWindow();
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
}
/// <summary>
/// Main loop of our application.
/// </summary>
void VulkanRenderEngine::mainLoop(){
	while(!glfwWindowShouldClose(viewport.window)) {
		glfwPollEvents();
	}
}
#pragma endregion

#pragma region Creation methods
/// <summary>
/// Creation of VulkanInstance.
/// </summary>
void VulkanRenderEngine::createVulkanInstance(){
	//Application info data.
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello VR";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	//Instance creation info data.
	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	//GLFW extensions. POR HACER --> METODO GETEXTENSIONS EN VULKANHELPER
	auto extensions = vulkanHelper.getRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();
	createInfo.enabledLayerCount = 0;
	//Print extensions.
	vulkanHelper.printExtensions();

	//ValidationLayers. POR HACER --> METODO GETVALIDATIONLAYERS EN VULKANHELPER
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
#pragma endregion

#pragma region Cleanup methods
/// <summary>
/// Cleanup of Vulkan elements.
/// </summary>
void VulkanRenderEngine::cleanup(){
	//VulkanHelper elements: Validation layers
	vulkanHelper.cleanup(instance);
	//Vulkan instance
	vkDestroyInstance(instance, nullptr);
	//Glfw Window
	viewport.cleanup();
}
#pragma endregion