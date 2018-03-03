#include "Camera.h"

#pragma region Constructor & destructor
/// <summary>
/// Constructor of <c>Camera</c> class.
/// </summary>
Camera::Camera()
{
	this->projection_angle = NUMBER_PI / 6.0f;
	this->near_plane = 1.0f;
	this->far_plane = 50.0f;
	this->projection_matrix = glm::mat4(1.0f);
	this->projection_matrix = glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 50.0f);
	this->view_matrix = glm::mat4(1.0f);
	this->view_matrix[3].z = -6;
	this->position[0] = 0.0f;
	this->position[1] = 0.0f;
	this->position[2] = 6.0f;
	this->rotation[0] = 0.0f;
	this->rotation[1] = 0.0f;
}
/// <summary>
/// Destructor of <c>Camera</c> class.
/// </summary>
Camera::~Camera()
{
}
#pragma endregion