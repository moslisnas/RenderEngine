/**
C++ VulkanRenderEngine.h
Purpose: Header of VulkanRenderEngine class

@author Alejandro Molina Quesada
@version 1.0
@date 24/09/2018
*/

#ifndef IOS_AND_EXCEPT
	#define IOS_AND_EXCEPT
	#include <iostream>
	#include <stdexcept>
#endif
#ifndef STRUCTURE_DATAS
	#define STRUCTURE_DATAS
	#include <set>
	#include <array>
#endif
#ifndef ALGEBRAIC_METHODS
	#define ALGEBRAIC_METHODS
	#include <algorithm>
	#include <glm/glm.hpp>
	#include <glm/gtc/matrix_transform.hpp>
#endif
#ifndef ANIMATIONS
	#define ANIMATIONS
	#include <chrono>
#endif
#ifndef GLM_FORCE_DEPTH_ZERO_TO_ONE
	#define GLM_FORCE_RADIANS
	#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#endif
#include "VulkanHelper.h"
#include "Viewport.h"
#include "Auxiliar.h"

#pragma region Structs POR HACER --> LLEVAR A CLASE MODEL EL STRUCT VERTEX Y VER SI LLEVAR LOS UBOS A VULKANHELPER CLASS
struct UniformBufferObject {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};
struct Vertex {
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;

	static VkVertexInputBindingDescription getBindingDescription(){
		VkVertexInputBindingDescription bindingDescription = {};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions(){
		std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = {};
		//Coordinates.
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);
		//Colors.
		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);
		//Texture coordinates.
		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

		return attributeDescriptions;
	}
};
#pragma endregion

class VulkanRenderEngine{
private:
	#pragma region Data members
	VulkanHelper vulkanHelper;
	VkInstance instance;
	//Device elements.
	VkPhysicalDevice physicalDevice;
	VkDevice logicalDevice;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	//Window elements.
	Viewport viewport;
	VkSurfaceKHR surface;
	//Swap chain elements.
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFramebuffers;
	//Pipeline elements.
	VkPipeline graphicsPipeline;
	VkPipelineLayout pipelineLayout;
	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;
	VkRenderPass renderPass;
	//Command elements.
	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;
	//Synchronization elements.
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	size_t currentFrame = 0;
	//Buffers.
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	//Textures.
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	VkImageView textureImageView;
	VkSampler textureSampler;
	//Depth.
	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;
	//Others.
	bool framebufferResized = false;
	uint32_t mipLevels;
	VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
	VkImage colorImage;
	VkDeviceMemory colorImageMemory;
	VkImageView colorImageView;
	#pragma endregion
public:
	#pragma region Data members
	//Vertex data. POR HACER --> LLEVAR A ARRAY DE MODELS DE CLASE ESCENA.
	const std::vector<Vertex> vertices = {
		{{-0.5f, -0.5f, 0.0f},{1.0f, 0.0f, 0.0f},{0.0f, 0.0f}},
		{{0.5f, -0.5f, 0.0f},{0.0f, 1.0f, 0.0f},{1.0f, 0.0f}},
		{{0.5f, 0.5f, 0.0f},{0.0f, 0.0f, 1.0f},{1.0f, 1.0f}},
		{{-0.5f, 0.5f, 0.0f},{1.0f, 1.0f, 1.0f},{0.0f, 1.0f}},
		
		{{-0.5f, -0.5f, -0.5f},{1.0f, 0.0f, 0.0f},{0.0f, 0.0f}},
		{{0.5f, -0.5f, -0.5f},{0.0f, 1.0f, 0.0f},{1.0f, 0.0f}},
		{{0.5f, 0.5f, -0.5f},{0.0f, 0.0f, 1.0f},{1.0f, 1.0f}},
		{{-0.5f, 0.5f, -0.5f},{1.0f, 1.0f, 1.0f},{0.0f, 1.0f}}
	};
	//Index data. POR HACER --> LLEVAR A ARRAY DE MODELS DE CLASE ESCENA.
	const std::vector<uint16_t> indices = {
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4
	};
	#pragma endregion

	#pragma region Contructor & destructor
	/// <summary>
	/// Constructor of <c>VulkanRenderEngine</c> class.
	/// </summary>
	VulkanRenderEngine();
	/// <summary>
	/// Destructor of <c>VulkanRenderEngine</c> class.
	/// </summary>
	~VulkanRenderEngine();
	#pragma endregion

	#pragma region Main methods
	/// <summary>
	/// Method to launch the graphycs application.
	/// </summary>
	void run();
	/// <summary>
	/// Initialization of Vulkan elements.
	/// </summary>
	void initVulkan();
	/// <summary>
	/// Main loop of our application.
	/// </summary>
	void mainLoop();
	#pragma endregion

	#pragma region Callbacks
	static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
		auto app = reinterpret_cast<VulkanRenderEngine*>(glfwGetWindowUserPointer(window));
		app->framebufferResized = true;
	}
	#pragma endregion

	#pragma region Creation methods
	/// <summary>
	/// Creation of VulkanInstance.
	/// </summary>
	void createVulkanInstance();
	/// <summary>
	/// Creation of logical device.
	/// </summary>
	void createLogicalDevice();
	/// <summary>
	/// Creation of swap chain.
	/// </summary>
	void createSwapChain();
	/// <summary>
	/// Recreation of swap chain.
	/// </summary>
	void recreateSwapChain();
	/// <summary>
	/// Creation of image views.
	/// </summary>
	void createImageViews();
	/// <summary>
	/// Creation of descriptor set layout (UBO).
	/// </summary>
	void createDescriptorSetLayout();
	/// <summary>
	/// Creation of descriptor pool.
	/// </summary>
	void createDescriptorPool();
	/// <summary>
	/// Creation of descriptor sets.
	/// </summary>
	void createDescriptorSets();
	/// <summary>
	/// Creation of graphics pipeline.
	/// </summary>
	void createGraphicsPipeline();
	/// <summary>
	/// Creation of render pass.
	/// </summary>
	void createRenderPass();
	/// <summary>
	/// Creation of framebuffers.
	/// </summary>
	void createFramebuffers();
	/// <summary>
	/// Creation of vertex buffer.
	/// </summary>
	void createVertexBuffer();
	/// <summary>
	/// Creation of index buffer.
	/// </summary>
	void createIndexBuffer();
	/// <summary>
	/// Creation of uniform buffer.
	/// </summary>
	void createUniformBuffer();
	/// <summary>
	/// Creation of texture image.
	/// </summary>
	void createTextureImage();
	/// <summary>
	/// Mipmap generation for texture images.
	/// <param name="image">The image from we want generate mipmap.</param>
	/// <param name="imageFormat">The format of the image.</param>
	/// <param name="texWidth">The texture image width.</param>
	/// <param name="texHeight">The  texture image height.</param>
	/// <param name="mipLevels">The number of levels used for mipmaps.</param>
	/// </summary>
	void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
	/// <summary>
	/// Creation of texture image view.
	/// </summary>
	void createTextureImageView();
	/// <summary>
	/// Creation of texture sampler.
	/// </summary>
	void createTextureSampler();
	/// <summary>
	/// Creation of depth resources.
	/// </summary>
	void createDepthResources();
	/// <summary>
	/// Creation of command pool.
	/// </summary>
	void createCommandPool();
	/// <summary>
	/// Creation of command buffers.
	/// </summary>
	void createCommandBuffers();
	/// <summary>
	/// Creation of synchronization elements.
	/// </summary>
	void createSyncObjects();
	/// <summary>
	/// Creation of multisampling color elements.
	/// </summary>
	void createColorResources();
	#pragma endregion

	#pragma region Query methods
	/// <summary>
	/// Checks if our device can be used to our application requeriments.
	/// <param name="device">Physical device we want to check.</param>
	/// <returns>True if the device meet some requirements: graphycs and presentation queue families, extensions, swap chain support and anisotropy feature; false otherwise.</returns> 
	/// </summary>
	bool isDeviceSuitable(VkPhysicalDevice device);
	/// <summary>
	/// Checks if our device support some indicated queue families and get their indices.
	/// <param name="device">Physical device we want to check.</param>
	/// <returns>The indices of the queue families.</returns> 
	/// </summary>
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	/// <summary>
	/// Checks if our device support swap chain with surface formats and present modes, furthermore return swap chain support details.
	/// <param name="device">Physical device we want to check.</param>
	/// <returns>The swap chain support details.</returns> 
	/// </summary>
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	/// <summary>
	/// Checks if the format is available.
	/// <param name="candidates">The formats that we accept to use.</param>
	/// <param name="tiling">The way we dispose the image texel data.</param>
	/// <param name="features">Flags for the format features we want to get supporting.</param>
	/// <returns>The format itself.</returns>
	/// </summary>
	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	/// <summary>
	/// Checks if depth format is available and returns it.
	/// <returns>The depth format itself.</returns>
	/// </summary>
	VkFormat findDepthFormat();
	/// <summary>
	/// Checks if format has the stencil component.
	/// <param name="format">The format itself.</param>
	/// <returns>True if the format has stencil component, false otherwise.</returns> 
	/// </summary>
	bool hasStencilComponent(VkFormat format);
	/// <summary>
	/// Get the max number of multisampling samples supported.
	/// <returns>The max multisampling samples supported.</returns> 
	/// </summary>
	VkSampleCountFlagBits getMaxUsableSampleCount();
	#pragma endregion

	#pragma region Vulkan configuration methods
	/// <summary>
	/// Drawing method.
	/// </summary>
	void drawFrame();
	/// <summary>
	/// Update the pipeline uniform buffer with and image.
	/// <param name="currentImage">The image we want to include on the updating.</param>
	/// </summary>
	void updateUniformBuffer(uint32_t currentImage);
	/// <summary>
	/// Decide the hardware selected to use on render.
	/// </summary>
	void pickPhyshicalDevice();
	/// <summary>
	/// Chose swap chain surface format.
	/// <param name="availableFormats">The formats that we can use.</param>
	/// <returns>The surface format selected.</returns> 
	/// </summary>
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	/// <summary>
	/// Chose swap chain present mode.
	/// <param name="availablePresentModes">The present modes that we can use.</param>
	/// <returns>The present mode selected.</returns> 
	/// </summary>
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
	/// <summary>
	/// Chose swap chain extent.
	/// <param name="capabilities">The surface capabilities available.</param>
	/// <returns>The swap extent (2D) selected.</returns> 
	/// </summary>
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	/// <summary>
	/// Put image on a layout.
	/// <param name="image">The original image to transit.</param>
	/// <param name="format">The format for the image.</param>
	/// <param name="oldLayout">The old layout used.</param>
	/// <param name="newLayout">The new layout to use.</param>
	/// <param name="mipLevels">The mipmap levels used.</param>
	/// </summary>
	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
	#pragma endregion

	#pragma region Cleanup methods
	/// <summary>
	/// Cleanup of Vulkan elements.
	/// </summary>
	void cleanup();
	/// <summary>
	/// Cleanup of Vulkan swap chain.
	/// </summary>
	void cleanupSwapChain();
	#pragma endregion
};