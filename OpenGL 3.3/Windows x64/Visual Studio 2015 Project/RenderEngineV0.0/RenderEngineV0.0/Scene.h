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
#ifndef IOSTREAM
#define IOSTREAM
#include <iostream>
#endif
#ifndef VECTOR
#define VECTOR
#include <vector>
#endif
#include "Camera.h"
#include "Model.h"
#include "FileStreamLoad.h"
#include "DirectionalLight.h"

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
	/// <summary>
	/// This method add a directional light to the scene.
	/// <param name="light">The directional light that will be added.</param>  
	/// </summary>
	void addDirectionalLight(DirectionalLight light);
#pragma endregion

public:
#pragma region Variables
	Camera selected_camera;
	std::vector<Model> models;
	std::vector<DirectionalLight> directional_lights;
	unsigned int num_programs;
	unsigned int *programs;
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

#pragma region Initialization methods
	/// <summary>
	/// This method generate full scene to be rendered.
	/// </summary>
	void create();
	/// <summary>
	/// This method compile all model vertex/fragment shaders.
	/// </summary>
	void compileShaders();
	/// <summary>
	/// This method compile and link all model programs.
	/// </summary>
	void compilePrograms();
#pragma endregion

#pragma region Other methods
	/// <summary>
	/// This method generate the final image to render on this frame.
	/// </summary>  
	void render();
	/// <summary>
	/// This method update elements properties.
	/// </summary>  
	void animate();
	/// <summary>
	/// This method generate a cube and add it to the scene.
	/// </summary>
	void createCubeModel();
	/// <summary>
	/// This method generate a model from a file and add it to the scene.
	/// </summary>
	void createAssimpModel(char * filePath);
	/// <summary>
	/// This method generate a directional light and add it to the scene.
	/// </summary>
	void createDirectionalLight();
#pragma endregion

#pragma region Loading methods
	/// <summary>
	/// This method compile OpenGL shader from a file.
	/// <param name="fileName">Path of the file.</param>  
	/// <param name="type">Type os shader (vertices or fragments).</param>
	/// <returns>OpenGL shader id.</returns> 
	/// </summary>
	GLuint Scene::loadShader(const char *fileName, GLenum type);
#pragma endregion
};