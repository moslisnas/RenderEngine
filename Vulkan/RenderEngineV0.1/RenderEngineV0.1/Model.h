/**
C++ Model.h
Purpose: Header of Model class

@author Alejandro Molina Quesada
@version 1.0
@date 29/10/2018
*/

#ifndef GLFW_INCLUDE_VULKAN
	#define GLFW_INCLUDE_VULKAN
	#include <GLFW/glfw3.h>
#endif
#ifndef INCLUDE_VECTOR
	#define INCLUDE_VECTOR
	#include <vector>
#endif
#ifndef STRUCTURE_DATAS
	#define STRUCTURE_DATAS
	#include <set>
	#include <array>
	#include "AuxiliarStructs.h"
#endif
#ifndef ALGEBRAIC_METHODS
	#define ALGEBRAIC_METHODS
	#include <algorithm>
	#include <glm/glm.hpp>
	#include <glm/gtc/matrix_transform.hpp>
#endif
#include "Texture.h"

class Model{
public:
	#pragma region Data members
	uint32_t numVertices;
	std::vector<Vertex> vertices;
	uint32_t numIndices;
	std::vector<uint32_t> indices;
	//Textures.
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	VkImageView textureImageView;
	VkSampler textureSampler;
	//Texture texture;
	//Others.
	uint32_t mipLevels;
	#pragma endregion

	#pragma region Contructor & destructor
	/// <summary>
	/// Constructor of <c>Model</c> class.
	/// </summary>
	Model();
	/// <summary>
	/// Destructor of <c>Model</c> class.
	/// </summary>
	~Model();
	#pragma endregion

	#pragma region Creation methods
	/// <summary>
	/// Creation of texture image.
	/// </summary>
	void createTextureImage(int i);
	/// <summary>
	/// Creation of texture image view.
	/// </summary>
	void createTextureImageView();
	/// <summary>
	/// Creation of texture sampler.
	/// </summary>
	void createTextureSampler();
	/// <summary>
	/// Mipmap generation for texture images.
	/// <param name="image">The image from we want generate mipmap.</param>
	/// <param name="imageFormat">The format of the image.</param>
	/// <param name="texWidth">The texture image width.</param>
	/// <param name="texHeight">The  texture image height.</param>
	/// <param name="mipLevels">The number of levels used for mipmaps.</param>
	/// </summary>
	void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
	#pragma endregion

	#pragma region Loading methods
	/// <summary>
	/// Loads a rectangle as model mesh.
	/// </summary>
	void loadRectangle();
	/// <summary>
	/// Loads a rectangle as model mesh.
	/// </summary>
	void loadRectangle2();
	/// <summary>
	/// Loads an archive as model mesh.
	/// <param name="file">Path of the model file.</param>
	/// </summary>
	void loadFileModel(char * file);
	/// <summary>
	/// Loads an archive as model mesh.
	/// <param name="file">Path of the model file.</param>
	/// </summary>
	void loadFileModel2(char * file);
	/// <summary>
	/// Loads an archive as model mesh.
	/// <param name="file">Path of the model file.</param>
	/// </summary>
	void loadFileModel3(char * file);
	#pragma endregion

	#pragma region Cleanup methods
	/// <summary>
	/// Cleanup of Vulkan texture.
	/// </summary>
	void cleanupTexture();
	#pragma endregion
};