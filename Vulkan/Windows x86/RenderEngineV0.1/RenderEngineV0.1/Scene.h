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
	VulkanHelper vulkanHelper;

	std::vector<Model> models;
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
	/// Creation of default scene (a rectangle).
	/// 
	/// </summary>
	void createDefaultScene(VulkanHelper& vulkanHelper);
	#pragma endregion
};