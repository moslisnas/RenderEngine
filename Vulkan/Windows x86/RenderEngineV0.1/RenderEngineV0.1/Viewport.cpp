#include "Viewport.h"

#pragma region Contructor & destructor
/// <summary>
/// Constructor of <c>Viewport</c> class.
/// </summary>
Viewport::Viewport(){
}
/// <summary>
/// Destructor of <c>Viewport</c> class.
/// </summary>
Viewport::~Viewport(){
}
#pragma endregion

#pragma region Cleanup methods
/// <summary>
/// Cleanup of Viewport elements.
/// </summary>
void Viewport::cleanup(){
	glfwDestroyWindow(window);
	glfwTerminate();
}
#pragma endregion