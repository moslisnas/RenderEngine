/**
C++ Model.h
Purpose: Header of Model class

@author Alejandro Molina Quesada
@version 1.0
@date 29/10/2018
*/

#ifndef GLFW_INCLUDE_VULKAN
	#define GLFW_INCLUDE_VULKAN
	#include <GLFW/glfw3.h>
#endif
#ifndef INCLUDE_VECTOR
	#define INCLUDE_VECTOR
	#include <vector>
#endif
#ifndef STRUCTURE_DATAS
	#define STRUCTURE_DATAS
	#include <set>
	#include <array>
	#include "AuxiliarStructs.h"
#endif
#ifndef ALGEBRAIC_METHODS
	#define ALGEBRAIC_METHODS
	#include <algorithm>
	#include <glm/glm.hpp>
	#include <glm/gtc/matrix_transform.hpp>
#endif
#ifndef HELPER_CLASSES
	#define HELPER_CLASSES
	#include "VulkanHelper.h"
#endif

class Model{
public:
	#pragma region Data members
	VulkanHelper vulkanHelper;

	std::vector<Vertex> vertices;
	std::vector<uint16_t> indices;
	//Buffers.
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;
	#pragma endregion

	#pragma region Contructor & destructor
	/// <summary>
	/// Constructor of <c>Model</c> class.
	/// </summary>
	Model();
	/// <summary>
	/// Destructor of <c>Model</c> class.
	/// </summary>
	~Model();
	#pragma endregion
	
	#pragma region Creation methods
	/// <summary>
	/// Creation of vertex buffer.
	/// </summary>
	void createVertexBuffer();
	/// <summary>
	/// Creation of index buffer.
	/// </summary>
	void createIndexBuffer();
	#pragma endregion

	#pragma region Loading methods
	/// <summary>
	/// Loads a rectangle as model mesh.
	/// </summary>
	void loadRectangle(VulkanHelper& vulkanHelper);
	#pragma endregion

	#pragma region Cleanup methods
	/// <summary>
	/// Cleanup of Vulkan buffers.
	/// </summary>
	void cleanupBuffers();
	#pragma endregion
};