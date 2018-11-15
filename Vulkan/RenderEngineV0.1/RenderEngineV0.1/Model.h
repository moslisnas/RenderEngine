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

class Model{
public:
	#pragma region Data members
	uint32_t numVertices;
	std::vector<Vertex> vertices;
	uint32_t numIndices;
	std::vector<uint32_t> indices;
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
};