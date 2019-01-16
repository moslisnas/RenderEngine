#include "Model.h"
#ifndef TINYOBJLOADER_IMPLEMENTATION
	#define TINYOBJLOADER_IMPLEMENTATION
	#include <tiny_obj_loader.h>
	#include <unordered_map>
#endif
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

#pragma region Contructor & destructor
/// <summary>
/// Constructor of <c>Model</c> class.
/// </summary>
Model::Model(){
	this->numVertices = 0;
	this->numIndices = 0;
}
/// <summary>
/// Destructor of <c>Model</c> class.
/// </summary>
Model::~Model(){
}
#pragma endregion

#pragma region Creation methods
/// <summary>
/// Creation of texture image.
/// </summary>
void Model::createTextureImage(int i){
	//Loading image.
	int texWidth, texHeight, texChannels;
	stbi_uc* pixels;
	if(i==0)
		pixels = stbi_load("textures/binarycode.png", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	else
		pixels = stbi_load("textures/texture.jpg", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
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
/// Creation of texture image view.
/// </summary>
void Model::createTextureImageView(){
	//Texture image view creation.
	textureImageView = vulkanHelper->createImageView(textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT, mipLevels, vulkanHelper->logicalDevice);
}
/// <summary>
/// Creation of texture sampler.
/// </summary>
void Model::createTextureSampler(){
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
/// Mipmap generation for texture images.
/// <param name="image">The image from we want generate mipmap.</param>
/// <param name="imageFormat">The format of the image.</param>
/// <param name="texWidth">The texture image width.</param>
/// <param name="texHeight">The  texture image height.</param>
/// <param name="mipLevels">The number of levels used for mipmaps.</param>
/// </summary>
void Model::generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels){
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
#pragma endregion

#pragma region Loading methods
/// <summary>
/// Loads a rectangle as model mesh.
/// </summary>
void Model::loadRectangle(){
	//Texture.
	createTextureImage(0);
	createTextureImageView();
	createTextureSampler();

	this->numVertices = 4;
	this->numIndices = 6;
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
}
/// <summary>
/// Loads a rectangle as model mesh.
/// </summary>
void Model::loadRectangle2(){
	//Texture.
	createTextureImage(1);
	createTextureImageView();
	createTextureSampler();

	this->numVertices = 4;
	this->numIndices = 6;
	vertices.resize(numVertices);
	indices.resize(numIndices);

	//Coordinates data.
	Vertex auxVertex = {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}};
	vertices[0] = auxVertex;
	auxVertex = {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}};
	vertices[1] = auxVertex;
	auxVertex = {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}};
	vertices[2] = auxVertex;
	auxVertex = {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}};
	vertices[3] = auxVertex;

	//Indices data.
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 2;
	indices[4] = 3;
	indices[5] = 0;
}
/// <summary>
/// Loads an archive as model mesh.
/// <param name="file">Path of the model file.</param>
/// </summary>
void Model::loadFileModel(char * file){
	//Model data parameters.
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;
	//Load model data through the file.
	if(!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, file))
		throw std::runtime_error(err);

	//Data to avoid repeat vertices.
	std::unordered_map<Vertex, uint32_t> uniqueVertices = {};
	for(const auto& shape : shapes){
		for(const auto& index : shape.mesh.indices){
			//Reading vertices data.
			Vertex vertex = {};
			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};
			vertex.texCoord = {
				attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};
			vertex.color = {1.0f, 1.0f, 1.0f};
			//Pass data to our struct data.
			if(uniqueVertices.count(vertex) == 0){
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}
			indices.push_back(uniqueVertices[vertex]);
		}
	}
}
/// <summary>
/// Loads an archive as model mesh.
/// <param name="file">Path of the model file.</param>
/// </summary>
void Model::loadFileModel2(char * file){
	//Model data parameters.
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;
	//Load model data through the file.
	if(!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, file))
		throw std::runtime_error(err);

	//Data to avoid repeat vertices.
	std::unordered_map<Vertex, uint32_t> uniqueVertices ={};
	for(const auto& shape : shapes){
		for(const auto& index : shape.mesh.indices){
			//Reading vertices data.
			Vertex vertex ={};
			vertex.pos ={
				attrib.vertices[3 * index.vertex_index + 0] + 1.5,
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};
			vertex.texCoord ={
				attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};
			vertex.color ={1.0f, 1.0f, 1.0f};
			//Pass data to our struct data.
			if(uniqueVertices.count(vertex) == 0){
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}
			indices.push_back(uniqueVertices[vertex]);
		}
	}
}
/// <summary>
/// Loads an archive as model mesh.
/// <param name="file">Path of the model file.</param>
/// </summary>
void Model::loadFileModel3(char * file){
	//Model data parameters.
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;
	//Load model data through the file.
	if(!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, file))
		throw std::runtime_error(err);

	//Data to avoid repeat vertices.
	std::unordered_map<Vertex, uint32_t> uniqueVertices ={};
	for(const auto& shape : shapes){
		for(const auto& index : shape.mesh.indices){
			//Reading vertices data.
			Vertex vertex ={};
			vertex.pos ={
				attrib.vertices[3 * index.vertex_index + 0] - 1.5,
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};
			vertex.texCoord ={
				attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};
			vertex.color ={1.0f, 1.0f, 1.0f};
			//Pass data to our struct data.
			if(uniqueVertices.count(vertex) == 0){
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}
			indices.push_back(uniqueVertices[vertex]);
		}
	}
}
#pragma endregion

#pragma region Cleanup methods
/// <summary>
/// Cleanup of Vulkan texture.
/// </summary>
void Model::cleanupTexture(){
	vkDestroySampler(vulkanHelper->logicalDevice, textureSampler, nullptr);
	vkDestroyImageView(vulkanHelper->logicalDevice, textureImageView, nullptr);
	vkDestroyImage(vulkanHelper->logicalDevice, textureImage, nullptr);
	vkFreeMemory(vulkanHelper->logicalDevice, textureImageMemory, nullptr);
}
#pragma endregion