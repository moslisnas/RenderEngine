/**
C++ Texture.h
Purpose: Header of Texture class

@author Alejandro Molina Quesada
@version 1.0
@date 16/01/2019
*/

#ifndef GLOBAL_VARIABLES
#define GLOBAL_VARIABLES
#include "GlobalVariables.h"
#endif

class Texture{
public:
#pragma region Data members

#pragma endregion

#pragma region Contructor & destructor
	/// <summary>
	/// Constructor of <c>Texture</c> class.
	/// </summary>
	Texture();
	/// <summary>
	/// Destructor of <c>Texture</c> class.
	/// </summary>
	~Texture();
#pragma endregion

#pragma region Loading methods
	/// <summary>
	/// Loads a default image texture.
	/// </summary>
	void loadDefaultTexture();
#pragma endregion
};