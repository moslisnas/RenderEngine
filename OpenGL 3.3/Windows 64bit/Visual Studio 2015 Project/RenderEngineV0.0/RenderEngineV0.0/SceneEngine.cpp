#include "SceneEngine.h"

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

#pragma region Initialization methods POR HACER-->CAMBIAR CONTEXTO Y PARAMETROS A PARTIR DE ARGC Y ARGV O ENUMERADOS
/// <summary>
/// Initialize local variables and OpenGL.
/// <param name="argc">Number of our main parameters.</param>  
/// <param name="argv">Parameters strings.</param>  
/// </summary>  
void SceneEngine::initSceneEngine(int argc, char** argv)
{
	//glutInit(&argc, argv);
	initContext();
	initExtensions();
	initOSEvents();
	initPipelineConfiguration();
}

/// <summary>
/// Initialize OpenGL context: context version, context profile, framebuffer and window.
/// </summary>  
void SceneEngine::initContext()
{
	/*glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);*/
	initFrameBuffer();
	initWindow();
}
void SceneEngine::initFrameBuffer()
{
	//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
}
void SceneEngine::initWindow(int width, int height, int x_position, int y_position, char* title)
{
	/*glutInitWindowSize(width, height);
	glutInitWindowPosition(x_position, y_position);
	glutCreateWindow(title);*/
}
void SceneEngine::initExtensions()
{
	//Extensiones
	/*glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
		exit(-1);
	}
	const GLubyte *oglVersion = glGetString(GL_VERSION);
	std::cout << "This system supports OpenGL Version: " << oglVersion << std::endl;*/
}
void SceneEngine::initOSEvents()
{
	/*glutReshapeFunc(resizeFunc);
	glutDisplayFunc(renderFunc);
	glutIdleFunc(idleFunc);
	glutKeyboardFunc(keyboardFunc);
	glutMouseFunc(mouseFunc);
	glutMotionFunc(mouseMotionFunc);*/
}
void SceneEngine::initPipelineConfiguration()
{
	/*glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);*/
}
#pragma endregion