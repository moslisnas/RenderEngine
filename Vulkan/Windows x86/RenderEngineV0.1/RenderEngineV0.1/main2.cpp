#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>
#include "VulkanRenderEngine.h"

/*int main(){
	VulkanRenderEngine vulkanApp;

	try{
		vulkanApp.run();
	}
	catch(const std::exception& e){
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	system("PAUSE");
	return EXIT_SUCCESS;
}*/