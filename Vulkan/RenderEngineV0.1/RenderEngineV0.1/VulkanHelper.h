/**
C++ VulkanHelper.h
Purpose: Header of VulkanHelper class

@author Alejandro Molina Quesada
@version 1.0
@date 09/10/2018
*/

#ifndef GLFW_INCLUDE_VULKAN
	#define GLFW_INCLUDE_VULKAN
	#include <GLFW/glfw3.h>
#endif
#ifndef INCLUDE_VECTOR
	#define INCLUDE_VECTOR
	#include <vector>
#endif
#ifndef IOS_AND_EXCEPT
	#define IOS_AND_EXCEPT
	#include <iostream>
	#include <stdexcept>
#endif
#ifndef STRUCTURE_DATAS
	#define STRUCTURE_DATAS
	#include <set>
	#include <array>
	#include "AuxiliarStructs.h"
#endif

const int MAX_FRAMES_IN_FLIGHT = 2;

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
#pragma endregion

class VulkanHelper{
private:
	#pragma region Singleton data member
	static VulkanHelper* instance;
	#pragma endregion

	#pragma region Contructor
	/// <summary>
	/// Constructor of <c>VulkanHelper</c> class.
	/// </summary>
	VulkanHelper();
	#pragma endregion

	#pragma region Data members
	#ifdef NDEBUG
	const bool enableValidationLayers = false;
	#else
	const bool enableValidationLayers = true;
	#endif
	VkDebugUtilsMessengerEXT callback;
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData){
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

		return VK_FALSE;
	}
	#pragma endregion
public:
	#pragma region Data members
	const std::vector<const char*> validationLayers = {"VK_LAYER_LUNARG_standard_validation"};
	const std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
	VkPhysicalDevice physicalDevice;
	VkDevice logicalDevice;
	VkQueue queue;
	VkCommandPool commandPool;
	#pragma endregion

	#pragma region Static access method
	static VulkanHelper* getInstance();
	#pragma endregion

	#pragma region Destructor
	/// <summary>
	/// Destructor of <c>VulkanHelper</c> class.
	/// </summary>
	~VulkanHelper();
	#pragma endregion

	#pragma region Operators
	VulkanHelper& operator=(const VulkanHelper& other);
	#pragma endregion

	#pragma region Query methods
	/// <summary>
	/// Method to check if debug mode is active.
	/// <returns>True if debug mode is active, false otherwise.</returns> 
	/// </summary>
	bool isDebugging();
	/// <summary>
	/// Check if validation layers can be used.
	/// <returns>True if we can use validation layer, false otherwise.</returns> 
	/// </summary>
	bool checkValidationLayerSupport();
	/// <summary>
	/// Check if our requested device extensions can be used.
	/// <param name="device">Logical device we want to check.</param>  
	/// <returns>True if we have all extensions of "deviceExtensions" available, false otherwise.</returns> 
	/// </summary>
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	/// <summary>
	/// Obtain our requestted application extensions.
	/// <returns>Glfw extensions and validation layer extension (if enabled).</returns> 
	/// </summary>
	std::vector<const char*> getRequiredExtensions();
	/// <summary>
	/// Print our application extensions.
	/// </summary>
	void printExtensions();
	/// <summary>
	/// Checks if its available one specific type of device memory and return his reference.
	/// <param name="typeFilter">The type filter of the memory that we are searching.</param>
	/// <param name="properties">Flags for the memory properties.</param>
	/// <param name="physicalDevice">The physical device where we search the memory type.</param>
	/// <returns>The reference to the memory.</returns> 
	/// </summary>
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice& physicalDevice);
	/// <summary>
	/// Checks if format has the stencil component.
	/// <param name="format">The format itself.</param>
	/// <returns>True if the format has stencil component, false otherwise.</returns> 
	/// </summary>
	bool hasStencilComponent(VkFormat format);
	#pragma endregion

	#pragma region Setting methods
	/// <summary>
	/// Method to set all the vulkan variables related to the device.
	/// <param name="physicalDevice">Physical device used on this VulkanHelper instance.</param>
	/// <param name="logicalDevice">Logical device used on this VulkanHelpe instance.</param>
	/// <param name="queue">Graphics queue used on this VulkanHelper instance.</param>
	/// </summary>
	void setDeviceData(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, VkQueue& queue);
	/// <summary>
	/// Method to set all the vulkan variables related to commands.
	/// <param name="commandPool">Command pool used on this VulkanHelper instance.</param>
	/// </summary>
	void setCommandsData(VkCommandPool& commandPool);
	#pragma endregion

	#pragma region Debug methods
	/// <summary>
	/// Setup the debug callback.
	/// <param name="instance">Vulkan instance where we setup the debug callback.</param>  
	/// </summary>
	void setupDebugCallback(VkInstance instance);
	/// <summary>
	/// Link the debug callback with the vulkan instance.
	/// <param name="instance">Vulkan instance where we bind the debug utility.</param>
	/// <param name="pCreateInfo">Struct with the debug utilities creation data.</param>
	/// <param name="pAllocator">Optional callback for memory allocator.</param>
	/// <param name="pCallback">Variable where we store the created element.</param>  
	/// </summary>
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback);
	/// <summary>
	/// Unlink the debug callback with the vulkan instance.
	/// <param name="instance">Vulkan instance where we destroy the debug utility.</param>
	/// <param name="callback">Callback we want to free from memory.</param>
	/// <param name="pAllocator">Optional callback for memory allocator.</param>
	/// </summary>
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator);
	#pragma endregion

	#pragma region Creation methods
	/// <summary>
	/// Creation of buffer.
	/// <param name="size">Buffer size.</param>
	/// <param name="usage">Flags to indicate the purpose of the buffer.</param>
	/// <param name="properties">Flags for the buffer memory properties.</param>
	/// <param name="buffer">Variable where we store the created buffer.</param>  
	/// <param name="bufferMemory">Variable where we store the buffer device memory data.</param>
	/// <param name="logicalDevice">The logical device where we create the buffer.</param>
	/// <param name="physicalDevice">The physical device to search the memory type to use for the buffer creation.</param>
	/// </summary>
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory, VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice);
	/// <summary>
	/// Creation of image.
	/// <param name="width">Image width.</param>
	/// <param name="height">Image height.</param>
	/// <param name="mipLevels">The mipmap levels used.</param>
	/// <param name="numSamples">The number of samples used for multisampling.</param>
	/// <param name="format">The format used to create the image.</param>
	/// <param name="tiling">The way we dispose the image texel data.</param>
	/// <param name="usage">Flags to indicate the purpose of the image.</param>
	/// <param name="properties">Flags for the image memory properties.</param>
	/// <param name="image">Variable where we store the created image.</param>  
	/// <param name="imageMemory">Variable where we store the image device memory data.</param>
	/// <param name="logicalDevice">The logical device where we create the image.</param>
	/// <param name="physicalDevice">The physical device to search the memory type to use for the image creation.</param>
	/// </summary>
	void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory, VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice);
	/// <summary>
	/// Creation of image view.
	/// <param name="image">The image from which we create the image view.</param>
	/// <param name="format">The format used to create the image view.</param>
	/// <param name="aspectFlags">Flags for the image view properties.</param>
	/// <param name="mipLevels">The mipmap levels used.</param>
	/// <param name="logicalDevice">The logical device where we create the image.</param>
	/// <returns>The image view created.</returns> 
	/// </summary>
	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels, VkDevice& logicalDevice);
	/// <summary>
	/// Creation of shader module.
	/// <param name="code">The code from which we built the module.</param>
	/// <param name="logicalDevice">The logical device where we create the module.</param>
	/// <returns>The shader module created.</returns> 
	/// </summary>
	VkShaderModule createShaderModule(const std::vector<char>& code, VkDevice& logicalDevice);
	#pragma endregion

	#pragma region Utility methods
	/// <summary>
	/// Copy of buffer.
	/// <param name="srcBuffer">The original buffer to copy.</param>
	/// <param name="dstBuffer">The destiny buffer to make the copy.</param>
	/// <param name="size">The buffer to copy size.</param>
	/// <param name="logicalDevice">The logical device where we do the copy.</param>
	/// <param name="commandPool">The command pool which perform the command.</param>
	/// <param name="queue">The queue where the copy is done.</param>
	/// </summary>
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkDevice& logicalDevice, VkCommandPool& commandPool, VkQueue& queue);
	/// <summary>
	/// Copy of buffer to a image.
	/// <param name="buffer">The original buffer to copy.</param>
	/// <param name="image">The destiny image to make the copy.</param>
	/// <param name="width">The image width.</param>
	/// <param name="height">The image height.</param>
	/// <param name="logicalDevice">The logical device where we do the copy.</param>
	/// <param name="commandPool">The command pool which perform the command.</param>
	/// <param name="queue">The queue where the copy is done.</param>
	/// </summary>
	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, VkDevice& logicalDevice, VkCommandPool& commandPool, VkQueue& queue);
	/// <summary>
	/// Allocate command buffer and registry begin.
	/// <param name="logicalDevice">The logical device where we registry the command.</param>
	/// <param name="commandPool">The command pool which perform the command.</param>
	/// <returns>The command buffer initialized.</returns> 
	/// </summary>
	VkCommandBuffer beginSingleTimeCommands(VkDevice& logicalDevice, VkCommandPool& commandPool);
	/// <summary>
	/// Registry command buffer and free resources.
	/// <param name="commandBuffer">The command buffer that we want to free.</param>
	/// <param name="logicalDevice">The logical device where we registry command buffer and free resources.</param>
	/// <param name="commandPool">The command pool which perform the command.</param>
	/// <param name="queue">The queue where the command is performed.</param>
	/// </summary>
	void endSingleTimeCommands(VkCommandBuffer commandBuffer, VkDevice& logicalDevice, VkCommandPool& commandPool, VkQueue& queue);
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
	/// Cleanup of VulkanHelper elements.
	/// <param name="instance">Vulkan instance where we clean.</param>  
	/// </summary>
	void cleanup(VkInstance instance);
	#pragma endregion
};