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
	#pragma region Data members POR HACER --> REVISAR PARAMETROS Y VER SI DEJAR AQUI LA DEFINICIÓN DEL DEBUGCALLBACK
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
	#pragma endregion

	#pragma region Contructor & destructor
	/// <summary>
	/// Constructor of <c>VulkanHelper</c> class.
	/// </summary>
	VulkanHelper();
	/// <summary>
	/// Destructor of <c>VulkanHelper</c> class.
	/// </summary>
	~VulkanHelper();
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
	
	#pragma region Cleanup methods
	/// <summary>
	/// Cleanup of VulkanHelper elements.
	/// <param name="instance">Vulkan instance where we clean.</param>  
	/// </summary>
	void cleanup(VkInstance instance);
	#pragma endregion
};