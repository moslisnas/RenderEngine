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
#include "Viewport.h"
#include "VulkanHelper.h"

class VulkanRenderEngine{
private:
	#pragma region Data members
	Viewport viewport;
	VulkanHelper vulkanHelper;
	VkInstance instance;
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
	#pragma endregion

	#pragma region Cleanup methods
	/// <summary>
	/// Cleanup of Vulkan elements.
	/// </summary>
	void cleanup();
	#pragma endregion
};