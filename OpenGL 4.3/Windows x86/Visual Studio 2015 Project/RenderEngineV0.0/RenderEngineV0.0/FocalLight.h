#pragma once
/**
C++ FocalLight.h
Purpose: Header of FocalLight class

@author Alejandro Molina Quesada
@version 1.0
@date 12/04/2018
*/

#ifndef GLM
#define GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
const float NUMBER_PI = (float)atan(1) * 4;
#endif

/// <summary>
/// Class for focal lights of the scene, store information about position, direction, intensity, color, apperture angle and attenuation.
/// </summary>
class FocalLight
{
public:
#pragma region Variables
	glm::mat4 light_matrix;
	float position[3];
	float direction[3];
	float diffuse_intensity[3];
	float specular_intensity[3];
	float apperture_angle;
	float attenuation;
#pragma endregion

#pragma region Constructor & destructor
	/// <summary>
	/// Constructor of <c>FocalLight</c> class.
	/// </summary>
	FocalLight();
	/// <summary>
	/// Destructor of <c>FocalLight</c> class.
	/// </summary>
	~FocalLight();
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
	/// <summary>
	/// Setter of apperture angle.
	/// <param name="angle">The apperture angle in degrees or radians.</param>
	/// <param name="radians">True if we pass de angle in radians, false at degrees.</param>
	/// </summary>
	void setAppertureAngle(float angle, bool radians);
	/// <summary>
	/// Setter of attenuation factor.
	/// <param name="attenuation">The attenuation factor value.</param>
	/// </summary>
	void setAttenuation(float attenuation);
#pragma endregion

#pragma region Other methods
	/// <summary>
	/// This method generate a focal light with default parameters (White, 0,0,0 position, -Z direction, 15º apperture and 0.5 attenuation factor).
	/// </summary>
	void loadDefault();
#pragma endregion
};