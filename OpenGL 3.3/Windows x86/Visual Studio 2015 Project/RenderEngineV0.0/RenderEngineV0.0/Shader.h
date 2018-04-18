#pragma once
/**
C++ Shader.h
Purpose: Header of Shader class

@author Alejandro Molina Quesada
@version 1.0
@date 04/03/2018
*/

#ifndef OPENGL
#define OPENGL
#define SOLVE_FGLUT_WARNING
#include <GL/glew.h>
#include <GL/freeglut.h>
#endif
#ifndef VECTOR
#define VECTOR
#include <vector>
#endif

/// <summary>
/// Class for shaders of the models, store information about uniform variables and attributes.
/// </summary>
class Shader
{
public:
#pragma region Variables
	unsigned int id;
	char* file_path;
	unsigned int type;
	#pragma region Uniforms and attributes
	unsigned int texture_start_index;
	unsigned int num_uniforms;
	unsigned int* uniform_ids;
	std::vector<char*> uniform_names;
	unsigned int num_attribs;
	unsigned int* attrib_ids;
	std::vector<char*> attrib_names;
	#pragma endregion
#pragma endregion

#pragma region Constructor & destructor
	/// <summary>
	/// Constructor of <c>Shader</c> class.
	/// </summary>
	Shader();
	/// <summary>
	/// Destructor of <c>Shader</c> class.
	/// </summary>
	~Shader();
#pragma endregion

#pragma region Shaders loading methods
	/// <summary>
	/// This method load more usual uniform variables for vertex shader.
	/// </summary>
	void loadCommonUniformsVertex();
	/// <summary>
	/// This method load more usual attributes for vertex shader.
	/// </summary>
	void loadCommonAttributesVertex();
	/// <summary>
	/// This method load more usual uniform variables for fragment shader.
	/// </summary>
	void loadCommonUniformsFragment();

	#pragma region Default shaders
	/// <summary>
	/// This method load a vertex shader for classic Phong shading method.
	/// </summary>
	void loadPhongVertexShader();
	/// <summary>
	/// This method load a fragment shader for classic Phong shading method.
	/// </summary>
	void loadPhongFragmentShader();
	/// <summary>
	/// This method load a vertex shader for Blinn-Phong shading method.
	/// </summary>
	void loadBlinnPhongVertexShader();
	/// <summary>
	/// This method load a fragment shader for Blinn-Phong shading method.
	/// </summary>
	void loadBlinnPhongFragmentShader();
	/// <summary>
	/// This method load a vertex shader for a bumped Phong shading method.
	/// </summary>
	void loadPhongBumpVertexShader();
	/// <summary>
	/// This method load a fragment shader for a bumped Phong shading method.
	/// </summary>
	void loadPhongBumpFragmentShader();
	/// <summary>
	/// This method load a vertex shader for a toon shading method.
	/// </summary>
	void loadToonVertexShader();
	/// <summary>
	/// This method load a fragment shader for a toon shading method.
	/// </summary>
	void loadToonFragmentShader();
	#pragma endregion

#pragma endregion
};