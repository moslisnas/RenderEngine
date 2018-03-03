#include "SceneEngine.h"

//POR HACER--> Array de Scenes
Scene scene;

#pragma region Constructor & destructor
/// <summary>
/// Constructor of <c>SceneEngine</c> class.
/// </summary> 
SceneEngine::SceneEngine()
{
}
/// <summary>
/// Destructor of <c>SceneEngine</c> class.
/// </summary> 
SceneEngine::~SceneEngine()
{
}
#pragma endregion

/// <summary>
/// Rendering function.
/// </summary>  
void renderFunc() {
	scene.render();
}

#pragma region OS Event callbacks POR HACER-->Interacciones, varias cámaras
/// <summary>
/// OS event function for idle time.
/// </summary>  
void idleFunc() {
	//scene.animate();
}
/// <summary>
/// OS event function for window resizing.
/// <param name="width">Size width after resizing.</param>  
/// <param name="height">Size height after resizing.</param>  
/// </summary>  
void resizeFunc(int width, int height) {
	float aspect_ratio = (float)width / (float)height;
	float r, l, t, b;

	if (width > height) {
		r = scene.selected_camera.near_plane*tan(scene.selected_camera.projection_angle) * aspect_ratio;
		t = scene.selected_camera.near_plane*tan(scene.selected_camera.projection_angle);
	}
	else if (height > width) {
		r = scene.selected_camera.near_plane*tan(scene.selected_camera.projection_angle);
		t = scene.selected_camera.near_plane*tan(scene.selected_camera.projection_angle) * (1 / aspect_ratio);
	}
	else {
		r = scene.selected_camera.near_plane*tan(scene.selected_camera.projection_angle);
		t = scene.selected_camera.near_plane*tan(scene.selected_camera.projection_angle);
	}
	l = -r;
	b = -t;
	scene.selected_camera.updateProjectionMatrix(r, l, t, b);
	glViewport(0, 0, width, height);
	glutPostRedisplay();
}
/// <summary>
/// OS event function for keyboard actions.
/// <param name="key">Character of the pressed key.</param>  
/// <param name="x">.</param>  POR HACER
/// <param name="y">.</param>  POR HACER
/// </summary>  
void keyboardFunc(unsigned char key, int x, int y) {
	/*scene.keyboardCameraInteraction(key, x, y);

	//Render
	glutPostRedisplay();*/
}
/// <summary>
/// OS event function for mouse action (when a button is pressed).
/// <param name="button">Identifier of the pressed button.</param>  
/// <param name="state">Indicates if the button was pressed or if the button was released.</param>
/// <param name="x">X position of the mouse cursor when button was pressed.</param>
/// <param name="y">Y position of the mouse cursor when button was pressed.</param>
/// </summary>  
void mouseFunc(int button, int state, int x, int y)
{
	/*if (button == 0)
		last_x_position = x;*/
}
/// <summary>
/// OS event function for mouse action (mouse displacement).
/// <param name="x">X position of the mouse cursor.</param>
/// <param name="y">Y position of the mouse cursor.</param>
/// </summary>  
void mouseMotionFunc(int x, int y)
{
	/*scene.mouseCameraInteraction(x, y, last_x_position, last_y_position);

	//Render
	glutPostRedisplay();*/
}
#pragma endregion

#pragma region Initialization methods (private) POR HACER-->Cambiar contexto y parametros a partir de argc y argv (o enumerads)
/// <summary>
/// Initialize OpenGL context: context version, context profile, framebuffer and window.
/// </summary>
void SceneEngine::initContext()
{
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	initFrameBuffer();
	initWindow();
}

/// <summary>
/// This method init the default OpenGL FrameBuffer.
/// </summary>
void SceneEngine::initFrameBuffer()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
}

/// <summary>
/// This method init the window for rendering.
/// <param name="width">Window size width, default value = 500px.</param>  
/// <param name="height">Window size height, default value = 500px.</param>  
/// <param name="x_position">X pixel position that window will take in the display device, 0 = left side, default value = 0.</param>  
/// <param name="y_position">Y pixel position that window will take in the display device, 0 = top side, default value = 0.</param>  
/// <param name="title">This text will appear on top bar of our window.</param>
/// </summary>
void SceneEngine::initWindow(int width, int height, int x_position, int y_position, char* title)
{
	glutInitWindowSize(width, height);
	glutInitWindowPosition(x_position, y_position);
	glutCreateWindow(title);
}

/// <summary>
/// This method init the OpenGL extension manager.
/// </summary>
void SceneEngine::initExtensions()
{
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
		exit(-1);
	}
	const GLubyte *oglVersion = glGetString(GL_VERSION);
	std::cout << "This system supports OpenGL Version: " << oglVersion << std::endl;
}

/// <summary>
/// This method asociate OS event functions to OpenGL window.
/// </summary>
void SceneEngine::initOSEvents()
{
	glutReshapeFunc(resizeFunc);
	glutDisplayFunc(renderFunc);
	glutIdleFunc(idleFunc);
	glutKeyboardFunc(keyboardFunc);
	glutMouseFunc(mouseFunc);
	glutMotionFunc(mouseMotionFunc);
}

/// <summary>
/// This method configure graphics pipeline and rendering with several values.
/// </summary>
void SceneEngine::initPipelineConfiguration()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
}
#pragma endregion

#pragma region Initialization methods (public) POR HACER--> Cambiar para poder cargar más escenas, ventanas o varias zonas de render.
/// <summary>
/// Initialize local variables and OpenGL.
/// <param name="argc">Number of our main parameters.</param>  
/// <param name="argv">Parameters strings.</param>  
/// </summary>  
void SceneEngine::initSceneEngine(int argc, char** argv)
{
	glutInit(&argc, argv);
	initContext();
	initExtensions();
	initOSEvents();
	initPipelineConfiguration();
}
/// <summary>
/// Initialize the default scene and the OpenGL MainLoop.
/// </summary>  
void SceneEngine::initRender()
{
	loadDefaultSceneRender();
	glutMainLoop();
}
#pragma endregion

#pragma region Other methods
void SceneEngine::loadDefaultSceneRender()
{
	//Default camera
	Camera camera;
	scene.setSelectedCamera(camera);
	//Default geometry
	/*Model cube;
	cube.loadCubeModel();
	scene.addModel(cube);*/
	//Default light
	/**/

}
#pragma endregion