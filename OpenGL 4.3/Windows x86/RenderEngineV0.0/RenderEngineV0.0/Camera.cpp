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

#pragma region Other methods
/// <summary>
/// This method update values of projection matrix (frustrum).
/// <param name="r">Right limit of frustrum.</param> 
/// <param name="l">Left limit of frustrum.</param> 
/// <param name="t">Top limit of frustrum.</param> 
/// <param name="b">Bottom limit of frustrum.</param> 
/// </summary>
void Camera::updateProjectionMatrix(float r, float l, float t, float b)
{
	this->projection_matrix[0].x = (2*near_plane) / (r-l);
	this->projection_matrix[1].y = (2*near_plane) / (t-b);
	this->projection_matrix[2].x = (r+l) / (r-l);
	this->projection_matrix[2].y = (t+b) / (t-b);
	this->projection_matrix[2].z = -(far_plane+near_plane) / (far_plane-near_plane);
	this->projection_matrix[2].w = -1.0f;
	this->projection_matrix[3].z = (-2*far_plane*near_plane) / (far_plane-near_plane);
}
#pragma endregion