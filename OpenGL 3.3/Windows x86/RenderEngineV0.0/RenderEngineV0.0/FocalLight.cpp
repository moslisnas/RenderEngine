#include "FocalLight.h"

#pragma region Constructor & destructor
/// <summary>
/// Constructor of <c>FocalLight</c> class.
/// </summary>
FocalLight::FocalLight()
{
	this->light_matrix = glm::mat4(1.0f);
	this->position[0] = 0.0f;
	this->position[1] = 0.0f;
	this->position[2] = 0.0f;
	this->direction[0] = 0.0f;
	this->direction[1] = 0.0f;
	this->direction[2] = 0.0f;
}
/// <summary>
/// Destructor of <c>FocalLight</c> class.
/// </summary>
FocalLight::~FocalLight()
{
}
#pragma endregion

#pragma region Setter methods
/// <summary>
/// Setter of position.
/// <param name="xyz">The position vector with same values at 3 components.</param>
/// </summary>
void FocalLight::setPosition(float xyz)
{
	this->position[0] = xyz;
	this->position[1] = xyz;
	this->position[2] = xyz;
}
/// <summary>
/// Setter of position.
/// <param name="x">The component X of the position vector.</param>
/// <param name="y">The component Y of the position vector.</param> 
/// <param name="z">The component Z of the position vector.</param> 
/// </summary>
void FocalLight::setPosition(float x, float y, float z)
{
	this->position[0] = x;
	this->position[1] = y;
	this->position[2] = z;
}
/// <summary>
/// Setter of direction.
/// <param name="xyz">The direction vector with same values at 3 components.</param>
/// </summary>
void FocalLight::setDirection(float xyz)
{
	this->direction[0] = xyz;
	this->direction[1] = xyz;
	this->direction[2] = xyz;
}
/// <summary>
/// Setter of direction.
/// <param name="x">The component X of the direction vector.</param>
/// <param name="y">The component Y of the direction vector.</param> 
/// <param name="z">The component Z of the direction vector.</param> 
/// </summary>
void FocalLight::setDirection(float x, float y, float z)
{
	this->direction[0] = x;
	this->direction[1] = y;
	this->direction[2] = z;
}
/// <summary>
/// Setter of diffuse intensity.
/// <param name="rgb">The color intensity with same values at 3 components.</param>
/// </summary>
void FocalLight::setDiffuseIntensity(float rgb)
{
	this->diffuse_intensity[0] = rgb;
	this->diffuse_intensity[1] = rgb;
	this->diffuse_intensity[2] = rgb;
}
/// <summary>
/// Setter of diffuse intensity.
/// <param name="r">The component R of the color intensity.</param>
/// <param name="g">The component G of the color intensity.</param> 
/// <param name="b">The component B of the color intensity.</param> 
/// </summary>
void FocalLight::setDiffuseIntensity(float r, float g, float b)
{
	this->diffuse_intensity[0] = r;
	this->diffuse_intensity[1] = g;
	this->diffuse_intensity[2] = b;
}
/// <summary>
/// Setter of specular intensity.
/// <param name="rgb">The color intensity with same values at 3 components.</param>
/// </summary>
void FocalLight::setSpecularIntensity(float rgb)
{
	this->specular_intensity[0] = rgb;
	this->specular_intensity[1] = rgb;
	this->specular_intensity[2] = rgb;
}
/// <summary>
/// Setter of specular intensity.
/// <param name="r">The component R of the color intensity.</param>
/// <param name="g">The component G of the color intensity.</param> 
/// <param name="b">The component B of the color intensity.</param> 
/// </summary>
void FocalLight::setSpecularIntensity(float r, float g, float b)
{
	this->specular_intensity[0] = r;
	this->specular_intensity[1] = g;
	this->specular_intensity[2] = b;
}
/// <summary>
/// Setter of apperture angle.
/// <param name="angle">The apperture angle in degrees or radians.</param>
/// <param name="radians">True if we pass de angle in radians, false at degrees.</param>
/// </summary>
void FocalLight::setAppertureAngle(float angle, bool radians)
{
	if (radians) {
		this->apperture_angle = angle;
	}
	else {
		this->apperture_angle = (angle * NUMBER_PI) / 180.0f;
	}
}
/// <summary>
/// Setter of attenuation factor.
/// <param name="attenuation">The attenuation factor value.</param>
/// </summary>
void FocalLight::setAttenuation(float attenuation)
{
	this->attenuation = attenuation;
}
#pragma endregion

#pragma region Other methods
/// <summary>
/// This method generate a focal light with default parameters (White, 0,0,0 position, -Z direction, 15º apperture and 0.5 attenuation factor).
/// </summary>
void FocalLight::loadDefault()
{
	this->setPosition(0.0f, 0.0f, 0.0f);
	this->setDirection(0.0f, 0.0f, 3.0f);
	this->setDiffuseIntensity(1.0f);
	this->setSpecularIntensity(1.0f);
	this->setAppertureAngle(7.5f, false);
	this->setAttenuation(0.5f);
}
#pragma endregion