/**
C++ SceneEngine.h
Purpose: Header of SceneEngine class

@author Alejandro Molina Quesada
@version 1.0
*/

/// <summary>
/// Class for managing several scenes.
/// This class let us to change all our pipeline configuration of our OpenGL 3.3
/// </summary>
class SceneEngine
{
private:
	//Initializations
	void static initContext();
	void static initFrameBuffer();
	void static initWindow(int width = 1000, int height = 1000, int x_position = 0, int y_position = 0, char* title = "OpenGL Window");
	void static initExtensions();
	void static initOSEvents();
	void initPipelineConfiguration();
public:
	/// <summary>
	/// Constructor of <c>SceneEngine</c> class.
	/// </summary> 
	SceneEngine();

	/// <summary>
	/// Destructor of <c>SceneEngine</c> class.
	/// </summary> 
	~SceneEngine();

	//Initializations
	/// <summary>
	/// Initialize local variables and OpenGL.
	/// <param name="argc">Number of our main parameters.</param>  
	/// <param name="argv">Parameters strings.</param>  
	/// </summary>  
	void initSceneEngine(int argc, char** argv);
};