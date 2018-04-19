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
#include "Texture.h"
#include "Animation.h"
#include "BOX.h"
#include "FileStreamLoad.h"
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>

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
	std::vector<Texture> textures;
	Animation anim;

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
	/// This method obtain uniform locations and store them.
	/// <param name="programId">Id of the program which contains this variables.</param>  
	/// </summary>
	void loadUniforms(int programId);
	/// <summary>
	/// This method obtain attribute locations and store them.
	/// <param name="programId">Id of the program which contains this variables.</param>  
	/// </summary>
	void loadAttributes(int programId);
	/// <summary>
	/// This method generate a cube with default parameters.
	/// <param name="shade_mode">Shade mode.</param>  
	/// </summary>
	void loadDefaultCubeModel(Shade shade_mode=PHONG);
	/// <summary>
	/// This method generate an imported model with default parameters.
	/// <param name="filePath">Path of the model.</param>  
	/// <param name="shade_mode">Shade mode.</param>  
	/// </summary>
	void loadAssimpModel(char* filePath, Shade shade_mode=TOON);
	/// <summary>
	/// This method generate default textures for a cube.
	/// </summary>
	void loadDefaultCubeTextures();
	/// <summary>
	/// This method generate default textures for an Assimp model.
	/// </summary>
	void loadDefaultAssimpModelTextures();
	/// <summary>
	/// This method create OpenGL texture from a file.
	/// <param name="fileName">Path of the file.</param>  
	/// <returns>OpenGL texture id.</returns> 
	/// </summary>
	unsigned int loadTex(const char *fileName);
	/// <summary>
	/// This method update animation.
	/// </summary>
	void animate();
#pragma endregion
};