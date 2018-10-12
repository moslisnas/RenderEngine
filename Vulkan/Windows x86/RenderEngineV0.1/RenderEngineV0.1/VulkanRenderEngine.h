/**
C++ VulkanRenderEngine.h
Purpose: Header of VulkanRenderEngine class

@author Alejandro Molina Quesada
@version 1.0
@date 24/09/2018
*/

#ifndef IOS_AND_EXCEPT
	#include <iostream>
	#include <stdexcept>
#endif
#ifndef STRUCTURE_DATAS
	#include <set>
#endif
#ifndef ALGEBRAIC_METHODS
	#include <algorithm>
#endif
#include "VulkanHelper.h"
#include "Viewport.h"

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
	#pragma endregion
public:
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
	/// Method to launch the graphyc application.
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
	/// Creation of image views.
	/// </summary>
	void createImageViews();
	#pragma endregion

	#pragma region Query methods POR HACER --> AÑADIR PARAMS DE DOCUMENTACIÓN
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
	#pragma endregion

	#pragma region Vulkan configuration methods POR HACER --> AÑADIR PARAMS DE DOCUMENTACIÓN
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
	#pragma endregion

	#pragma region Cleanup methods
	/// <summary>
	/// Cleanup of Vulkan elements.
	/// </summary>
	void cleanup();
	#pragma endregion
};