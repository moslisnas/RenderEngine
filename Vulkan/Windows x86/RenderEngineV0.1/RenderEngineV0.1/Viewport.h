/**
C++ Viewport.h
Purpose: Header of Viewport class

@author Alejandro Molina Quesada
@version 1.0
@date 01/10/2018
*/

#ifndef GLFW_INCLUDE_VULKAN
	#define GLFW_INCLUDE_VULKAN
	#include <GLFW/glfw3.h>
#endif
#ifndef IOS_AND_EXCEPT
	#include <iostream>
	#include <stdexcept>
#endif

class Viewport{
private:
	#pragma region Data members
	const int WIDTH = 800;
	const int HEIGHT = 600;
	#pragma endregion
public:
	#pragma region Data members
	GLFWwindow * window;
	#pragma endregion

	#pragma region Contructor & destructor
	/// <summary>
	/// Constructor of <c>Viewport</c> class.
	/// </summary>
	Viewport();
	/// <summary>
	/// Destructor of <c>Viewport</c> class.
	/// </summary>
	~Viewport();
	#pragma endregion

	#pragma region Initialization methods
	/// <summary>
	/// Initalize window elements.
	/// </summary>
	void initWindow();
	#pragma endregion

	#pragma region Cleanup methods
	/// <summary>
	/// Cleanup of Viewport elements.
	/// </summary>
	void cleanup();
#pragma endregion
};

