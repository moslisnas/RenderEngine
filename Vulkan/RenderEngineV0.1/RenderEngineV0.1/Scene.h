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
#ifndef GLOBAL_VARIABLES
	#define GLOBAL_VARIABLES
	#include "GlobalVariables.h"
#endif
#include "Model.h"

class Scene{
public:
	#pragma region Data members
	unsigned int numModels;
	std::vector<Model> models;
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
	/// </summary>
	void createDefaultScene();
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
	size_t getNumVertices();
	/// <summary>
	/// Get method of the indices number.
	/// <returns>Number of indices of all the scene.</returns> 
	/// </summary>
	size_t getNumIndices();
	#pragma endregion

	#pragma region Cleanup methods
	/// <summary>
	/// Cleanup of Vulkan buffers.
	/// </summary>
	void cleanupBuffers();
	#pragma endregion
};