#pragma once
/**
C++ Shader.h
Purpose: Header of Shader class

@author Alejandro Molina Quesada
@version 1.0
@date 04/03/2018
*/

/// <summary>
/// Class for shaders of the models, store information about uniform variables and attributes.
/// </summary>
class Shader
{
public:
#pragma region Variables
	unsigned int id;
	char* filePath;
	unsigned int type;
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

	void loadPhongVertexShader();

	void loadPhongFragmentShader();
#pragma endregion
};