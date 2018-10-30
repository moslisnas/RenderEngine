#include "Model.h"

#pragma region Contructor & destructor
/// <summary>
/// Constructor of <c>Model</c> class.
/// </summary>
Model::Model(){
}
/// <summary>
/// Destructor of <c>Model</c> class.
/// </summary>
Model::~Model(){
}
#pragma endregion

#pragma region Creation methods
/// <summary>
/// Creation of vertex buffer.
/// </summary>
void Model::createVertexBuffer(){
	//Getting size and CPU buffer creation.
	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	vulkanHelper.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory, vulkanHelper.logicalDevice, vulkanHelper.physicalDevice);

	//Copy vertex data to buffer.
	void* data;
	vkMapMemory(vulkanHelper.logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, vertices.data(), (size_t)bufferSize);
	vkUnmapMemory(vulkanHelper.logicalDevice, stagingBufferMemory);

	//Creating and filling buffer at GPU.
	vulkanHelper.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory, vulkanHelper.logicalDevice, vulkanHelper.physicalDevice);

	//Copying the CPU buffer to GPU.
	vulkanHelper.copyBuffer(stagingBuffer, vertexBuffer, bufferSize, vulkanHelper.logicalDevice, vulkanHelper.commandPool, vulkanHelper.queue);

	//Free staging buffer resources.
	vkDestroyBuffer(vulkanHelper.logicalDevice, stagingBuffer, nullptr);
	vkFreeMemory(vulkanHelper.logicalDevice, stagingBufferMemory, nullptr);
}
/// <summary>
/// Creation of index buffer.
/// </summary>
void Model::createIndexBuffer(){
	//Getting size and CPU buffer creation.
	VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	vulkanHelper.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory, vulkanHelper.logicalDevice, vulkanHelper.physicalDevice);

	//Copy index data to buffer.
	void* data;
	vkMapMemory(vulkanHelper.logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, indices.data(), (size_t)bufferSize);
	vkUnmapMemory(vulkanHelper.logicalDevice, stagingBufferMemory);

	//Creating and filling buffer at GPU.
	vulkanHelper.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory, vulkanHelper.logicalDevice, vulkanHelper.physicalDevice);

	//Copying the CPU buffer to GPU.
	vulkanHelper.copyBuffer(stagingBuffer, indexBuffer, bufferSize, vulkanHelper.logicalDevice, vulkanHelper.commandPool, vulkanHelper.queue);

	//Free staging buffer resources.
	vkDestroyBuffer(vulkanHelper.logicalDevice, stagingBuffer, nullptr);
	vkFreeMemory(vulkanHelper.logicalDevice, stagingBufferMemory, nullptr);
}
#pragma endregion

#pragma region Loading methods
/// <summary>
/// Loads a rectangle as model mesh.
/// 
/// </summary>
void Model::loadRectangle(VulkanHelper& vulkanHelper){
	this->vulkanHelper = vulkanHelper;

	uint32_t numVertices = 4;
	uint32_t numIndices = 6;
	vertices.resize(numVertices);
	indices.resize(numIndices);

	//Coordinates data.
	Vertex auxVertex = {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}};
	vertices[0] = auxVertex;
	auxVertex = {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}};
	vertices[1] = auxVertex;
	auxVertex = {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}};
	vertices[2] = auxVertex;
	auxVertex = {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}};
	vertices[3] = auxVertex;

	//Indices data.
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 2;
	indices[4] = 3;
	indices[5] = 0;

	//Loading buffers.
	createVertexBuffer();
	createIndexBuffer();
}
#pragma endregion

#pragma region Cleanup methods
/// <summary>
/// Cleanup of Vulkan buffers.
/// </summary>
void Model::cleanupBuffers(){
	//Index buffer.
	vkDestroyBuffer(vulkanHelper.logicalDevice, indexBuffer, nullptr);
	vkFreeMemory(vulkanHelper.logicalDevice, indexBufferMemory, nullptr);
	//Vertex buffer.
	vkDestroyBuffer(vulkanHelper.logicalDevice, vertexBuffer, nullptr);
	vkFreeMemory(vulkanHelper.logicalDevice, vertexBufferMemory, nullptr);
}
#pragma endregion