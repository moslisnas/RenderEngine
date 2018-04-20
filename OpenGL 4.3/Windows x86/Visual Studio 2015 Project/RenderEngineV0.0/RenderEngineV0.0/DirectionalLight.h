#pragma once
/**
C++ DirectionalLight.h
Purpose: Header of DirectionalLight class

@author Alejandro Molina Quesada
@version 1.0
@date 04/03/2018
*/

#ifndef GLM
#define GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
const float NUMBER_PI = (float)atan(1) * 4;
#endif

/// <summary>
/// Class for directional lights of the scene, store information about direction, intensity and color.
/// </summary>
class DirectionalLight
{
public:
#pragma region Variables
	glm::mat4 light_matrix;
	float direction[3];
	float diffuse_intensity[3];
	float specular_intensity[3];
#pragma endregion

#pragma region Constructor & destructor
	/// <summary>
	/// Constructor of <c>DirectionalLight</c> class.
	/// </summary>
	DirectionalLight();
	/// <summary>
	/// Destructor of <c>DirectionalLight</c> class.
	/// </summary>
	~DirectionalLight();
#pragma endregion

#pragma region Setter methods
	/// <summary>
	/// Setter of direction.
	/// <param name="xyz">The direction vector with same values at 3 components.</param>
	/// </summary>
	void setDirection(float xyz);
	/// <summary>
	/// Setter of direction.
	/// <param name="x">The component X of the direction vector.</param>
	/// <param name="y">The component Y of the direction vector.</param> 
	/// <param name="z">The component Z of the direction vector.</param> 
	/// </summary>
	void setDirection(float x, float y, float z);
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
	/// This method generate a directional light with default parameters (White and -Z direction).
	/// </summary>
	void loadDefault();
#pragma endregion
};