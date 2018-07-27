#pragma once
/**
C++ SceneEngine.h
Purpose: Header of SceneEngine class

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
#include "Scene.h"

/// <summary>
/// Class for managing several scenes.
/// This class let us to change all our pipeline configuration of our OpenGL 3.3
/// </summary>
class SceneEngine
{
private:
#pragma region Initizalization methods
	/// <summary>
	/// Initialize OpenGL context: context version, context profile, framebuffer and window.
	/// </summary>
	void static initContext();
	/// <summary>
	/// This method init the default OpenGL FrameBuffer.
	/// </summary>
	void static initFrameBuffer();
	/// <summary>
	/// This method init the window for rendering.
	/// <param name="width">Window size width, default value = 500px.</param>  
	/// <param name="height">Window size height, default value = 500px.</param>  
	/// <param name="x_position">X pixel position that window will take in the display device, 0 = left side, default value = 0.</param>  
	/// <param name="y_position">Y pixel position that window will take in the display device, 0 = top side, default value = 0.</param>  
	/// <param name="title">This text will appear on top bar of our window.</param>
	/// </summary>
	void static initWindow(int width = 500, int height = 500, int x_position = 0, int y_position = 0, char* title = "OpenGL Window");
	/// <summary>
	/// This method init the OpenGL extension manager.
	/// </summary>
	void static initExtensions();
	/// <summary>
	/// This method asociate OS event functions to OpenGL window.
	/// </summary>
	void static initOSEvents();
	/// <summary>
	/// This method configure graphics pipeline and rendering with several values.
	/// </summary>
	void initPipelineConfiguration();
#pragma endregion
public:
#pragma region Constructor & destructor
	/// <summary>
	/// Constructor of <c>SceneEngine</c> class.
	/// </summary> 
	SceneEngine();
	/// <summary>
	/// Destructor of <c>SceneEngine</c> class.
	/// </summary> 
	~SceneEngine();
#pragma endregion

#pragma region Initizalization methods
	/// <summary>
	/// Initialize local variables and OpenGL.
	/// <param name="argc">Number of our main parameters.</param>  
	/// <param name="argv">Parameters strings.</param>  
	/// </summary>  
	void initSceneEngine(int argc, char** argv);
	/// <summary>
	/// Initialize the default scene and the OpenGL MainLoop.
	/// </summary>  
	void initRender();
#pragma endregion

#pragma region Other methods
	void loadDefaultSceneRender();
#pragma endregion
};