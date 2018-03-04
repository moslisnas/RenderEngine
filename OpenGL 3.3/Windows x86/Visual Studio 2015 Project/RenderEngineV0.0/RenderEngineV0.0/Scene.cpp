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

#pragma region Initialization methods POR HACER --> Añadir a la clase model la carga de uniforms y atributos.
/// <summary>
/// This method generate full scene to be rendered.
/// </summary>
void Scene::create()
{
	this->compileShaders();
	this->compilePrograms();
}
/// <summary>
/// This method compile all model vertex/fragment shaders.
/// </summary>
void Scene::compileShaders()
{
	this->num_programs = models.size();
	//POR HACER--> We use same program for diferent models with same shaders 
	/*for (unsigned int i=0; i<numModels-1; i++) {
	for (int j = i; j < numModels; j++)
	if (models[i].vertexShader.filePath == models[i].vertexShader.filePath)
	numPrograms--;
	}*/
	for (unsigned int i = 0; i<num_programs; i++) {
		this->models[i].vertex_shader.id = loadShader(models[i].vertex_shader.file_path, models[i].vertex_shader.type);
		this->models[i].fragment_shader.id = loadShader(models[i].fragment_shader.file_path, models[i].fragment_shader.type);
	}
}
/// <summary>
/// This method compile and link all model programs.
/// </summary>
void Scene::compilePrograms()
{
	this->programs = new unsigned int[num_programs];
	for (unsigned int i = 0; i<num_programs; i++) {
		this->programs[i] = glCreateProgram();
		//Deprecated attrib binding
		/*for (unsigned int j=0; j<models[i].vertexShader.numAttribs; j++)
		glBindAttribLocation(programs[i], j, models[i].vertexShader.attribNames[j]);*/
		glAttachShader(this->programs[i], models[i].vertex_shader.id);
		glAttachShader(this->programs[i], models[i].fragment_shader.id);
		glLinkProgram(this->programs[i]);
		//Error checks
		int linked;
		glGetProgramiv(programs[i], GL_LINK_STATUS, &linked);
		if (!linked)
		{
			GLint logLen;
			glGetProgramiv(programs[i], GL_INFO_LOG_LENGTH, &logLen);
			char *logString = new char[logLen];
			glGetProgramInfoLog(programs[i], logLen, NULL, logString);
			std::cout << "Error: " << logString << std::endl;
			delete[] logString;
			glDeleteProgram(programs[i]);
			programs[i] = 0;
			exit(-1);
		}

		//Uniform variables POR HACER --> Añadir a la clase model
		for (unsigned int j = 0; j<models[i].vertex_shader.num_uniforms; j++)
			models[i].vertex_shader.uniform_ids[j] = glGetUniformLocation(programs[i], models[i].vertex_shader.uniform_names[j]);
		for (unsigned int j = 0; j<models[i].fragment_shader.num_uniforms; j++)
			models[i].fragment_shader.uniform_ids[j] = glGetUniformLocation(programs[i], models[i].fragment_shader.uniform_names[j]);
		//Basic attributes
		for (unsigned int j = 0; j<models[i].vertex_shader.num_attribs; j++)
			models[i].vertex_shader.attrib_ids[j] = glGetAttribLocation(programs[i], models[i].vertex_shader.attrib_names[j]);
	}
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

	//Models
	for(unsigned int i=0; i<models.size(); i++){
		glUseProgram(programs[i]);
		models[i].bind();
		glBindVertexArray(models[i].vao_id);

		glm::mat4 model_view = selected_camera.view_matrix * models[i].model_matrix;
		glm::mat4 model_view_projection = selected_camera.projection_matrix * selected_camera.view_matrix * models[i].model_matrix;
		glm::mat4 normal = glm::transpose(glm::inverse(model_view));
		if (models[i].vertex_shader.uniform_ids[0] != -1)
			glUniformMatrix4fv(models[i].vertex_shader.uniform_ids[0], 1, GL_FALSE, &(model_view[0][0]));
		if (models[i].vertex_shader.uniform_ids[1] != -1)
			glUniformMatrix4fv(models[i].vertex_shader.uniform_ids[1], 1, GL_FALSE, &(model_view_projection[0][0]));
		if (models[i].vertex_shader.uniform_ids[2] != -1)
			glUniformMatrix4fv(models[i].vertex_shader.uniform_ids[2], 1, GL_FALSE, &(normal[0][0]));

		//Textures

		glDrawElements(GL_TRIANGLES, models[i].n_triangles * 3, GL_UNSIGNED_INT, (void*)0);
	}

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

#pragma region Loading methods
/// <summary>
/// This method compile OpenGL shader from a file.
/// <param name="fileName">Path of the file.</param>  
/// <param name="type">Type os shader (vertices or fragments).</param>
/// <returns>OpenGL shader id.</returns> 
/// </summary>
GLuint Scene::loadShader(const char *fileName, GLenum type)
{
	unsigned int fileLen;
	char *source = loadStringFromFile(fileName, fileLen);
	//Creation and compilation of the shader.
	GLuint shader;
	shader = glCreateShader(type);
	glShaderSource(shader, 1, (const GLchar **)&source, (const GLint *)&fileLen);
	glCompileShader(shader);
	delete[] source;

	//Error checks.
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		GLint logLen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
		char *logString = new char[logLen];
		glGetShaderInfoLog(shader, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete[] logString;
		glDeleteShader(shader);
		exit(-1);
	}

	return shader;
	return 0;
}
#pragma endregion