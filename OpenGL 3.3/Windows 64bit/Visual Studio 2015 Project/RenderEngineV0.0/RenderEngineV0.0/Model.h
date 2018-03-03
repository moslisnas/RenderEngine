#pragma once
/**
C++ Model.h
Purpose: Header of Model class

@author Alejandro Molina Quesada
@version 1.0
@date 03/03/2018
*/

#ifndef GLM
#define GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
const float NUMBER_PI = (float)atan(1) * 4;
#endif
#include "BOX.h"

/// <summary>
/// Class for models of the scene, store information about vertices, normals, etc.
/// </summary>
class Model
{
public:
#pragma region Variables
	glm::mat4 model_matrix;
	unsigned int n_vertices;
	unsigned int n_triangles;
	float *coordinates;
	unsigned int *triangle_indices;
	float *normals;
	float *colors;
	float *tangents;
	float *tex_coords;
#pragma endregion
#pragma region OpenGL id variables
	unsigned int vao_id;
	unsigned int posVBO_id;
	unsigned int colorVBO_id;
	unsigned int normalVBO_id;
	unsigned int tangentVBO_id;
	unsigned int texCoordVBO_id;
#pragma endregion

#pragma region Constructor & destructor
	/// <summary>
	/// Constructor of <c>Model</c> class.
	/// </summary>
	Model();
	/// <summary>
	/// Destructor of <c>Model</c> class.
	/// </summary>
	~Model();
#pragma endregion

#pragma region Other methods
	/// <summary>
	/// This method generate a cube with default parameters.
	/// </summary>
	void loadDefaultCubeModel();
#pragma endregion
};