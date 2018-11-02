#include "Model.h"

#pragma region Contructor & destructor
/// <summary>
/// Constructor of <c>Model</c> class.
/// </summary>
Model::Model(){
}
/// <summary>
/// Destructor of <c>Model</c> class.
/// </summary>
Model::~Model(){
}
#pragma endregion

#pragma region Loading methods
/// <summary>
/// Loads a rectangle as model mesh.
/// </summary>
void Model::loadRectangle(){
	this->vulkanHelper = vulkanHelper;

	uint32_t numVertices = 4;
	uint32_t numIndices = 6;
	vertices.resize(numVertices);
	indices.resize(numIndices);

	//Coordinates data.
	Vertex auxVertex = {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}};
	vertices[0] = auxVertex;
	auxVertex = {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}};
	vertices[1] = auxVertex;
	auxVertex = {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}};
	vertices[2] = auxVertex;
	auxVertex = {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}};
	vertices[3] = auxVertex;

	//Indices data.
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 2;
	indices[4] = 3;
	indices[5] = 0;
}
/// <summary>
/// Loads a rectangle as model mesh.
/// </summary>
void Model::loadRectangle2(){
	this->vulkanHelper = vulkanHelper;

	uint32_t numVertices = 4;
	uint32_t numIndices = 6;
	vertices.resize(numVertices);
	indices.resize(numIndices);

	//Coordinates data.
	Vertex auxVertex = {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}};
	vertices[0] = auxVertex;
	auxVertex = {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}};
	vertices[1] = auxVertex;
	auxVertex = {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}};
	vertices[2] = auxVertex;
	auxVertex = {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}};
	vertices[3] = auxVertex;

	//Indices data.
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 2;
	indices[4] = 3;
	indices[5] = 0;
}
#pragma endregion