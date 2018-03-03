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

#pragma region OS Event callbacks POR HACER-->Interacciones, varias cámaras
void resizeFunc(int width, int height) {
	/*float aspect_ratio = (float)width / (float)height;
	float r, l, t, b;

	if (width > height) {
		r = scene.camera.n*tan(scene.camera.projectionAngle) * aspect_ratio;
		t = scene.camera.n*tan(scene.camera.projectionAngle);
	}
	else if (height > width) {
		r = scene.camera.n*tan(scene.camera.projectionAngle);
		t = scene.camera.n*tan(scene.camera.projectionAngle) * (1 / aspect_ratio);
	}
	else {
		r = scene.camera.n*tan(scene.camera.projectionAngle);
		t = scene.camera.n*tan(scene.camera.projectionAngle);
	}
	l = -r;
	b = -t;
	scene.camera.updateProjectionMatrix(r, l, t, b);
	glViewport(0, 0, width, height);
	glutPostRedisplay();*/
}
void renderFunc() {
	//scene.render();
}
void idleFunc() {
	//scene.animate();
}
void keyboardFunc(unsigned char key, int x, int y) {
	/*scene.keyboardCameraInteraction(key, x, y);

	//Render
	glutPostRedisplay();*/
}
void mouseFunc(int button, int state, int x, int y)
{
	/*if (button == 0)
		last_x_position = x;*/
}
void mouseMotionFunc(int x, int y)
{
	/*scene.mouseCameraInteraction(x, y, last_x_position, last_y_position);

	//Render
	glutPostRedisplay();*/
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
	glutInit(&argc, argv);
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
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	initFrameBuffer();
	initWindow();
}
void SceneEngine::initFrameBuffer()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
}
void SceneEngine::initWindow(int width, int height, int x_position, int y_position, char* title)
{
	glutInitWindowSize(width, height);
	glutInitWindowPosition(x_position, y_position);
	glutCreateWindow(title);
}
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
void SceneEngine::initOSEvents()
{
	glutReshapeFunc(resizeFunc);
	glutDisplayFunc(renderFunc);
	glutIdleFunc(idleFunc);
	glutKeyboardFunc(keyboardFunc);
	glutMouseFunc(mouseFunc);
	glutMotionFunc(mouseMotionFunc);
}
void SceneEngine::initPipelineConfiguration()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
}
#pragma endregion