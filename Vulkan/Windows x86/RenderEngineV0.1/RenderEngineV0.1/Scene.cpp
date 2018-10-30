#include "Scene.h"

#pragma region Contructor & destructor
/// <summary>
/// Constructor of <c>Scene</c> class.
/// </summary>
Scene::Scene(){
}
/// <summary>
/// Destructor of <c>Scene</c> class.
/// </summary>
Scene::~Scene(){
}
#pragma endregion

#pragma region Creation methods
/// <summary>
/// Creation of default scene (a rectangle).
/// 
/// </summary>
void Scene::createDefaultScene(VulkanHelper& vulkanHelper){
	this->vulkanHelper = vulkanHelper;

	uint32_t numModels = 1;
	models.resize(1);
	models[0].loadRectangle(this->vulkanHelper);
}
#pragma endregion