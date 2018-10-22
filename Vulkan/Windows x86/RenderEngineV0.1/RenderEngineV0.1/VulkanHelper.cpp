#include "VulkanHelper.h"

#pragma region Contructor & destructor
/// <summary>
/// Constructor of <c>VulkanHelper</c> class.
/// </summary>
VulkanHelper::VulkanHelper(){
}
/// <summary>
/// Destructor of <c>VulkanHelper</c> class.
/// </summary>
VulkanHelper::~VulkanHelper(){
}
#pragma endregion

#pragma region Query methods
/// <summary>
/// Method to check if debug mode is active.
/// <returns>True if debug mode is active, false otherwise.</returns> 
/// </summary>
bool VulkanHelper::isDebugging(){
	return enableValidationLayers;
}
/// <summary>
/// Check if validation layers can be used.
/// <returns>True if we can use validation layer, false otherwise.</returns> 
/// </summary>
bool VulkanHelper::checkValidationLayerSupport(){
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for(const char * layerName:validationLayers){
		bool layerFound = false;

		for(const auto & layerProperties:availableLayers){
			if(strcmp(layerName, layerProperties.layerName) == 0){
				layerFound = true;
				break;
			}
		}
		if(!layerFound)
			return false;
	}

	return true;
}
/// <summary>
/// Check if our requested device extensions can be used.
/// <param name="device">Logical device we want to check.</param>  
/// <returns>True if we have all extensions of "deviceExtensions" available, false otherwise.</returns> 
/// </summary>
bool VulkanHelper::checkDeviceExtensionSupport(VkPhysicalDevice device){
	//Reading available extensions.
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	//Check if we have all extensions required of "deviceExtensions" list are available.
	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
	for(const auto& extension : availableExtensions)
		requiredExtensions.erase(extension.extensionName);

	return requiredExtensions.empty();
}
/// <summary>
/// Obtain our requestted application extensions.
/// <returns>Glfw extensions and validation layer extension (if enabled).</returns> 
/// </summary>
std::vector<const char *> VulkanHelper::getRequiredExtensions(){
	//Obtaining extensions.
	uint32_t glfwExtensionCount = 0;
	const char ** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
	//Adding validation layer extension.
	if(enableValidationLayers)
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

	return extensions;
}
/// <summary>
/// Print our application extensions.
/// </summary>
void VulkanHelper::printExtensions(){
	//Vulkan extensions.
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
	//Printing extensions.
	std::cout << "Available extensions:" << std::endl;
	for(const auto& extension : extensions)
		std::cout << "\t" << extension.extensionName << std::endl;
}
#pragma endregion

#pragma region Debug methods
/// <summary>
/// Setup the debug callback.
/// <param name="instance">Vulkan instance where we setup the debug callback.</param>  
/// </summary>
void VulkanHelper::setupDebugCallback(VkInstance instance) {
	//Debug extension creation data.
	VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
	createInfo.pUserData = nullptr; //Optional
	//Activate only on debug mode.
	if(!enableValidationLayers)
		return;
	//Create debug extension.
	if(CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &callback) != VK_SUCCESS)
		throw std::runtime_error("failed to set up debug callback!");
}
/// <summary>
/// Link the debug callback with the vulkan instance.
/// <param name="instance">Vulkan instance where we bind the debug utility.</param>
/// <param name="pCreateInfo">Struct with the debug utilities creation data.</param>
/// <param name="pAllocator">Optional callback for memory allocator.</param>
/// <param name="pCallback">Variable where we store the created element.</param>
/// </summary>
VkResult VulkanHelper::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback){
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

	if(func != nullptr)
		return func(instance, pCreateInfo, pAllocator, pCallback);
	else
		return VK_ERROR_EXTENSION_NOT_PRESENT;
}
/// <summary>
/// Unlink the debug callback with the vulkan instance.
/// <param name="instance">Vulkan instance where we destroy the debug utility.</param>
/// <param name="callback">Callback we want to free from memory.</param>
/// <param name="pAllocator">Optional callback for memory allocator.</param>
/// </summary>
void VulkanHelper::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator){
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

	if(func != nullptr)
		func(instance, callback, pAllocator);
}
#pragma endregion

#pragma region Utility methods
/// <summary>
/// Copy of buffer.
/// <param name="srcBuffer">The original buffer to copy.</param>
/// <param name="dstBuffer">The destiny buffer to make the copy.</param>
/// <param name="size">The buffer to copy size.</param>
/// <param name="logicalDevice">The logical device where we do the copy.</param>
/// <param name="commandPool">The command pool which perform the command.</param>
/// <param name="queue">The queue where the copy is done.</param>
/// </summary>
void VulkanHelper::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkDevice& logicalDevice, VkCommandPool& commandPool, VkQueue& queue){
	//Allocate copy command buffer.
	VkCommandBuffer commandBuffer = beginSingleTimeCommands(logicalDevice, commandPool);

	//Copy buffer data.
	VkBufferCopy copyRegion = {};
	copyRegion.size = size;
	//Copy buffer.
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	//Command buffer copying.
	endSingleTimeCommands(commandBuffer, logicalDevice, commandPool, queue);
}
/// <summary>
/// Copy of buffer to a image.
/// <param name="buffer">The original buffer to copy.</param>
/// <param name="image">The destiny image to make the copy.</param>
/// <param name="width">The image width.</param>
/// <param name="height">The image height.</param>
/// <param name="logicalDevice">The logical device where we do the copy.</param>
/// <param name="commandPool">The command pool which perform the command.</param>
/// <param name="queue">The queue where the copy is done.</param>
/// </summary>
void VulkanHelper::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, VkDevice& logicalDevice, VkCommandPool& commandPool, VkQueue& queue){
	//Allocate copy buffer to image command buffer.
	VkCommandBuffer commandBuffer = beginSingleTimeCommands(logicalDevice, commandPool);

	//Image buffer copy data.
	VkBufferImageCopy region ={};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;
	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;
	region.imageOffset ={0, 0, 0};
	region.imageExtent ={width, height, 1};
	//Image buffer copy.
	vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

	//Command buffer copy buffer to image.
	endSingleTimeCommands(commandBuffer, logicalDevice, commandPool, queue);
}
/// <summary>
/// Allocate command buffer and registry begin.
/// <param name="logicalDevice">The logical device where we registry the command.</param>
/// <param name="commandPool">The command pool which perform the command.</param>
/// <returns>The command buffer initialized.</returns> 
/// </summary>
VkCommandBuffer VulkanHelper::beginSingleTimeCommands(VkDevice& logicalDevice, VkCommandPool& commandPool){
	//Command buffer allocation data.
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = commandPool;
	allocInfo.commandBufferCount = 1;
	//Command buffer allocation.
	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(logicalDevice, &allocInfo, &commandBuffer);

	//Command buffer begin data.
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	//Command buffer begin.
	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	return commandBuffer;
}
/// <summary>
/// Registry command buffer and free resources.
/// <param name="commandBuffer">The command buffer that we want to free.</param>
/// <param name="logicalDevice">The logical device where we registry command buffer and free resources.</param>
/// <param name="commandPool">The command pool which perform the command.</param>
/// <param name="queue">The queue where the command is performed.</param>
/// </summary>
void VulkanHelper::endSingleTimeCommands(VkCommandBuffer commandBuffer, VkDevice& logicalDevice, VkCommandPool& commandPool, VkQueue& queue){
	//Command buffer end.
	vkEndCommandBuffer(commandBuffer);

	//Submit data.
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;
	//Submit.
	vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);

	//Synchronization.
	vkQueueWaitIdle(queue);

	//Free command buffer.
	vkFreeCommandBuffers(logicalDevice, commandPool, 1, &commandBuffer);
}
#pragma endregion

#pragma region Cleanup methods
/// <summary>
/// Cleanup of VulkanHelper elements.
/// <param name="instance">Vulkan instance where we clean.</param>  
/// </summary>
void VulkanHelper::cleanup(VkInstance instance){
	if(enableValidationLayers)
		DestroyDebugUtilsMessengerEXT(instance, callback, nullptr);
}
#pragma endregion