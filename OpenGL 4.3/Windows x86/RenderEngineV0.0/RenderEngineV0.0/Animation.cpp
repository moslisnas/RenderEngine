#include "Animation.h"

#pragma region Constructor & destructor
/// <summary>
/// Constructor of <c>Animation</c> class.
/// </summary>
Animation::Animation()
{
	this->angle_x = 0.0f;
	this->angle_y = 0.0f;
	this->angle_z = 0.0f;
	this->rotation_speed_x = 0.0f;
	this->rotation_speed_y = 0.0f;
	this->rotation_speed_z = 0.0f;
}
/// <summary>
/// Destructor of <c>Animation</c> class.
/// </summary>
Animation::~Animation()
{
}
#pragma endregion

#pragma region Other methods
/// <summary>
/// This method generate a default animation.
/// <param name="position">The model position to update.</param>
/// <param name="rotation">The model rotation to update.</param>
/// </summary>
void Animation::updateDefaultAnimation(glm::vec3 &position, glm::vec3 &rotation)
{
	this->angle_x = (angle_x > NUMBER_PI * 2.0f) ? 0 : angle_x + rotation_speed_x;
	rotation.x = angle_x;
	this->angle_y = (angle_y > NUMBER_PI * 2.0f) ? 0 : angle_y + rotation_speed_y;
	rotation.y = angle_y;
}
#pragma endregion