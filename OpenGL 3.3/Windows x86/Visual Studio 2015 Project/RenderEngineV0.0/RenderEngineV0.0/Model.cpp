#include "Model.h"


#pragma region Intialization methods (private)
/// <summary>
/// This method create VAO and store OpenGL VAO id.
/// </summary>
void Model::createVAO()
{
	glGenVertexArrays(1, &this->vao_id);
}
/// <summary>
/// This method create VBOs and store OpenGL VBOs id.
/// </summary>
void Model::createVBOs()
{
	glGenBuffers(1, &this->posVBO_id);
	glGenBuffers(1, &this->colorVBO_id);
	glGenBuffers(1, &this->normalVBO_id);
	glGenBuffers(1, &this->tangentVBO_id);
	glGenBuffers(1, &this->tex_coordVBO_id);
	glGenBuffers(1, &this->triangle_indexVBO_id);
}
/// <summary>
/// This method bind the VAO of this model.
/// </summary>
void Model::bindVAO()
{
	glBindVertexArray(vao_id);
}
/// <summary>
/// This method bind the VBOs of this model.
/// </summary>
void Model::bindVBOs()
{
	//Attribs VBOs
	if (this->vertex_shader.attrib_ids[0] != -1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, posVBO_id);
		glBufferData(GL_ARRAY_BUFFER, n_vertices * sizeof(float) * 3, coordinates, GL_STATIC_DRAW);
		glVertexAttribPointer(this->vertex_shader.attrib_ids[0], 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(this->vertex_shader.attrib_ids[0]);
	}
	if (this->vertex_shader.attrib_ids[1] != -1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, colorVBO_id);
		glBufferData(GL_ARRAY_BUFFER, n_vertices * sizeof(float) * 3, colors, GL_STATIC_DRAW);
		glVertexAttribPointer(this->vertex_shader.attrib_ids[1], 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(this->vertex_shader.attrib_ids[1]);
	}
	if (this->vertex_shader.attrib_ids[2] != -1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, normalVBO_id);
		glBufferData(GL_ARRAY_BUFFER, n_vertices * sizeof(float) * 3, normals, GL_STATIC_DRAW);
		glVertexAttribPointer(this->vertex_shader.attrib_ids[2], 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(this->vertex_shader.attrib_ids[2]);
	}
	if (this->vertex_shader.attrib_ids[3] != -1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, tangentVBO_id);
		glBufferData(GL_ARRAY_BUFFER, n_vertices * sizeof(float) * 3, tangents, GL_STATIC_DRAW);
		glVertexAttribPointer(this->vertex_shader.attrib_ids[3], 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(this->vertex_shader.attrib_ids[3]);
	}
	if (this->vertex_shader.attrib_ids[4] != -1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, tex_coordVBO_id);
		glBufferData(GL_ARRAY_BUFFER, n_vertices * sizeof(float) * 2, tex_coords, GL_STATIC_DRAW);
		glVertexAttribPointer(this->vertex_shader.attrib_ids[4], 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(this->vertex_shader.attrib_ids[4]);
	}
	//Index VBOs
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangle_indexVBO_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, n_triangles * sizeof(unsigned int) * 3, triangle_indices, GL_STATIC_DRAW);
}
#pragma endregion

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

#pragma region Intialization methods (public)
/// <summary>
/// This method generate OpenGL variables used for render the model.
/// </summary>
void Model::bind()
{
	if (this->vao_id == -1) {
		this->createVAO();
		this->createVBOs();
	}
	this->bindVAO();
	this->bindVBOs();
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
	vertex_shader.loadPhongVertexShader();
	fragment_shader.loadPhongFragmentShader();
}
#pragma endregion