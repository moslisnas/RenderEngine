#include "Scene.h"
#ifndef STB_IMAGE_IMPLEMENTATION
	#define STB_IMAGE_IMPLEMENTATION
	#include <stb_image.h>
#endif

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
	createTextureImage();
	createTextureImageView();
	createTextureSampler();

	//Models.
	numModels += 5;
	models.resize(numModels);
	models[0].loadRectangle();
	models[1].loadRectangle2();
	models[2].loadFileModel("Models/ToonTorus.obj");
	models[3].loadFileModel2("Models/ToonTorus.obj");
	models[4].loadFileModel3("Models/ToonTorus.obj");

	//Loading buffers.
	createVertexBuffer();
	createIndexBuffer();
}
/// <summary>
/// Creation of texture image.
/// </summary>
void Scene::createTextureImage(){
	//Loading image.
	int texWidth, texHeight, texChannels;
	stbi_uc* pixels = stbi_load("textures/binarycode.png", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;

	VkDeviceSize imageSize = texWidth * texHeight * 4;
	if(!pixels)
		throw std::runtime_error("failed to load texture image!");

	//CPU buffer creation.
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	vulkanHelper->createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory, vulkanHelper->logicalDevice, vulkanHelper->physicalDevice);

	//Copy image data to buffer.
	void* data;
	vkMapMemory(vulkanHelper->logicalDevice, stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, pixels, static_cast<size_t>(imageSize));
	vkUnmapMemory(vulkanHelper->logicalDevice, stagingBufferMemory);

	//Free image variable.
	stbi_image_free(pixels);

	//Image buffer creation.
	vulkanHelper->createImage(texWidth, texHeight, mipLevels, VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory, vulkanHelper->logicalDevice, vulkanHelper->physicalDevice);

	//Put image on a layout.
	vulkanHelper->transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, mipLevels);

	//Put buffer image data on image resource and transitioned to VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL while generating mipmaps.
	vulkanHelper->copyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight), vulkanHelper->logicalDevice, vulkanHelper->commandPool, vulkanHelper->queue);

	//Free staging buffer resources.
	vkDestroyBuffer(vulkanHelper->logicalDevice, stagingBuffer, nullptr);
	vkFreeMemory(vulkanHelper->logicalDevice, stagingBufferMemory, nullptr);

	//Mipmaps generation.
	generateMipmaps(textureImage, VK_FORMAT_R8G8B8A8_UNORM, texWidth, texHeight, mipLevels);
}
/// <summary>
/// Mipmap generation for texture images.
/// <param name="image">The image from we want generate mipmap.</param>
/// <param name="imageFormat">The format of the image.</param>
/// <param name="texWidth">The texture image width.</param>
/// <param name="texHeight">The  texture image height.</param>
/// <param name="mipLevels">The number of levels used for mipmaps.</param>
/// </summary>
void Scene::generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels){
	// Check if image format supports linear blitting.
	VkFormatProperties formatProperties;
	vkGetPhysicalDeviceFormatProperties(vulkanHelper->physicalDevice, imageFormat, &formatProperties);
	if(!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
		throw std::runtime_error("texture image format does not support linear blitting!");

	//Allocate generate mipmaps command buffer.
	VkCommandBuffer commandBuffer = vulkanHelper->beginSingleTimeCommands(vulkanHelper->logicalDevice, vulkanHelper->commandPool);

	//Mipmaps memory barrier data.
	VkImageMemoryBarrier barrier ={};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.image = image;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.subresourceRange.levelCount = 1;

	//Mip map generation.
	int32_t mipWidth = texWidth;
	int32_t mipHeight = texHeight;
	for(uint32_t i = 1; i < mipLevels; i++){
		//Subresource range, layouts and masks for transfer barrier.
		barrier.subresourceRange.baseMipLevel = i - 1;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		//Mipmap transfer barrier.
		vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

		//Blit mipmap image data.
		VkImageBlit blit ={};
		blit.srcOffsets[0] ={0, 0, 0};
		blit.srcOffsets[1] ={mipWidth, mipHeight, 1};
		blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.srcSubresource.mipLevel = i - 1;
		blit.srcSubresource.baseArrayLayer = 0;
		blit.srcSubresource.layerCount = 1;
		blit.dstOffsets[0] ={0, 0, 0};
		blit.dstOffsets[1] ={mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1};
		blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.dstSubresource.mipLevel = i;
		blit.dstSubresource.baseArrayLayer = 0;
		blit.dstSubresource.layerCount = 1;
		//Blit mipmap image.
		vkCmdBlitImage(commandBuffer, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, VK_FILTER_LINEAR);

		//Subresource layouts and masks for memory barrier.
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		//Mipmap memory barrier.
		vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

		//Update mip level.
		if(mipWidth > 1)
			mipWidth /= 2;
		if(mipHeight > 1)
			mipHeight /= 2;
	}

	//Subresource range, layout and masks for memory barrier.
	barrier.subresourceRange.baseMipLevel = mipLevels - 1;
	barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	//Mipmap memory barrier.
	vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

	//Command buffer generate mipmaps.
	vulkanHelper->endSingleTimeCommands(commandBuffer, vulkanHelper->logicalDevice, vulkanHelper->commandPool, vulkanHelper->queue);
}
/// <summary>
/// Creation of texture image view.
/// </summary>
void Scene::createTextureImageView(){
	//Texture image view creation.
	textureImageView = vulkanHelper->createImageView(textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT, mipLevels, vulkanHelper->logicalDevice);
}
/// <summary>
/// Creation of texture sampler.
/// </summary>
void Scene::createTextureSampler(){
	//Sampler creation data.
	VkSamplerCreateInfo samplerInfo ={};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = 16;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.minLod = 0; // Optional
	samplerInfo.maxLod = static_cast<float>(mipLevels);
	samplerInfo.mipLodBias = 0; // Optional
	//Sampler creation.
	if(vkCreateSampler(vulkanHelper->logicalDevice, &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS)
		throw std::runtime_error("failed to create texture sampler!");
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
	vkDestroySampler(vulkanHelper->logicalDevice, textureSampler, nullptr);
	vkDestroyImageView(vulkanHelper->logicalDevice, textureImageView, nullptr);
	vkDestroyImage(vulkanHelper->logicalDevice, textureImage, nullptr);
	vkFreeMemory(vulkanHelper->logicalDevice, textureImageMemory, nullptr);
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