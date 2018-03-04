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
#ifndef IOSTREAM
#define IOSTREAM
#include <iostream>
#endif
#include "Shader.h"
#include "BOX.h"
#include "FileStreamLoad.h"

/// <summary>
/// Class for models of the scene, store information about vertices, normals, etc.
/// </summary>
class Model
{
private:
#pragma region Intialization methods
	/// <summary>
	/// This method create VAO and store OpenGL VAO id.
	/// </summary>
	void createVAO();
	/// <summary>
	/// This method create VBOs and store OpenGL VBOs id.
	/// </summary>
	void createVBOs();
	/// <summary>
	/// This method bind the VAO of this model.
	/// </summary>
	void bindVAO();
	/// <summary>
	/// This method bind the VBOs of this model.
	/// </summary>
	void bindVBOs();
	/// <summary>
	/// This method create textures and store OpenGL textures id.
	/// </summary>
	void createTextures();
	/// <summary>
	/// This method bind the textures of this model.
	/// </summary>
	void bindTextures();
#pragma endregion

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
	Shader vertex_shader;
	Shader fragment_shader;

	#pragma region Textures
	bool color_texture_on;
	bool emissive_texture_on;
	bool specular_texture_on;
	bool normal_texture_on;
	char *color_texture_file;
	char *emissive_texture_file;
	char *specular_texture_file;
	char *normal_texture_file;
	#pragma endregion

	#pragma region OpenGL id variables
	unsigned int vao_id;
	unsigned int posVBO_id;
	unsigned int colorVBO_id;
	unsigned int normalVBO_id;
	unsigned int tangentVBO_id;
	unsigned int tex_coordVBO_id;
	unsigned int triangle_indexVBO_id;
	unsigned int color_texture_id;
	unsigned int emissive_texture_id;
	unsigned int specular_texture_id;
	unsigned int normal_texture_id;
	#pragma endregion

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

#pragma region Intialization methods
	/// <summary>
	/// This method generate OpenGL variables used for render the model.
	/// </summary>
	void bind();
#pragma endregion

#pragma region Other methods
	/// <summary>
	/// This method generate a cube with default parameters.
	/// </summary>
	void loadDefaultCubeModel();
	/// <summary>
	/// This method create OpenGL texture from a file.
	/// <param name="fileName">Path of the file.</param>  
	/// <returns>OpenGL texture id.</returns> 
	/// </summary>
	unsigned int loadTex(const char *fileName);
#pragma endregion
};