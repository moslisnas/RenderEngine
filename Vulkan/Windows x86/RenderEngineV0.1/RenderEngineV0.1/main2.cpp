#ifndef GLFW_INCLUDE_VULKAN
	#define GLFW_INCLUDE_VULKAN
	#include <GLFW/glfw3.h>
#endif
#ifndef IOS_AND_EXCEPT
	#include <iostream>
	#include <stdexcept>
#endif
#include "VulkanRenderEngine.h"

int main(){
	VulkanRenderEngine vulkanApp;

	try{
		vulkanApp.run();
	}
	catch(const std::exception& e){
		std::cerr << e.what() << std::endl;
		system("PAUSE");
		return EXIT_FAILURE;
	}
	system("PAUSE");
	return EXIT_SUCCESS;
}