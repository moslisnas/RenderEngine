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

#pragma region Initialization methods
/// <summary>
/// Initalize window elements.
/// <param name="framebufferResizeCallback">The callback used for framebuffer resize.</param>
/// </summary>
void Viewport::initWindow(GLFWframebuffersizefun framebufferResizeCallback){
	glfwInit();
	//Avoid set OpenGL as default render API and the resizable option for the window. 
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); //Window resize
	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}
#pragma endregion

#pragma region Creation methods
/// <summary>
/// Creation of surface.
/// <param name="instance">The vulkan instance where we create the surface.</param>
/// <param name="surface">The surface itself.</param>
/// </summary>
void Viewport::createSurface(VkInstance& instance, VkSurfaceKHR& surface){
	//Surface creation.
	if(glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
		throw std::runtime_error("failed to create window surface!");
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