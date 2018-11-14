/**
C++ Scene.h
Purpose: Header of Scene class

@author Alejandro Molina Quesada
@version 1.0
@date 29/10/2018
*/

#ifndef INCLUDE_VECTOR
	#define INCLUDE_VECTOR
	#include <vector>
#endif
#ifndef HELPER_CLASSES
	#define HELPER_CLASSES
	#include "VulkanHelper.h"
#endif
#include "Model.h"

class Scene{
public:
	#pragma region Data members
	unsigned int numModels;
	std::vector<Model> models;
	//Vulkan helper.
	VulkanHelper vulkanHelper;
	//Buffers.
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;
	#pragma endregion

	#pragma region Contructor & destructor
	/// <summary>
	/// Constructor of <c>Scene</c> class.
	/// </summary>
	Scene();
	/// <summary>
	/// Destructor of <c>Scene</c> class.
	/// </summary>
	~Scene();
	#pragma endregion

	#pragma region Creation methods
	/// <summary>
	/// Creation of default scene.
	/// <param name="vulkanHelper">VulkanHelper instance with all the information about: physical/logical device, queue and command pool.</param>
	/// </summary>
	void createDefaultScene(VulkanHelper& vulkanHelper);
	/// <summary>
	/// Creation of vertex buffer.
	/// </summary>
	void createVertexBuffer();
	/// <summary>
	/// Creation of index buffer.
	/// </summary>
	void createIndexBuffer();
	#pragma endregion

	#pragma region Query methods
	/// <summary>
	/// Get method of the vertices number.
	/// <returns>Number of vertices of all the scene.</returns> 
	/// </summary>
	int getNumVertices();
	/// <summary>
	/// Get method of the indices number.
	/// <returns>Number of indices of all the scene.</returns> 
	/// </summary>
	int getNumIndices();
	#pragma endregion

	#pragma region Cleanup methods
	/// <summary>
	/// Cleanup of Vulkan buffers.
	/// </summary>
	void cleanupBuffers();
	#pragma endregion
};