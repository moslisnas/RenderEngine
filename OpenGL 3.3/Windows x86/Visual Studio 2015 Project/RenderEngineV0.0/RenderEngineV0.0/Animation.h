#pragma once
/**
C++ Animation.h
Purpose: Header of Animation class

@author Alejandro Molina Quesada
@version 1.0
@date 19/04/2018
*/

//#include "Model.h"
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
	float angle_x;
	float rotation_speed_x;
	float angle_y;
	float rotation_speed_y;
	float angle_z;
	float rotation_speed_z;
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
	/// <summary>
	/// This method generate a default animation.
	/// <param name="position">The model position to update.</param>
	/// <param name="rotation">The model rotation to update.</param>
	/// </summary>
	void updateDefaultAnimation(glm::vec3 &position, glm::vec3 &rotation);
#pragma endregion
};