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
	#include <vector>
#endif
#ifndef IOS_AND_EXCEPT
	#include <iostream>
	#include <stdexcept>
#endif

class VulkanHelper{
private:
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
	const std::vector<const char*> validationLayers ={"VK_LAYER_LUNARG_standard_validation"};
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

	#pragma region Query methods POR HACER --> Añadir return param
	/// <summary>
	/// Method to check if debug mode is active.
	/// 
	/// </summary>
	bool isDebugging();
	/// <summary>
	/// Check if validation layers can be used.
	/// </summary>
	bool checkValidationLayerSupport();
	/// <summary>
	/// Obtain our glfw application extensions.
	/// </summary>
	std::vector<const char*> getRequiredExtensions();
	/// <summary>
	/// Print our application extensions.
	/// </summary>
	void printExtensions();
	#pragma endregion

	#pragma region Debug methods POR HACER --> AÑADIR PARAMS DE DOCUMENTACIÓN
	/// <summary>
	/// Setup the debug callbacks.
	/// 
	/// </summary>
	void setupDebugCallback(VkInstance instance);
	/// <summary>
	/// Link the debug callback with the vulkan instance.
	/// 
	/// </summary>
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback);
	/// <summary>
	/// Unlink the debug callback with the vulkan instance.
	/// 
	/// </summary>
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator);
	#pragma endregion

	#pragma region Cleanup methods POR HACER --> AÑADIR PARAMS DE DOCUMENTACIÓN
	/// <summary>
	/// Cleanup of VulkanHelper elements.
	/// 
	/// </summary>
	void cleanup(VkInstance instance);
	#pragma endregion
};

