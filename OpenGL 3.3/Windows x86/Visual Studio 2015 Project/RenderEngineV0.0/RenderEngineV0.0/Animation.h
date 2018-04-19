#pragma once
/**
C++ Animation.h
Purpose: Header of Animation class

@author Alejandro Molina Quesada
@version 1.0
@date 19/04/2018
*/

#ifndef GLM
#define GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
const float NUMBER_PI = (float)atan(1) * 4;
#endif

/// <summary>
/// Class to define animations of translation, rotation or scale.
/// </summary>
class Animation
{
public:
#pragma region Variables
#pragma endregion

#pragma region Contructor & destructor
	/// <summary>
	/// Constructor of <c>Animation</c> class.
	/// </summary>
	Animation();
	/// <summary>
	/// Destructor of <c>Animation</c> class.
	/// </summary>
	~Animation();
#pragma endregion

#pragma region Other methods

	void rotateItself();

	void loadDefaultAnimation()
	{
		rotateItself();
	}
#pragma endregion
};

