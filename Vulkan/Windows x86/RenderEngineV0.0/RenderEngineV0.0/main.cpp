#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <map>
#include <set>
#include <algorithm>
#include <fstream>
#include <array>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

const int WIDTH = 800;
const int HEIGHT = 600;
const int MAX_FRAMES_IN_FLIGHT = 2;

const std::vector<const char*> validationLayers ={"VK_LAYER_LUNARG_standard_validation"};
const std::vector<const char*> deviceExtensions ={ VK_KHR_SWAPCHAIN_EXTENSION_NAME };

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif

#pragma region Debug callbacks
VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT * pCreateInfo, const VkAllocationCallbacks * pAllocator, VkDebugReportCallbackEXT * pCallback){
	auto func = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");

	if(func != nullptr) 
		return func(instance, pCreateInfo, pAllocator, pCallback);
	else
		return VK_ERROR_EXTENSION_NOT_PRESENT;
}
void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks * pAllocator){
	auto func = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
	
	if(func != nullptr)
		func(instance, callback, pAllocator);
}
#pragma endregion

#pragma region Structs
struct QueueFamilyIndices {
	int graphicsFamily = -1;
	int presentFamily = -1;
	
	bool isComplete() {
		return graphicsFamily >= 0 && presentFamily >= 0;
	}
};
struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};
struct Vertex {
	glm::vec2 pos;
	glm::vec3 color;
	static VkVertexInputBindingDescription getBindingDescription(){
		VkVertexInputBindingDescription bindingDescription ={};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}
	static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions(){
		std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions ={};
		//Position
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);
		//Color
		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		return attributeDescriptions;
	}
};
struct UniformBufferObject {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};
#pragma endregion


class HelloTriangleApplication{
public:
	void run(){
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}
	//Callbacks
	static void framebufferResizeCallback(GLFWwindow* window, int width, int height){
		auto app = reinterpret_cast<HelloTriangleApplication*>(glfwGetWindowUserPointer(window));
		app->framebufferResized = true;
	}
	//Data members
	const std::vector<Vertex> vertices = {
		{{-0.5f, -0.5f},{1.0f, 0.0f, 0.0f}}, {{0.5f, -0.5f},{0.0f, 1.0f, 0.0f}}, {{0.5f, 0.5f},{0.0f, 0.0f, 1.0f}}, {{-0.5f, 0.5f},{1.0f, 1.0f, 1.0f}}
	};
	const std::vector<uint16_t> indices = {0, 1, 2, 2, 3, 0};
private:
	#pragma region Data members
	GLFWwindow * window;
	VkInstance instance;
	VkDebugReportCallbackEXT callback;
	VkSurfaceKHR surface;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device;
	//Queues
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	//Swap chain
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;
	//Render pass
	VkRenderPass renderPass;
	//Pipeline
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;
	//Descriptor set layout
	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;
	//Framebuffers
	std::vector<VkFramebuffer> swapChainFramebuffers;
	//Vertex buffer
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	//Index buffer
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;
	//Uniform buffers
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	//Textures
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	//Commands
	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;
	//Semaphores
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	//Fence
	std::vector<VkFence> inFlightFences;
	//Frame
	size_t currentFrame = 0;
	bool framebufferResized = false;
	#pragma endregion

	#pragma region Initialization & debug
	void initWindow(){
		glfwInit();
		//Avoid set OpenGL as default render API and the resizable option for the window. 
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); Window resize
		
		window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
	}
	void initVulkan(){
		createInstance();
		setupDebugCallback();
		createSurface();
		pickPhysicalDevice();
		createLogicalDevice();
		createSwapChain();
		createImageViews();
		createRenderPass();
		createDescriptorSetLayout();
		createGraphicsPipeline();
		createFramebuffers();
		createCommandPool();
		createTextureImage();
		createVertexBuffer();
		createIndexBuffer();
		createUniformBuffer();
		createDescriptorPool();
		createDescriptorSet();
		createCommandBuffers();
		createSyncObjects();
	}
	//Instance
	void createInstance(){
		VkApplicationInfo appInfo ={};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hello Triangle";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo ={};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		//GLFW extensions
		auto glfwExtensions = getRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(glfwExtensions.size());
		createInfo.ppEnabledExtensionNames = glfwExtensions.data();
		createInfo.enabledLayerCount = 0;

		//Vulkan extensions
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
		std::cout << "Available extensions:" << std::endl;
		for(const auto& extension : extensions)
			std::cout << "\t" << extension.extensionName << std::endl;

		//ValidationLayers
		if(enableValidationLayers && !checkValidationLayerSupport())
			throw std::runtime_error("validation layers requested, but not available!");
		if(enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else
			createInfo.enabledLayerCount = 0;

		if(vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
			throw std::runtime_error("failed to create instance!");
	}
	//Validation layer
	bool checkValidationLayerSupport() {
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		
		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for(const char * layerName:validationLayers){
			bool layerFound = false;
			
			for(const auto & layerProperties:availableLayers) {
				if(strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}
			if(!layerFound)
				return false;
		}

		return true;
	}
	//Extensions
	std::vector<const char*> getRequiredExtensions() {
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
		if(enableValidationLayers)
			extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
		
		return extensions;
	}
	//Debug Callback
	void setupDebugCallback() {
		VkDebugReportCallbackCreateInfoEXT createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
		createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
		createInfo.pfnCallback = debugCallback;
		if(!enableValidationLayers)
			return;
		if(CreateDebugReportCallbackEXT(instance, &createInfo, nullptr, &callback) != VK_SUCCESS)
			throw std::runtime_error("failed to set up debug callback!");
	}
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData) {
			std::cerr << "validation layer: " << msg << std::endl;
			
			return VK_FALSE;
	}
	#pragma endregion

	#pragma region Physical & logical device methods
	//Suitable
	bool isDeviceSuitable(VkPhysicalDevice device) {
		//Checking queue families
		QueueFamilyIndices indices = findQueueFamilies(device);
		//Checking extensions
		bool extensionsSupported = checkDeviceExtensionSupport(device);
		//Cheking swap chain
		bool swapChainAdequate = false;
		if(extensionsSupported) {
			SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}

		return indices.isComplete() && extensionsSupported && swapChainAdequate;
	}

	//Surface
	void createSurface(){
		if(glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
			throw std::runtime_error("failed to create window surface!");
	}
	
	//Physical devices
	void pickPhysicalDevice(){
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
		if(deviceCount == 0)
			throw std::runtime_error("failed to find GPUs with Vulkan support!");

		//Listing & checking physical devices
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
		for(const auto & device : devices){
			if(isDeviceSuitable(device)){
				physicalDevice = device;
				break;
			}
		}

		if(physicalDevice == VK_NULL_HANDLE)
			throw std::runtime_error("failed to find a suitable GPU!");

		/*Optional: scoring the graphyc cards*/
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
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device){
		QueueFamilyIndices indices;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for(const auto & queueFamily:queueFamilies){
			//Graphycs
			if(queueFamily.queueCount > 0 && queueFamily.queueFlags &VK_QUEUE_GRAPHICS_BIT)
				indices.graphicsFamily = i;
			//Presentation
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
	
	//Logical device
	void createLogicalDevice(){
		VkPhysicalDeviceFeatures deviceFeatures = {};

		QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<int> uniqueQueueFamilies = {indices.graphicsFamily, indices.presentFamily};

		float queuePriority = 1.0f;
		for(int queueFamily:uniqueQueueFamilies){
			VkDeviceQueueCreateInfo queueCreateInfo ={};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}
		//Creation info
		VkDeviceCreateInfo createInfo = {};
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pEnabledFeatures = &deviceFeatures;

		if(enableValidationLayers){
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else
			createInfo.enabledLayerCount = 0;

		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();
		//Creation
		if(vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS)
			throw std::runtime_error("failed to create logical device!");

		//Getting queues
		vkGetDeviceQueue(device, indices.graphicsFamily, 0, &graphicsQueue);
		vkGetDeviceQueue(device, indices.presentFamily, 0, &presentQueue);
	}
	bool checkDeviceExtensionSupport(VkPhysicalDevice device){
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
		for(const auto & extension:availableExtensions)
			requiredExtensions.erase(extension.extensionName);

		return requiredExtensions.empty();
	}
	#pragma endregion

	#pragma region Swap chain & image views
	//Swap chain
	void createSwapChain(){
		//Checking
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);
		//Choosing right settings of swap chain
		VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
		VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
		VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);
		//Optimizing for tripple buffering
		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
		if(swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
			imageCount = swapChainSupport.capabilities.maxImageCount;

		//Creation info
		VkSwapchainCreateInfoKHR createInfo ={};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = surface;
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
		uint32_t queueFamilyIndices[] ={(uint32_t) indices.graphicsFamily, (uint32_t)indices.presentFamily};
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

		//Creation
		if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS)
			throw std::runtime_error("failed to create swap chain!");

		//Swap chain images
		vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
		swapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());
		swapChainImageFormat = surfaceFormat.format;
		swapChainExtent = extent;
	}
	void recreateSwapChain(){
		int width = 0, height = 0;
		while(width == 0 || height == 0){
			glfwGetFramebufferSize(window, &width, &height);
			glfwWaitEvents();
		}
		vkDeviceWaitIdle(device);
		cleanupSwapChain();

		createSwapChain();
		createImageViews();
		createRenderPass();
		createGraphicsPipeline();
		createFramebuffers();
		createCommandBuffers();
	}
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device){
		SwapChainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);
		//Surface formats
		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
		if(formatCount != 0){
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
		}
		//Presentation modes
		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
		if(presentModeCount != 0){
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
		}

		return details;
	}
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats){
		if(availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED)
			return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};

		for(const auto & availableFormat:availableFormats){
			if(availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
				return availableFormat;
		}
		
		return availableFormats[0];
	}
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes){
		VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;
		
		for(const auto & availablePresentMode:availablePresentModes) {
			if(availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
				return availablePresentMode;
			else if(availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
				bestMode = availablePresentMode;
		}

		return bestMode;
	}
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR & capabilities) {
		if(capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
			return capabilities.currentExtent;
		else{
			int width, height;
			glfwGetFramebufferSize(window, &width, &height);
			VkExtent2D actualExtent ={static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

			actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
			actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
			
			return actualExtent;
		}
	}
	//Swap chain image views
	void createImageViews(){
		swapChainImageViews.resize(swapChainImages.size());

		for(size_t i=0; i<swapChainImages.size(); i++){
			//Creation info
			VkImageViewCreateInfo createInfo ={};
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
			//Creation
			if(vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS)
				throw std::runtime_error("failed to create image views!");
		}
	}
	#pragma endregion

	#pragma region Pipeline
	//Pipeline
	void createGraphicsPipeline(){
		auto vertShaderCode = readFile("Shaders/vert.spv");
		auto fragShaderCode = readFile("Shaders/frag.spv");

		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;
		vertShaderModule = createShaderModule(vertShaderCode);
		fragShaderModule = createShaderModule(fragShaderCode);

		//Vertex shader
		VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
		vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertShaderStageInfo.module = vertShaderModule;
		vertShaderStageInfo.pName = "main";
		//Fragment shader
		VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
		fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragShaderStageInfo.module = fragShaderModule;
		fragShaderStageInfo.pName = "main";

		//Pipeline elements
		VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

		//Vertex input
		VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		auto bindingDescription = Vertex::getBindingDescription();
		auto attributeDescriptions = Vertex::getAttributeDescriptions();
		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
		//Input assembly
		VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;
		//Viewport
		VkViewport viewport = {};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)swapChainExtent.width;
		viewport.height = (float)swapChainExtent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		//Scissor
		VkRect2D scissor ={};
		scissor.offset ={0, 0};
		scissor.extent = swapChainExtent;
		VkPipelineViewportStateCreateInfo viewportState = {};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;
		//Rasterizer
		VkPipelineRasterizationStateCreateInfo rasterizer = {};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		//rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;
		rasterizer.depthBiasConstantFactor = 0.0f; // Optional
		rasterizer.depthBiasClamp = 0.0f; // Optional
		rasterizer.depthBiasSlopeFactor = 0.0f; // Optional
		//Multisampling
		VkPipelineMultisampleStateCreateInfo multisampling = {};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampling.minSampleShading = 1.0f; // Optional
		multisampling.pSampleMask = nullptr; // Optional
		multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
		multisampling.alphaToOneEnable = VK_FALSE; // Optional
		//Blending
		VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE;
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional
		colorBlendAttachment.blendEnable = VK_TRUE;
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
		VkPipelineColorBlendStateCreateInfo colorBlending ={};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f; // Optional
		colorBlending.blendConstants[1] = 0.0f; // Optional
		colorBlending.blendConstants[2] = 0.0f; // Optional
		colorBlending.blendConstants[3] = 0.0f; // Optional
		//Dynamic state
		VkDynamicState dynamicStates[] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_LINE_WIDTH};
		VkPipelineDynamicStateCreateInfo dynamicState ={};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = 2;
		dynamicState.pDynamicStates = dynamicStates;

		//Pipeline layout
		VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 1;
		pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
		pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
		pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional
		if(vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
			throw std::runtime_error("failed to create pipelinelayout!");

		//Creation info
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
		if(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS)
			throw std::runtime_error("failed to create graphics pipeline!");

		//Free
		vkDestroyShaderModule(device, fragShaderModule, nullptr);
		vkDestroyShaderModule(device, vertShaderModule, nullptr);
	}
	static std::vector<char> readFile(const std::string& filename){
		//Reading file
		std::ifstream file(filename, std::ios::ate | std::ios::binary);
		if(!file.is_open())
			throw std::runtime_error("failed to open file!");
		//Creating buffer
		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);
		file.seekg(0);
		file.read(buffer.data(), fileSize);
		//Closing file
		file.close();
		
		return buffer;
	}
	VkShaderModule createShaderModule(const std::vector<char>& code){
		//Creation info
		VkShaderModuleCreateInfo createInfo ={};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
		//Creation
		VkShaderModule shaderModule;
		if(vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
			throw std::runtime_error("failed to create shader module!");

		return shaderModule;
	}
	#pragma endregion

	#pragma region Descriptor layouts & uniform buffers
	//Descriptor set layout
	void createDescriptorSetLayout(){
		//UBO info
		VkDescriptorSetLayoutBinding uboLayoutBinding ={};
		uboLayoutBinding.binding = 0;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.descriptorCount = 1;
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
													   //Creation info
		VkDescriptorSetLayoutCreateInfo layoutInfo ={};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = 1;
		layoutInfo.pBindings = &uboLayoutBinding;
		//Creation
		if(vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
			throw std::runtime_error("failed to create descriptor set layout!");
	}
	void createDescriptorPool(){
		//Creation info
		VkDescriptorPoolSize poolSize = {};
		poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSize.descriptorCount = static_cast<uint32_t>(swapChainImages.size());
		VkDescriptorPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = 1;
		poolInfo.pPoolSizes = &poolSize;
		poolInfo.maxSets = static_cast<uint32_t>(swapChainImages.size());
		//Creation
		if(vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
			throw std::runtime_error("failed to create descriptor pool!");
	}
	void createDescriptorSet(){
		//Allocation info
		std::vector<VkDescriptorSetLayout> layouts(swapChainImages.size(), descriptorSetLayout);
		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = descriptorPool;
		allocInfo.descriptorSetCount = static_cast<uint32_t>(swapChainImages.size());
		allocInfo.pSetLayouts = layouts.data();
		descriptorSets.resize(swapChainImages.size());
		//Allocation
		if(vkAllocateDescriptorSets(device, &allocInfo, &descriptorSets[0]) != VK_SUCCESS)
			throw std::runtime_error("failed to allocate descriptor sets!");
		//Descriptor configurations
		for(size_t i=0; i < swapChainImages.size(); i++){
			VkDescriptorBufferInfo bufferInfo ={};
			bufferInfo.buffer = uniformBuffers[i];
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(UniformBufferObject);
			VkWriteDescriptorSet descriptorWrite ={};
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = descriptorSets[i];
			descriptorWrite.dstBinding = 0;
			descriptorWrite.dstArrayElement = 0;
			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrite.descriptorCount = 1;
			descriptorWrite.pBufferInfo = &bufferInfo;
			descriptorWrite.pImageInfo = nullptr; // Optional
			descriptorWrite.pTexelBufferView = nullptr; // Optional
			//Updating data
			vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
		}
	}
	//Uniform buffers
	void createUniformBuffer(){
		VkDeviceSize bufferSize = sizeof(UniformBufferObject);
		
		uniformBuffers.resize(swapChainImages.size());
		uniformBuffersMemory.resize(swapChainImages.size());

		for(size_t i = 0; i < swapChainImages.size(); i++)
			createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
	}
	void updateUniformBuffer(uint32_t currentImage){
		//Timming
		static auto startTime = std::chrono::high_resolution_clock::now();
		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
		//Ubo
		UniformBufferObject ubo ={};
		ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ubo.proj = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f);
		ubo.proj[1][1] *= -1;
		//Filling data
		void* data;
		vkMapMemory(device, uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, uniformBuffersMemory[currentImage]);
	}
	#pragma endregion

	#pragma region Render elements & buffers
	//Render pass
	void createRenderPass(){
		VkAttachmentDescription colorAttachment ={};
		colorAttachment.format = swapChainImageFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		//Subpasses
		VkAttachmentReference colorAttachmentRef ={};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		VkSubpassDescription subpass ={};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		//Creation info
		VkRenderPassCreateInfo renderPassInfo ={};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		//Dependency subpass
		VkSubpassDependency dependency ={};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;
		//Creation
		if(vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS)
			throw std::runtime_error("failed to create render pass!");
	}
	//Framebuffers
	void createFramebuffers(){
		swapChainFramebuffers.resize(swapChainImageViews.size());

		for(size_t i=0; i<swapChainImageViews.size(); i++){
			VkImageView attachments[] = {swapChainImageViews[i]};
			VkFramebufferCreateInfo framebufferInfo ={};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = renderPass;
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = attachments;
			framebufferInfo.width = swapChainExtent.width;
			framebufferInfo.height = swapChainExtent.height;
			framebufferInfo.layers = 1;
			if(vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS)
				throw std::runtime_error("failed to create framebuffer!");
		}
	}
	//Data buffers
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory){
		//Creation info
		VkBufferCreateInfo bufferInfo ={};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		//Creation
		if(vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
			throw std::runtime_error("failed to create buffer!");
		//Memory allocation info
		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(device, buffer, &memRequirements);
		VkMemoryAllocateInfo allocInfo ={};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);
		//Memory allocation
		if(vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
			throw std::runtime_error("failed to allocate buffer memory!");
		vkBindBufferMemory(device, buffer, bufferMemory, 0);
	}
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
		//Begin command buffer
		VkCommandBuffer commandBuffer = beginSingleTimeCommands();
		//Preparing copy
		VkBufferCopy copyRegion ={};
		copyRegion.size = size;
		//Copy command
		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
		//End command buffer
		endSingleTimeCommands(commandBuffer);
	}
	VkCommandBuffer beginSingleTimeCommands(){
		//Allocation info
		VkCommandBufferAllocateInfo allocInfo ={};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = commandPool;
		allocInfo.commandBufferCount = 1;
		//Allocation
		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);
		//Command buffer info
		VkCommandBufferBeginInfo beginInfo ={};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		return commandBuffer;
	}
	void endSingleTimeCommands(VkCommandBuffer commandBuffer) {
		vkEndCommandBuffer(commandBuffer);
		//Executing transfer command
		VkSubmitInfo submitInfo ={};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;
		vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(graphicsQueue);
		vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
	}
	void createVertexBuffer(){
		VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
		//Staging buffer
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
		//Filling data
		void* data;
		vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, vertices.data(), (size_t)bufferSize);
		vkUnmapMemory(device, stagingBufferMemory);
		//Final vertex buffer
		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);
		//Copy
		copyBuffer(stagingBuffer, vertexBuffer, bufferSize);
		//Free buffer
		vkDestroyBuffer(device, stagingBuffer, nullptr);
		vkFreeMemory(device, stagingBufferMemory, nullptr);

	}
	void createIndexBuffer(){
		VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();
		//Staging buffer
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
		//Filling data
		void* data;
		vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, indices.data(), (size_t)bufferSize);
		vkUnmapMemory(device, stagingBufferMemory);
		//Final index buffer
		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);
		//Copy
		copyBuffer(stagingBuffer, indexBuffer, bufferSize);
		//Free buffer
		vkDestroyBuffer(device, stagingBuffer, nullptr);
		vkFreeMemory(device, stagingBufferMemory, nullptr);
	}
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties){
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);
		for(uint32_t i=0; i < memProperties.memoryTypeCount; i++){
			if((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags &	properties) == properties)
				return i;
		}
		throw std::runtime_error("failed to find suitable memory type!");
	}
	//Command buffers
	void createCommandPool(){
		//Creation info
		QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice);
		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
		poolInfo.flags = 0; // Optional
		//Creation
		if(vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
			throw std::runtime_error("failed to create command pool!");
	}
	void createCommandBuffers(){
		//Allocation info
		commandBuffers.resize(swapChainFramebuffers.size());
		VkCommandBufferAllocateInfo allocInfo ={};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();
		//Allocation
		if(vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS)
			throw std::runtime_error("failed to allocate command buffers!");
		for(size_t i=0; i<commandBuffers.size(); i++){
			//Command buffer info
			VkCommandBufferBeginInfo beginInfo ={};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
			beginInfo.pInheritanceInfo = nullptr; // Optional
			if(vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
				throw std::runtime_error("failed to begin recording command buffer!");
			VkRenderPassBeginInfo renderPassInfo ={};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = renderPass;
			renderPassInfo.framebuffer = swapChainFramebuffers[i];
			renderPassInfo.renderArea.offset ={0, 0};
			renderPassInfo.renderArea.extent = swapChainExtent;
			VkClearValue clearColor ={0.0f, 0.0f, 0.0f, 1.0f};
			renderPassInfo.clearValueCount = 1;
			renderPassInfo.pClearValues = &clearColor;
			//Command buffer configuration
			vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
			vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
			//Vertex buffer
			VkBuffer vertexBuffers[] = {vertexBuffer};
			VkDeviceSize offsets[] = {0};
			vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);
			vkCmdBindIndexBuffer(commandBuffers[i], indexBuffer, 0, VK_INDEX_TYPE_UINT16);
			//Uniform buffers
			vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[i], 0, nullptr);
			//Draw
			vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
			vkCmdEndRenderPass(commandBuffers[i]);
			if(vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
				throw std::runtime_error("failed to record command buffer!");
		}

	}
	#pragma endregion

	#pragma region Images
	//Images
	void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) {
		//Creation image info
		VkImageCreateInfo imageInfo ={};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = width;
		imageInfo.extent.height = height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = format;
		imageInfo.tiling = tiling;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = usage;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		//Creation image
		if(vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS)
			throw std::runtime_error("failed to create image!");
		//Allocation info
		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(device, image, &memRequirements);
		VkMemoryAllocateInfo allocInfo ={};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);
		//Allocation
		if(vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
			throw std::runtime_error("failed to allocate image memory!");
		vkBindImageMemory(device, image, imageMemory, 0);
	}
	void createTextureImage(){
		//Loading file
		int texWidth, texHeight, texChannels;
		stbi_uc* pixels = stbi_load("textures/texture.jpg", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		VkDeviceSize imageSize = texWidth * texHeight * 4;
		if(!pixels)
			throw std::runtime_error("failed to load texture image!");
		//Buffer creation
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
		//Pixel copies
		void * data;
		vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
		memcpy(data, pixels, static_cast<size_t>(imageSize));
		vkUnmapMemory(device, stagingBufferMemory);
		//Free original array
		stbi_image_free(pixels);
		//Creation image
		createImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory);
		//Transition image
		transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		//Copy buffer to image
		copyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
		//Shader acces
		transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		//Free staging texture memory
		vkDestroyBuffer(device, stagingBuffer, nullptr);
		vkFreeMemory(device, stagingBufferMemory, nullptr);
	}
	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout){
		//Begin command buffer
		VkCommandBuffer commandBuffer = beginSingleTimeCommands();
		//Creation image memory barrier info
		VkImageMemoryBarrier barrier = {};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;
		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;
		if(oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL){
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if(oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL){
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else
			throw std::invalid_argument("unsupported layout transition!");
		//Barrier command
		vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
		//End command buffer
		endSingleTimeCommands(commandBuffer);
	}
	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height){
		//Begin command buffer
		VkCommandBuffer commandBuffer = beginSingleTimeCommands();
		//Copy region info
		VkBufferImageCopy region = {};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;
		region.imageOffset = {0, 0, 0};
		region.imageExtent = {width, height, 1};
		//Copy command
		vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
		//End command buffer
		endSingleTimeCommands(commandBuffer);
	}
	#pragma endregion

	#pragma region Mainloop
	//Main loop
	void mainLoop(){
		while(!glfwWindowShouldClose(window)) {
			glfwPollEvents();
			drawFrame();
		}

		vkDeviceWaitIdle(device);
	}
	//Drawframe
	void drawFrame(){
		//Fences
		vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());
		//Acquire image of swap chain
		uint32_t imageIndex;

		VkResult result = vkAcquireNextImageKHR(device, swapChain, std::numeric_limits<uint64_t>::max(), imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);
		if(result == VK_ERROR_OUT_OF_DATE_KHR){
			recreateSwapChain();
			return;
		}
		else if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			throw std::runtime_error("failed to acquire swap chain image!");

		updateUniformBuffer(imageIndex);

		VkSubmitInfo submitInfo ={};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		//Semaphores
		VkSemaphore waitSemaphores[] ={imageAvailableSemaphores[currentFrame]};
		VkPipelineStageFlags waitStages[] ={VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffers[imageIndex];
		VkSemaphore signalSemaphores[] ={renderFinishedSemaphores[currentFrame]};
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;
		//Fences
		vkResetFences(device, 1, &inFlightFences[currentFrame]);
		//Submit
		if(vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS)
			throw std::runtime_error("failed to submit draw command buffer!");

		//Presentation image
		VkPresentInfoKHR presentInfo ={};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;
		VkSwapchainKHR swapChains[] ={swapChain};
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr; // Optional

		result = vkQueuePresentKHR(presentQueue, &presentInfo);
		if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized){
			framebufferResized = false;
			recreateSwapChain();
		}
		else if(result != VK_SUCCESS)
			throw std::runtime_error("failed to present swap chain image!");

		currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
		//Rough sincronization method
		//vkQueueWaitIdle(presentQueue);
	}
	#pragma endregion

	#pragma region Cleanup methods
	void cleanup(){
		cleanupSwapChain();
		//Texture image
		vkDestroyImage(device, textureImage, nullptr);
		//Descriptor set layout & descriptor pool
		vkDestroyDescriptorPool(device, descriptorPool, nullptr);
		vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
		//Uniform buffers
		for(size_t i = 0; i < swapChainImages.size(); i++){
			vkDestroyBuffer(device, uniformBuffers[i], nullptr);
			vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
		}
		//Index buffer
		vkDestroyBuffer(device, indexBuffer, nullptr);
		vkFreeMemory(device, indexBufferMemory, nullptr);
		//Vertex buffer
		vkDestroyBuffer(device, vertexBuffer, nullptr);
		vkFreeMemory(device, vertexBufferMemory, nullptr);
		//Semaphores
		for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
			vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
			vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
			vkDestroyFence(device, inFlightFences[i], nullptr);
		}
		//Command pool
		vkDestroyCommandPool(device, commandPool, nullptr);
		//Logical device
		vkDestroyDevice(device, nullptr);
		//Validation layers
		if(enableValidationLayers)
			DestroyDebugReportCallbackEXT(instance, callback, nullptr);
		//Surface
		vkDestroySurfaceKHR(instance, surface, nullptr);
		//Instance
		vkDestroyInstance(instance, nullptr);
		//Glfw Window
		glfwDestroyWindow(window);
		glfwTerminate();
	}
	void cleanupSwapChain(){
		//Framebuffers
		for(size_t i = 0; i < swapChainFramebuffers.size(); i++)
			vkDestroyFramebuffer(device, swapChainFramebuffers[i], nullptr);
		//Commands
		vkFreeCommandBuffers(device, commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
		//Pipeline
		vkDestroyPipeline(device, graphicsPipeline, nullptr);
		vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
		//Render pass
		vkDestroyRenderPass(device, renderPass, nullptr);
		//Image views
		for(size_t i = 0; i < swapChainImageViews.size(); i++)
			vkDestroyImageView(device, swapChainImageViews[i], nullptr);
		//Swap chain
		vkDestroySwapchainKHR(device, swapChain, nullptr);
	}
	#pragma endregion

	//Semaphores & fences
	void createSyncObjects(){
		//Initialization
		imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
		//Creation info
		VkSemaphoreCreateInfo semaphoreInfo ={};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		VkFenceCreateInfo fenceInfo ={};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
		//Creation
		for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
			if(vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS || vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS || vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS)
				throw std::runtime_error("failed to create synchronization objects for a frame!");
		}
	}
};

int main(){
	HelloTriangleApplication app;
	
	try{
		app.run();
	}
	catch(const std::exception& e){
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	system("PAUSE");
	return EXIT_SUCCESS;
}