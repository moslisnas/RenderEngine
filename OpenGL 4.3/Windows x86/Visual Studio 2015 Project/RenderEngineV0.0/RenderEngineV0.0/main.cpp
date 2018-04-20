#include "SceneEngine.h"

int main(int argc, char** argv)
{
	SceneEngine engine;
	engine.initSceneEngine(argc, argv);
	engine.initRender();

	return 0;
}