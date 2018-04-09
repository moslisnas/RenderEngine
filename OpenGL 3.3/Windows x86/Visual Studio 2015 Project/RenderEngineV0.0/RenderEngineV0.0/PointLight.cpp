#include "PointLight.h"

#pragma region Constructor & destructor
/// <summary>
/// Constructor of <c>PointLight</c> class.
/// </summary>
PointLight::PointLight()
{
}
/// <summary>
/// Destructor of <c>PointLight</c> class.
/// </summary>
PointLight::~PointLight()
{
}
#pragma endregion

#pragma region Setter methods
/// <summary>
/// Setter of position.
/// <param name="xyz">The position vector with same values at 3 components.</param>
/// </summary>
void PointLight::setPosition(float xyz)
{
	this->position[0] = xyz;
	this->position[1] = xyz;
	this->position[2] = xyz;
}
/// <summary>
/// Setter of direction.
/// <param name="x">The component X of the direction vector.</param>
/// <param name="y">The component Y of the direction vector.</param> 
/// <param name="z">The component Z of the direction vector.</param> 
/// </summary>
void PointLight::setPosition(float x, float y, float z)
{
	this->position[0] = x;
	this->position[1] = y;
	this->position[2] = z;
}
/// <summary>
/// Setter of ambiental intensity.
/// <param name="rgb">The color intensity with same values at 3 components.</param>
/// </summary>
void PointLight::setAmbientalIntensity(float rgb)
{
	this->ambiental_intensity[0] = rgb;
	this->ambiental_intensity[1] = rgb;
	this->ambiental_intensity[2] = rgb;
}
/// <summary>
/// Setter of ambiental intensity.
/// <param name="r">The component R of the color intensity.</param>
/// <param name="g">The component G of the color intensity.</param> 
/// <param name="b">The component B of the color intensity.</param> 
/// </summary>
void PointLight::setAmbientalIntensity(float r, float g, float b)
{
	this->ambiental_intensity[0] = r;
	this->ambiental_intensity[1] = g;
	this->ambiental_intensity[2] = b;
}
/// <summary>
/// Setter of diffuse intensity.
/// <param name="rgb">The color intensity with same values at 3 components.</param>
/// </summary>
void PointLight::setDiffuseIntensity(float rgb)
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
void PointLight::setDiffuseIntensity(float r, float g, float b)
{
	this->diffuse_intensity[0] = r;
	this->diffuse_intensity[1] = g;
	this->diffuse_intensity[2] = b;
}
/// <summary>
/// Setter of specular intensity.
/// <param name="rgb">The color intensity with same values at 3 components.</param>
/// </summary>
void PointLight::setSpecularIntensity(float rgb)
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
void PointLight::setSpecularIntensity(float r, float g, float b)
{
	this->specular_intensity[0] = r;
	this->specular_intensity[1] = g;
	this->specular_intensity[2] = b;
}
#pragma endregion

#pragma region Other methods
/// <summary>
/// This method generate a point light with default parameters (White and 0,0,0 position).
/// </summary>
void PointLight::loadDefault()
{
	this->setPosition(0.0f, 0.0f, 0.0f);
	this->setAmbientalIntensity(0.3f, 0.0f, 0.0f);
	this->setDiffuseIntensity(0.3f, 0.0f, 0.0f);
	this->setSpecularIntensity(0.3f, 0.0f, 0.0f);
}
#pragma endregion