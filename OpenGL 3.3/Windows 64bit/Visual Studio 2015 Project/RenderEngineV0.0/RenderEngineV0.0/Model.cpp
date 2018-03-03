#include "Model.h"

#pragma region Constructor & destructor
/// <summary>
/// Constructor of <c>Model</c> class.
/// </summary>
Model::Model()
{
	this->n_vertices = 0;
	this->n_triangles = 0;
	this->model_matrix = glm::mat4(1.0f);
	//Ids
	this->vao_id = -1;
}
/// <summary>
/// Destructor of <c>Model</c> class.
/// </summary>
Model::~Model()
{
}
#pragma endregion

#pragma region Other methods POR HACER--> Texturas y shaders del modelo cubo
/// <summary>
/// This method generate a cube with default parameters.
/// </summary>
void Model::loadDefaultCubeModel()
{
	//Initialization of cube model
	this->n_vertices = cubeNVertex;
	this->n_triangles = cubeNTriangleIndex;
	this->coordinates = new float[n_vertices * 3];
	this->triangle_indices = new unsigned int[n_triangles * 3];
	this->normals = new float[n_vertices * 3];
	this->colors = new float[n_vertices * 3];
	this->tangents = new float[n_vertices * 3];
	this->tex_coords = new float[n_vertices * 2];
	//Fill of cube data
	for(unsigned int i=0; i<n_vertices*3; i++){
		this->coordinates[i] = cubeVertexPos[i];
		this->normals[i] = cubeVertexNormal[i];
		this->colors[i] = cubeVertexColor[i];
		this->tangents[i] = cubeVertexTangent[i];
	}
	for(unsigned int i=0; i<n_vertices*2; i++)
		this->tex_coords[i] = cubeVertexTexCoord[i];
	for(unsigned int i=0; i<n_triangles*3; i++)
		this->triangle_indices[i] = cubeTriangleIndex[i];
	//Textures
	//Shaders
}
#pragma endregion