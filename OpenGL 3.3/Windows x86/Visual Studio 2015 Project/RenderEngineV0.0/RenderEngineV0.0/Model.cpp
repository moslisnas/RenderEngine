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
	for(unsigned int i=0; i<textures.size(); i++){
		if(textures[i].texture_on)
			this->textures[i].texture_id = loadTex(textures[i].texture_file);
	}
}
/// <summary>
/// This method bind the textures of this model.
/// </summary>
void Model::bindTextures()
{
	for(unsigned int i=0; i<textures.size(); i++){
		if(textures[i].texture_on){
			glBindTexture(GL_TEXTURE_2D, textures[i].texture_id);
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		}
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

#pragma region Setters
/// <summary>
/// Setter of model position.
/// <param name="x">The x component of position.</param>
/// <param name="y">The y component of position.</param>
/// <param name="z">The z component of position.</param>
/// </summary>
void Model::setPosition(float x, float y, float z)
{
	this->position.x = x;
	this->position.y = y;
	this->position.z = z;
}
#pragma endregion

#pragma region Other methods
/// <summary>
/// This method obtain uniform locations and store them.
/// <param name="programId">Id of the program which contains this variables.</param>  
/// </summary>
void Model::loadUniforms(int programId) {
	for(unsigned int i=0; i<vertex_shader.num_uniforms; i++)
		this->vertex_shader.uniform_ids[i] = glGetUniformLocation(programId, vertex_shader.uniform_names[i]);
	for(unsigned int i=0; i<fragment_shader.num_uniforms; i++)
		this->fragment_shader.uniform_ids[i] = glGetUniformLocation(programId, fragment_shader.uniform_names[i]);
}
/// <summary>
/// This method obtain attribute locations and store them.
/// <param name="programId">Id of the program which contains this variables.</param>  
/// </summary>
void Model::loadAttributes(int programId) {
	for(unsigned int j = 0; j<vertex_shader.num_attribs; j++)
		vertex_shader.attrib_ids[j] = glGetAttribLocation(programId, vertex_shader.attrib_names[j]);
}
/// <summary>
/// This method generate a cube with default parameters.
/// <param name="shade_mode">Shade mode.</param>  
/// </summary>
void Model::loadDefaultCubeModel(Shade shade_mode)
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
	for (unsigned int i = 0; i<n_vertices * 3; i++) {
		this->coordinates[i] = cubeVertexPos[i];
		this->normals[i] = cubeVertexNormal[i];
		this->colors[i] = cubeVertexColor[i];
		this->tangents[i] = cubeVertexTangent[i];
	}
	for (unsigned int i = 0; i<n_vertices * 2; i++)
		this->tex_coords[i] = cubeVertexTexCoord[i];
	for (unsigned int i = 0; i<n_triangles * 3; i++)
		this->triangle_indices[i] = cubeTriangleIndex[i];
	//Textures
	this->loadDefaultCubeTextures();
	//Shaders
	switch(shade_mode){
		case PHONG:
			vertex_shader.loadPhongVertexShader();
			fragment_shader.loadPhongFragmentShader();
			break;
		case BLINN_PHONG:
			vertex_shader.loadBlinnPhongVertexShader();
			fragment_shader.loadBlinnPhongFragmentShader();
			break;
		case BUMP:
			vertex_shader.loadPhongBumpVertexShader();
			fragment_shader.loadPhongBumpFragmentShader();
			break;
		case TOON:
			vertex_shader.loadToonVertexShader();
			fragment_shader.loadToonFragmentShader();
			break;
	}
}
/// <summary>
/// This method generate an imported model with default parameters.
/// <param name="filePath">Path of the model.</param>  
/// <param name="shade_mode">Shade mode.</param>  
/// </summary>
void Model::loadAssimpModel(char* filePath, Shade shade_mode)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
	//If importation fails we notify it
	if (!scene)
		std::cout << "Error importing the file: " << (importer.GetErrorString());
	else {
		if (scene->HasMeshes()) {
			aiMesh* myImportedModel = scene->mMeshes[0];
			std::cout << "Model name: " << &myImportedModel->mName.data[2] << std::endl;
			std::cout << "NumFaces: " << myImportedModel->mNumFaces << ", num vertices: " << myImportedModel->mNumVertices << ", num indices(per face): " << myImportedModel->mFaces->mNumIndices << std::endl;
			//Memory reserve for model
			this->n_vertices = myImportedModel->mNumVertices;
			this->n_triangles = myImportedModel->mNumFaces;
			this->coordinates = new float[myImportedModel->mNumVertices * 3];
			this->triangle_indices = new unsigned int[myImportedModel->mNumFaces * 3];
			this->normals = new float[myImportedModel->mNumVertices * 3];
			this->colors = new float[myImportedModel->mNumVertices * 3];
			this->tangents = new float[myImportedModel->mNumVertices * 3];
			this->tex_coords = new float[myImportedModel->mNumVertices * 2];
			//Vertices coordinates & normals
			for (unsigned int i = 0; i<myImportedModel->mNumVertices; i++) {
				this->coordinates[i * 3] = myImportedModel->mVertices[i].x;
				this->coordinates[(i * 3) + 1] = myImportedModel->mVertices[i].y;
				this->coordinates[(i * 3) + 2] = myImportedModel->mVertices[i].z;
				this->normals[i * 3] = myImportedModel->mNormals[i].x;
				this->normals[(i * 3) + 1] = myImportedModel->mNormals[i].y;
				this->normals[(i * 3) + 2] = myImportedModel->mNormals[i].z;
			}
			//Indices
			for (unsigned int i = 0; i<myImportedModel->mNumFaces; i++) {
				this->triangle_indices[i * 3] = myImportedModel->mFaces[i].mIndices[0];
				this->triangle_indices[(i * 3) + 1] = myImportedModel->mFaces[i].mIndices[1];
				this->triangle_indices[(i * 3) + 2] = myImportedModel->mFaces[i].mIndices[2];
			}
			//Textures
			this->loadDefaultAssimpModelTextures();
		}
	}

	//Shaders
	switch (shade_mode) {
	/*case PHONG:
		vertex_shader.loadPhongVertexShader();
		fragment_shader.loadPhongFragmentShader();
		break;
	case BLINN_PHONG:
		vertex_shader.loadBlinnPhongVertexShader();
		fragment_shader.loadBlinnPhongFragmentShader();
		break;
	case BUMP:
		vertex_shader.loadPhongBumpVertexShader();
		fragment_shader.loadPhongBumpFragmentShader();
		break;*/
	case TOON:
		vertex_shader.loadToonVertexShader();
		fragment_shader.loadToonFragmentShader();
		break;
	default:
		vertex_shader.loadToonVertexShader();
		fragment_shader.loadToonFragmentShader();
		break;
	}
}
/// <summary>
/// This method generate default textures for a cube.
/// </summary>
void Model::loadDefaultCubeTextures() {
	this->textures.resize(4);
	for(unsigned int i=0; i<textures.size(); i++)
		this->textures[i].texture_on = true;
	this->textures[0].texture_file = "../resources/img/color2.png";
	this->textures[1].texture_file = "../resources/img/emissive.png";
	this->textures[2].texture_file = "../resources/img/specMap.png";
	this->textures[3].texture_file = "../resources/img/normal.png";
}
/// <summary>
/// This method generate default textures for an Assimp model.
/// </summary>
void Model::loadDefaultAssimpModelTextures() {
	this->textures.resize(4);
	for (unsigned int i = 0; i<textures.size(); i++)
		this->textures[i].texture_on = false;
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
/// <summary>
/// This method update animation.
/// </summary>
void Model::animate()
{
	anim.loadDefaultAnimation();
}
#pragma endregion