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

#pragma region Initialization methods POR HACER --> AÑADIR PARAMS DE DOCUMENTACIÓN
/// <summary>
/// Initalize window elements.
/// </summary>
void Viewport::initWindow(){
	glfwInit();
	//Avoid set OpenGL as default render API and the resizable option for the window. 
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); //Window resize
	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
	//glfwSetWindowUserPointer(window, this);
	//glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
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