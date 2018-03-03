#pragma once
/**
C++ Scene.h
Purpose: Header of Scene class

@author Alejandro Molina Quesada
@version 1.0
@date 03/03/2018
*/

#include "Camera.h"

/// <summary>
/// Class with all scene elements to show in final render.
/// </summary>
class Scene
{
public:
#pragma region Variables
	Camera selected_camera;
#pragma endregion
#pragma region Constructor & destructor
	/// <summary>
	/// Constructor of <c>Scene</c> class.
	/// </summary>
	Scene();
	/// <summary>
	/// Destructor of <c>Scene</c> class.
	/// </summary>
	~Scene();
#pragma endregion
#pragma region Setters
	/// <summary>
	/// Setter of <c>selected_camera</c> variable.
	/// <param name="camera">Camera we want use to render scene.</param> 
	/// </summary>
	void setSelectedCamera(Camera camera);
#pragma endregion
};