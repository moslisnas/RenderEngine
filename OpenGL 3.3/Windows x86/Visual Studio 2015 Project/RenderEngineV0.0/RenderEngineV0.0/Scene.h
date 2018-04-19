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
#include "PointLight.h"
#include "DirectionalLight.h"
#include "FocalLight.h"

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
	/// This method add a point light to the scene.
	/// <param name="light">The point light that will be added.</param>  
	/// </summary>
	void addPointLight(PointLight light);
	/// <summary>
	/// This method add a directional light to the scene.
	/// <param name="light">The directional light that will be added.</param>  
	/// </summary>
	void addDirectionalLight(DirectionalLight light);
	/// <summary>
	/// This method add a focal light to the scene.
	/// <param name="light">The focal light that will be added.</param>  
	/// </summary>
	void addFocalLight(FocalLight light);
#pragma endregion

public:
#pragma region Variables
	//Camera
	Camera selected_camera;
	//Models
	std::vector<Model> models;
	//Ligths
	float ambient_lighting[3];
	GLuint buffer_point_lights_id;
	GLuint block_point_lights_id;
	std::vector<PointLight> point_lights;
	GLuint buffer_directional_lights_id;
	GLuint block_directional_lights_id;
	std::vector<DirectionalLight> directional_lights;
	GLuint buffer_focal_lights_id;
	GLuint block_focal_lights_id;
	std::vector<FocalLight> focal_lights;
	//Programs
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
	/// <summary>
	/// Setter of <c>ambient_lighting</c> variable.
	/// <param name="rgb">Value used for lighting ambient component on the scene.</param> 
	/// </summary>
	void setAmbientLighting(float rgb);
	/// <summary>
	/// Setter of <c>ambient_lighting</c> variable.
	/// <param name="r">The component R of the color intensity.</param>
	/// <param name="g">The component G of the color intensity.</param> 
	/// <param name="b">The component B of the color intensity.</param> 
	/// </summary>
	void setAmbientLighting(float r, float g, float b);
#pragma endregion

#pragma region Initialization methods
	/// <summary>
	/// This method generate full scene to be rendered.
	/// </summary>
	void create();
	/// <summary>
	/// This method create Uniform Buffer Objects.
	/// <param name="programId">Program where we create the Uniform Buffer Object.</param> 
	/// </summary>
	void createUBOs(int programId);
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
	/// This method update scene elements when a key of the keyboard is pressed.
	/// <param name="key">Character of the pressed key.</param>  
	/// <param name="x">X position of the mouse cursor when button was pressed.</param>
	/// <param name="y">Y position of the mouse cursor when button was pressed.</param>
	/// </summary>  
	void keyboardInteraction(unsigned char key, int x, int y);
	/// <summary>
	/// This method bind the Uniform Buffers Objects used on shaders.
	/// <param name="programId">Program where we create the Uniform Buffer Object.</param> 
	/// </summary>  
	void bindUBOs(int programId);
	/// <summary>
	/// This method generate a cube and add it to the scene.
	/// <param name="shade_mode">Shade mode.</param>
	/// </summary>
	void createCubeModel(Shade shade_mode=PHONG);
	/// <summary>
	/// This method generate a model from a file and add it to the scene.
	/// <param name="filePath">Path of the model file.</param>
	/// <param name="shade_mode">Shade mode.</param>
	/// </summary>
	void createAssimpModel(char * filePath, Shade shade_mode=TOON);
	/// <summary>
	/// This method generate a point light and add it to the scene.
	/// </summary>
	void createPointLight();
	/// <summary>
	/// This method generate a directional light and add it to the scene.
	/// </summary>
	void createDirectionalLight();
	/// <summary>
	/// This method generate a focal light and add it to the scene.
	/// </summary>
	void createFocalLight();
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