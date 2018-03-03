#pragma once
/**
C++ Scene.h
Purpose: Header of Camera class

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

/// <summary>
/// Class to define the camera, view and projection (frustrum & pespective).
/// </summary>
class Camera
{
public:
#pragma region Variables
	glm::mat4 projection_matrix;
	float projection_angle;
	float near_plane;
	float far_plane;
	glm::mat4 view_matrix;
	float position[3];
	float rotation[2];
#pragma endregion
#pragma region Constructor & destructor
	/// <summary>
	/// Constructor of <c>Camera</c> class.
	/// </summary>
	Camera();
	/// <summary>
	/// Destructor of <c>Camera</c> class.
	/// </summary>
	~Camera();
#pragma endregion
};

