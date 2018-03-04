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
	//Textures
	this->color_texture_on = true;
	this->emissive_texture_on = true;
	this->specular_texture_on = true;
	this->normal_texture_on = true;
}
/// <summary>
/// Destructor of <c>Model</c> class.
/// </summary>
Model::~Model()
{
}
#pragma endregion

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
/// <summary>
/// This method create textures and store OpenGL textures id.
/// </summary>
void Model::createTextures() {
	if (color_texture_on)
		color_texture_id = loadTex(color_texture_file);
	if (emissive_texture_on)
		emissive_texture_id = loadTex(emissive_texture_file);
	if (specular_texture_on)
		specular_texture_id = loadTex(specular_texture_file);
	if (normal_texture_on)
		normal_texture_id = loadTex(normal_texture_file);
}
/// <summary>
/// This method bind the textures of this model.
/// </summary>
void Model::bindTextures()
{
	if (color_texture_on) {
		glBindTexture(GL_TEXTURE_2D, color_texture_id);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	}
	if (emissive_texture_on) {
		glBindTexture(GL_TEXTURE_2D, emissive_texture_id);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	}
	if (specular_texture_on) {
		glBindTexture(GL_TEXTURE_2D, specular_texture_id);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	}
	if (normal_texture_on) {
		glBindTexture(GL_TEXTURE_2D, normal_texture_id);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	}
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
		this->createTextures();
	}
	this->bindVAO();
	this->bindVBOs();
	this->bindTextures();
}
#pragma endregion

#pragma region Other methods
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
	this->color_texture_on = true;
	this->emissive_texture_on = true;
	this->specular_texture_on = true;
	this->normal_texture_on = true;
	this->color_texture_file = "../resources/img/color2.png";
	this->emissive_texture_file = "../resources/img/emissive.png";
	this->specular_texture_file = "../resources/img/specMap.png";
	this->normal_texture_file = "../resources/img/normal.png";
	//Shaders
	vertex_shader.loadPhongVertexShader();
	fragment_shader.loadPhongFragmentShader();
}
/// <summary>
/// This method create OpenGL texture from a file.
/// <param name="fileName">Path of the file.</param>  
/// <returns>OpenGL texture id.</returns> 
/// </summary>
unsigned int Model::loadTex(const char *fileName) {
	unsigned char *map;
	unsigned int w, h;
	map = loadTexture(fileName, w, h);
	if (!map)
	{
		std::cout << "Error loading texture file: " << fileName << std::endl;
		exit(-1);
	}

	//Creating texture.
	unsigned int texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)map);

	delete[] map;

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);

	return texId;
}
#pragma endregion