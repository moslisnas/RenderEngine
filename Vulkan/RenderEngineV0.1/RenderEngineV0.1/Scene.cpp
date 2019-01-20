#include "Scene.h"

#pragma region Contructor & destructor
/// <summary>
/// Constructor of <c>Scene</c> class.
/// </summary>
Scene::Scene(){
	this->numModels = 0;
}
/// <summary>
/// Destructor of <c>Scene</c> class.
/// </summary>
Scene::~Scene(){
}
#pragma endregion

#pragma region Creation methods
/// <summary>
/// Creation of default scene.
/// </summary>
void Scene::createDefaultScene(){
	//Textures.
	//createTextureImage();
	//createTextureImageView();
	//createTextureSampler();

	//Models.
	numModels += 1;
	models.resize(numModels);
	models[0].loadRectangle();
	//models[1].loadRectangle2();
	//models[2].loadFileModel("Models/ToonTorus.obj");
	//models[3].loadFileModel2("Models/ToonTorus.obj");
	//models[4].loadFileModel3("Models/ToonTorus.obj");

	//Loading buffers.
	createVertexBuffer();
	createIndexBuffer();
}
/// <summary>
/// Creation of vertex buffer.
/// </summary>
void Scene::createVertexBuffer(){
	//Group all vertices data in the same vector.
	std::vector<Vertex> verticesData;
	for(unsigned int i=0; i<numModels; i++){
		for(unsigned int j=0; j<models[i].vertices.size(); j++)
			verticesData.push_back(models[i].vertices[j]);
	}

	//Getting size and CPU buffer creation.
	VkDeviceSize bufferSize = sizeof(verticesData[0]) * getNumVertices();
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	vulkanHelper->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory, vulkanHelper->logicalDevice, vulkanHelper->physicalDevice);

	//Copy vertex data to buffer.
	void* data;
	vkMapMemory(vulkanHelper->logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, verticesData.data(), (size_t)bufferSize);
	vkUnmapMemory(vulkanHelper->logicalDevice, stagingBufferMemory);

	//Creating and filling buffer at GPU.
	vulkanHelper->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory, vulkanHelper->logicalDevice, vulkanHelper->physicalDevice);

	//Copying the CPU buffer to GPU.
	vulkanHelper->copyBuffer(stagingBuffer, vertexBuffer, bufferSize, vulkanHelper->logicalDevice, vulkanHelper->commandPool, vulkanHelper->queue);

	//Free staging buffer resources.
	vkDestroyBuffer(vulkanHelper->logicalDevice, stagingBuffer, nullptr);
	vkFreeMemory(vulkanHelper->logicalDevice, stagingBufferMemory, nullptr);
}
/// <summary>
/// Creation of index buffer.
/// </summary>
void Scene::createIndexBuffer(){
	//Group all indices data in the same vector.
	std::vector<uint32_t> indicesData;
	size_t startIndex = 0;
	for(unsigned int i=0; i<numModels; i++){
		for(unsigned int j=0; j<models[i].indices.size(); j++)
			indicesData.push_back(models[i].indices[j] + startIndex);
		//We need to change where we start indexing.
		startIndex += models[i].vertices.size();
	}

	//Getting size and CPU buffer creation.
	VkDeviceSize bufferSize = sizeof(indicesData[0]) * getNumIndices();
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	vulkanHelper->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory, vulkanHelper->logicalDevice, vulkanHelper->physicalDevice);

	//Copy index data to buffer.
	void* data;
	vkMapMemory(vulkanHelper->logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, indicesData.data(), (size_t)bufferSize);
	vkUnmapMemory(vulkanHelper->logicalDevice, stagingBufferMemory);

	//Creating and filling buffer at GPU.
	vulkanHelper->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory, vulkanHelper->logicalDevice, vulkanHelper->physicalDevice);

	//Copying the CPU buffer to GPU.
	vulkanHelper->copyBuffer(stagingBuffer, indexBuffer, bufferSize, vulkanHelper->logicalDevice, vulkanHelper->commandPool, vulkanHelper->queue);

	//Free staging buffer resources.
	vkDestroyBuffer(vulkanHelper->logicalDevice, stagingBuffer, nullptr);
	vkFreeMemory(vulkanHelper->logicalDevice, stagingBufferMemory, nullptr);
}
#pragma endregion

#pragma region Query methods
/// <summary>
/// Get method of the vertices number.
/// <returns>Number of vertices of all the scene.</returns> 
/// </summary>
size_t Scene::getNumVertices(){
	size_t result = 0;

	for(unsigned int i=0; i<models.size(); i++)
		result += models[i].vertices.size();

	return result;
}
/// <summary>
/// Get method of the indices number.
/// <returns>Number of indices of all the scene.</returns> 
/// </summary>
size_t Scene::getNumIndices(){
	size_t result = 0;

	for(unsigned int i=0; i<models.size(); i++)
		result += models[i].indices.size();

	return result;
}
#pragma endregion

#pragma region Cleanup methods
/// <summary>
/// Cleanup of Vulkan textures.
/// </summary>
void Scene::cleanupTextures(){
	for(size_t i=0; i<numModels; i++)
		models[i].cleanupTexture();
}
/// <summary>
/// Cleanup of Vulkan buffers.
/// </summary>
void Scene::cleanupBuffers(){
	//Index buffer.
	vkDestroyBuffer(vulkanHelper->logicalDevice, indexBuffer, nullptr);
	vkFreeMemory(vulkanHelper->logicalDevice, indexBufferMemory, nullptr);
	//Vertex buffer.
	vkDestroyBuffer(vulkanHelper->logicalDevice, vertexBuffer, nullptr);
	vkFreeMemory(vulkanHelper->logicalDevice, vertexBufferMemory, nullptr);
}
#pragma endregion