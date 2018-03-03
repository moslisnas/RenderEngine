#pragma once
/**
C++ Scene.h
Purpose: Header of Scene class

@author Alejandro Molina Quesada
@version 1.0
@date 03/03/2018
*/

#ifndef OPENGL
#define OPENGL
#define SOLVE_FGLUT_WARNING
#include <GL/glew.h>
#include <GL/freeglut.h>
#endif
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

#pragma region Other methods
	/// <summary>
	/// This method generate the final image to render on this frame.
	/// </summary>  
	void render();
#pragma endregion
};