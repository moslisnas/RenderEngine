#pragma once
/**
C++ Texture.h
Purpose: Header of Texture class

@author Alejandro Molina Quesada
@version 1.0
@date 16/04/2018
*/

/// <summary>
/// Class for model textures.
/// </summary>
class Texture
{
public:
#pragma region Variables
	bool texture_on;
	char *texture_file;
	unsigned int texture_id;
#pragma endregion

#pragma region Constructor & destructor
	/// <summary>
	/// Constructor of <c>Texture</c> class.
	/// </summary>
	Texture();
	/// <summary>
	/// Destructor of <c>Texture</c> class.
	/// </summary>
	~Texture();
#pragma endregion
};

