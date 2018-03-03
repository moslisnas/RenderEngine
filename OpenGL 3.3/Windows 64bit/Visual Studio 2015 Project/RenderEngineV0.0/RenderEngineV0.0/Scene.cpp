#include "Scene.h"

#pragma region Constructor & destructor
/// <summary>
/// Constructor of <c>Scene</c> class.
/// </summary>
Scene::Scene()
{
}
/// <summary>
/// Destructor of <c>Scene</c> class.
/// </summary>
Scene::~Scene()
{
}
#pragma endregion

#pragma region Setters
/// <summary>
/// Setter of <c>selected_camera</c> variable.
/// <param name="camera">Camera we want use to render scene.</param> 
/// </summary>
void Scene::setSelectedCamera(Camera camera)
{
	this->selected_camera = camera;
}
#pragma endregion

#pragma region Adding methods
/// <summary>
/// This method add a model to the scene.
/// <param name="model">The model that will be added.</param>  
/// </summary>
void Scene::addModel(Model model)
{
	this->models.push_back(model);
}
#pragma endregion

#pragma region Other methods
/// <summary>
/// This method generate the final image to render on this frame.
/// </summary>  
void Scene::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glutSwapBuffers();
}
/// <summary>
/// This method generate a cube and add it to the scene.
/// </summary>
void Scene::generateCubeModel()
{
	Model cube;
	cube.loadDefaultCubeModel();
	this->addModel(cube);
}
#pragma endregion