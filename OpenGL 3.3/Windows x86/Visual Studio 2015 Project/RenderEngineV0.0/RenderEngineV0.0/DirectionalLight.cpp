#include "DirectionalLight.h"

#pragma region Constructor & destructor
/// <summary>
/// Constructor of <c>DirectionalLight</c> class.
/// </summary>
DirectionalLight::DirectionalLight()
{
	this->light_matrix = glm::mat4(1.0f);
	this->direction[0] = 0.0f;
	this->direction[1] = 0.0f;
	this->direction[2] = 0.0f;
}
/// <summary>
/// Destructor of <c>DirectionalLight</c> class.
/// </summary>
DirectionalLight::~DirectionalLight()
{
}
#pragma endregion

#pragma region Setter methods
/// <summary>
/// Setter of direction.
/// <param name="xyz">The direction vector with same values at 3 components.</param>
/// </summary>
void DirectionalLight::setDirection(float xyz)
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
void DirectionalLight::setDirection(float x, float y, float z)
{
	this->direction[0] = x;
	this->direction[1] = y;
	this->direction[2] = z;
}
/// <summary>
/// Setter of ambiental intensity.
/// <param name="rgb">The color intensity with same values at 3 components.</param>
/// </summary>
void DirectionalLight::setAmbientalIntensity(float rgb)
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
void DirectionalLight::setAmbientalIntensity(float r, float g, float b)
{
	this->ambiental_intensity[0] = r;
	this->ambiental_intensity[1] = g;
	this->ambiental_intensity[2] = b;
}
/// <summary>
/// Setter of diffuse intensity.
/// <param name="rgb">The color intensity with same values at 3 components.</param>
/// </summary>
void DirectionalLight::setDiffuseIntensity(float rgb)
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
void DirectionalLight::setDiffuseIntensity(float r, float g, float b)
{
	this->diffuse_intensity[0] = r;
	this->diffuse_intensity[1] = g;
	this->diffuse_intensity[2] = b;
}
/// <summary>
/// Setter of specular intensity.
/// <param name="rgb">The color intensity with same values at 3 components.</param>
/// </summary>
void DirectionalLight::setSpecularIntensity(float rgb)
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
void DirectionalLight::setSpecularIntensity(float r, float g, float b)
{
	this->specular_intensity[0] = r;
	this->specular_intensity[1] = g;
	this->specular_intensity[2] = b;
}
#pragma endregion

#pragma region Other methods
/// <summary>
/// This method generate a directional light with default parameters (White and -Z direction).
/// </summary>
void DirectionalLight::loadDefault()
{
	this->setDirection(1.0f, 0.0f, 0.0f);
	this->setAmbientalIntensity(0.3f);
	this->setDiffuseIntensity(1.0f);
	this->setSpecularIntensity(1.0f);
}
#pragma endregion