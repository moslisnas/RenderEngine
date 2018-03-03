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
#include <vector>
#include "Camera.h"
#include "Model.h"

/// <summary>
/// Class with all scene elements to show in final render.
/// </summary>
class Scene
{
private:
#pragma region Adding methods
	/// <summary>
	/// This method add a model to the scene.
	/// <param name="model">The model that will be added.</param>  
	/// </summary>
	void addModel(Model model);
#pragma endregion
public:
#pragma region Variables
	Camera selected_camera;
	std::vector<Model> models;
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
	/// <summary>
	/// This method generate a cube and add it to the scene.
	/// </summary>
	void generateCubeModel();
#pragma endregion
};