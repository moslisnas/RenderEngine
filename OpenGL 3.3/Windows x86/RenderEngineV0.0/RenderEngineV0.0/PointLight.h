#pragma once
/**
C++ PointLight.h
Purpose: Header of PointLight class

@author Alejandro Molina Quesada
@version 1.0
@date 08/04/2018
*/

#ifndef GLM
#define GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
const float NUMBER_PI = (float)atan(1) * 4;
#endif

/// <summary>
/// Class for point lights of the scene, store information about position, intensity and color.
/// </summary>
class PointLight
{
public:
#pragma region Variables
	glm::mat4 light_matrix;
	float position[3];
	float diffuse_intensity[3];
	float specular_intensity[3];
#pragma endregion

#pragma region Constructor & destructor
	/// <summary>
	/// Constructor of <c>PointLight</c> class.
	/// </summary>
	PointLight();
	/// <summary>
	/// Destructor of <c>PointLight</c> class.
	/// </summary>
	~PointLight();
#pragma endregion

#pragma region Setter methods
	/// <summary>
	/// Setter of position.
	/// <param name="xyz">The position vector with same values at 3 components.</param>
	/// </summary>
	void setPosition(float xyz);
	/// <summary>
	/// Setter of position.
	/// <param name="x">The component X of the position vector.</param>
	/// <param name="y">The component Y of the position vector.</param> 
	/// <param name="z">The component Z of the position vector.</param> 
	/// </summary>
	void setPosition(float x, float y, float z);
	/// <summary>
	/// Setter of diffuse intensity.
	/// <param name="rgb">The color intensity with same values at 3 components.</param>
	/// </summary>
	void setDiffuseIntensity(float rgb);
	/// <summary>
	/// Setter of diffuse intensity.
	/// <param name="r">The component R of the color intensity.</param>
	/// <param name="g">The component G of the color intensity.</param> 
	/// <param name="b">The component B of the color intensity.</param> 
	/// </summary>
	void setDiffuseIntensity(float r, float g, float b);
	/// <summary>
	/// Setter of specular intensity.
	/// <param name="rgb">The color intensity with same values at 3 components.</param>
	/// </summary>
	void setSpecularIntensity(float rgb);
	/// <summary>
	/// Setter of specular intensity.
	/// <param name="r">The component R of the color intensity.</param>
	/// <param name="g">The component G of the color intensity.</param> 
	/// <param name="b">The component B of the color intensity.</param> 
	/// </summary>
	void setSpecularIntensity(float r, float g, float b);
#pragma endregion

#pragma region Other methods
	/// <summary>
	/// This method generate a point light with default parameters (White and 0,0,0 position).
	/// </summary>
	void loadDefault();
#pragma endregion
};