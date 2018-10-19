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
	#include <glm/glm.hpp>
	#include <glm/gtc/matrix_transform.hpp>
#endif
#ifndef ANIMATIONS
	#define ANIMATIONS
	#include <chrono>
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
	glm::vec2 pos;
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
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
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
	//Others.
	bool framebufferResized = false;
	#pragma endregion
public:
	#pragma region Data members
	//Vertex data. POR HACER --> LLEVAR A ARRAY DE MODELS DE CLASE ESCENA.
	const std::vector<Vertex> vertices = {
		{{-0.5f, -0.5f},{1.0f, 0.0f, 0.0f},{1.0f, 0.0f}},
		{{0.5f, -0.5f},{0.0f, 1.0f, 0.0f},{0.0f, 0.0f}},
		{{0.5f, 0.5f},{0.0f, 0.0f, 1.0f},{0.0f, 1.0f}},
		{{-0.5f, 0.5f},{1.0f, 1.0f, 1.0f},{1.0f, 1.0f}}
	};
	//Index data. POR HACER --> LLEVAR A ARRAY DE MODELS DE CLASE ESCENA.
	const std::vector<uint16_t> indices = {
		0, 1, 2, 2, 3, 0
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

	#pragma region Main methods POR HACER --> INTENTAR LLEVAR initwintdow A VIEWPORT CLASS
	/// <summary>
	/// Method to launch the graphyc application.
	/// </summary>
	void run();
	/// <summary>
	/// Initialization of Vulkan elements.
	/// </summary>
	void initVulkan();
	/// <summary>
	/// Initalize window elements.
	/// </summary>
	void initWindow();
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
	/// Creation of surface.
	/// </summary>
	void createSurface();
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
	/// Creation of texture image. POR HACER --> VER SI LLEVAR A AUXILIAR.H
	/// </summary>
	void createTextureImage();
	/// <summary>
	/// Creation of texture image view. POR HACER --> VER SI LLEVAR A AUXILIAR.H
	/// </summary>
	void createTextureImageView();
	/// <summary>
	/// Creation of image view. POR HACER --> VER SI LLEVAR A AUXILIAR.H
	/// 
	/// 
	/// </summary>
	VkImageView createImageView(VkImage image, VkFormat format);
	/// <summary>
	/// Creation of texture sampler.
	/// </summary>
	void createTextureSampler();
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
	/// Creation of buffer. POR HACER --> VER SI MOVER A VULKANHELPER CLASS
	///
	///
	///
	/// </summary>
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	/// <summary>
	/// Creation of image buffer. POR HACER --> VER SI MOVER A VULKANHELPER CLASS
	/// 
	/// 
	/// 
	/// 
	/// </summary>
	void VulkanRenderEngine::createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	/// <summary>
	/// Creation of shader module. POR HACER --> VER SI MOVER A VULKANHELPER CLASS
	/// 
	/// </summary>
	VkShaderModule createShaderModule(const std::vector<char>& code);
	#pragma endregion

	#pragma region Query methods POR HACER --> A�ADIR PARAMS DE DOCUMENTACI�N
	/// <summary>
	/// Checks if our device can be used to our application requeriments.
	/// 
	/// </summary>
	bool isDeviceSuitable(VkPhysicalDevice device);
	/// <summary>
	/// Checks if our device support some indicated queue families.
	/// 
	/// </summary>
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	/// <summary>
	/// Checks if our device support swap chain with surface formats & present modes.
	/// 
	/// 
	/// </summary>
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	/// <summary>
	/// Checks our device memory.
	/// 
	/// 
	/// </summary>
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	#pragma endregion

	#pragma region Vulkan configuration methods POR HACER --> A�ADIR PARAMS DE DOCUMENTACI�N
	/// <summary>
	/// Drawing method.
	/// </summary>
	void drawFrame();
	/// <summary>
	/// Update the pipeline uniform buffer.
	/// 
	/// </summary>
	void updateUniformBuffer(uint32_t currentImage);
	/// <summary>
	/// Decide the hardware selected to use on render.
	/// </summary>
	void pickPhyshicalDevice();
	/// <summary>
	/// Chose swap chain surface format.
	/// 
	/// 
	/// </summary>
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	/// <summary>
	/// Chose swap chain present mode.
	/// 
	/// 
	/// </summary>
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
	/// <summary>
	/// Chose swap chain extent.
	/// 
	/// 
	/// </summary>
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	/// <summary>
	/// Allocate command and registry begin.  POR HACER --> VER SI MOVER A VULKANHELPER CLASS
	/// </summary>
	VkCommandBuffer beginSingleTimeCommands();
	/// <summary>
	/// Registry command buffer and free resources.  POR HACER --> VER SI MOVER A VULKANHELPER CLASS
	/// </summary>
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);
	/// <summary>
	/// Copy of buffer. POR HACER --> VER SI MOVER A VULKANHELPER CLASS
	///
	///
	///
	/// </summary>
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	/// <summary>
	/// Copy of buffer to a image. POR HACER --> VER SI MOVER A VULKANHELPER CLASS
	///
	///
	///
	/// </summary>
	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	/// <summary>
	/// Put image on a layout. POR HACER --> VER SI MOVER A VULKANHELPER CLASS
	///
	///
	///
	/// </summary>
	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
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