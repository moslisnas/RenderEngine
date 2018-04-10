﻿#include "Scene.h"

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

#pragma region Initialization methods
/// <summary>
/// This method generate full scene to be rendered.
/// </summary>
void Scene::create()
{
	this->compileShaders();
	this->compilePrograms();
}
/// <summary>
/// This method create Uniform Buffer Objects.
/// <param name="programId">Program where we create the Uniform Buffer Object.</param> 
/// </summary>
void Scene::createUBOs(int programId)
{
	glUniformBlockBinding(programId, block_directional_lights_id, 4);
	//Loading from light vectors
	float lights_data[3] = { 0.3f, 0.3f, 0.3f };
	/*float* lights_data = new float[directional_lights.size()*12];
	for(unsigned int i=0; i<directional_lights.size(); i++){
		lights_data[i*12] = 1.0f;//directional_lights[i].direction[0];
		lights_data[i*12+1] = 1.0f;//directional_lights[i].direction[1];
		lights_data[i*12+2] = 1.0f;//directional_lights[i].direction[2];
		lights_data[i*12+3] = directional_lights[i].ambiental_intensity[0];
		lights_data[i*12+4] = directional_lights[i].ambiental_intensity[1];
		lights_data[i*12+5] = directional_lights[i].ambiental_intensity[2];
		lights_data[i*12+6] = directional_lights[i].diffuse_intensity[0];
		lights_data[i*12+7] = directional_lights[i].diffuse_intensity[1];
		lights_data[i*12+8] = directional_lights[i].diffuse_intensity[2];
		lights_data[i*12+9] = directional_lights[i].specular_intensity[0];
		lights_data[i*12+10] = directional_lights[i].specular_intensity[1];
		lights_data[i*12+11] = directional_lights[i].specular_intensity[2];
	}*/
	glGenBuffers(1, &buffer_directional_lights_id);
	std::cout << "buffer_directional_lights_id: " << buffer_directional_lights_id << "\n";
	glBindBuffer(GL_UNIFORM_BUFFER, buffer_directional_lights_id);
	std::cout << "sizeof(data): " << sizeof(lights_data); // sizeof(float) * directional_lights.size() * 12;
	glBufferData(GL_UNIFORM_BUFFER, sizeof(lights_data) , &lights_data, GL_DYNAMIC_DRAW); //sizeof(float)*directional_lights.size()*12
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	/*glBindBuffer(GL_UNIFORM_BUFFER, buffer_directional_lights_id);
	GLvoid* p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
	memcpy(p, &lights_data, sizeof(lights_data));//sizeof(float) * directional_lights.size() * 12
	glUnmapBuffer(GL_UNIFORM_BUFFER);*/
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
		block_directional_lights_id = glGetUniformBlockIndex(programs[i], "directionalLights");
		//Uniform Buffer Objects creation
		createUBOs(programs[i]);
		models[i].loadUniforms(programs[i]);
		models[i].loadAttributes(programs[i]);
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
/// <summary>
/// This method add a point light to the scene.
/// <param name="light">The point light that will be added.</param>  
/// </summary>
void Scene::addPointLight(PointLight light)
{
	this->point_lights.push_back(light);
}
/// <summary>
/// This method add a directional light to the scene.
/// <param name="light">The directional light that will be added.</param>  
/// </summary>
void Scene::addDirectionalLight(DirectionalLight light)
{
	this->directional_lights.push_back(light);
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
		bindUBOs(programs[i]);
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
		//POR HACER-->Bucle FOR
		if (models[i].color_texture_on && models[i].fragment_shader.uniform_ids[12] != -1){
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, models[i].color_texture_id);
			glUniform1i(models[i].fragment_shader.uniform_ids[12], 0);
		}
		if (models[i].emissive_texture_on && models[i].fragment_shader.uniform_ids[13] != -1){
			glActiveTexture(GL_TEXTURE0 + 1);
			glBindTexture(GL_TEXTURE_2D, models[i].emissive_texture_id);
			glUniform1i(models[i].fragment_shader.uniform_ids[13], 1);
		}
		if (models[i].specular_texture_on && models[i].fragment_shader.uniform_ids[14] != -1){
			glActiveTexture(GL_TEXTURE0 + 2);
			glBindTexture(GL_TEXTURE_2D, models[i].specular_texture_id);
			glUniform1i(models[i].fragment_shader.uniform_ids[14], 2);
		}
		if (models[i].normal_texture_on && models[i].fragment_shader.uniform_ids[15] != -1){
			glActiveTexture(GL_TEXTURE0 + 3);
			glBindTexture(GL_TEXTURE_2D, models[i].normal_texture_id);
			glUniform1i(models[i].fragment_shader.uniform_ids[15], 3);
		}
		//Lights
		//Points--> POR HACER

		//Directional
		for (unsigned int j=0; j<directional_lights.size(); j++) {
			glm::mat4 light_view = selected_camera.view_matrix * directional_lights[j].light_matrix;
			//POR HACER-->Bucle FOR
			if (models[i].fragment_shader.uniform_ids[0] != -1)
				glUniformMatrix4fv(models[i].fragment_shader.uniform_ids[0], 1, GL_FALSE, &(light_view[0][0]));
			glUniform1i(models[i].fragment_shader.uniform_ids[1], point_lights.size());
			glUniform1i(models[i].fragment_shader.uniform_ids[2], directional_lights.size());
			//glUniform1i(models[i].fragment_shader.uniform_ids[3], directional_lights.size());
			if (models[i].fragment_shader.uniform_ids[8] != -1)
				glUniform3fv(models[i].fragment_shader.uniform_ids[8], 1, &(directional_lights[j].direction[0]));
			if (models[i].fragment_shader.uniform_ids[9] != -1)
				glUniform3fv(models[i].fragment_shader.uniform_ids[9], 1, &(directional_lights[j].ambiental_intensity[0]));
			if (models[i].fragment_shader.uniform_ids[10] != -1)
				glUniform3fv(models[i].fragment_shader.uniform_ids[10], 1, &(directional_lights[j].diffuse_intensity[0]));
			if (models[i].fragment_shader.uniform_ids[11] != -1)
				glUniform3fv(models[i].fragment_shader.uniform_ids[11], 1, &(directional_lights[j].specular_intensity[0]));
		}

		//Focal--> POR HACER

		glDrawElements(GL_TRIANGLES, models[i].n_triangles * 3, GL_UNSIGNED_INT, (void*)0);
	}

	glutSwapBuffers();
}
/// <summary>
/// This method update element properties.
/// </summary>  
void Scene::animate()
{
	static float angle = 0.0f;
	angle = (angle > 3.141592f * 2.0f) ? 0 : angle + 0.01f;
	//Animation cube 1
	models[0].model_matrix = glm::mat4(1.0f);
	models[0].model_matrix = glm::translate(models[0].model_matrix, glm::vec3(-1.5f, -1.25f, 0.0f));
	models[0].model_matrix = glm::rotate(models[0].model_matrix, angle, glm::vec3(1.0f, 1.0f, 0.0f));
	//Animation cube 2
	/*models[1].model_matrix = glm::mat4(1.0f);
	models[1].model_matrix = glm::translate(models[1].model_matrix, glm::vec3(1.5f, -1.25f, 0.0f));
	models[1].model_matrix = glm::rotate(models[1].model_matrix, angle, glm::vec3(1.0f, 1.0f, 0.0f));
	//Animation cube 3
	models[2].model_matrix = glm::mat4(1.0f);
	models[2].model_matrix = glm::translate(models[2].model_matrix, glm::vec3(0.0f, 1.25f, 0.0f));
	models[2].model_matrix = glm::rotate(models[2].model_matrix, angle, glm::vec3(1.0f, 1.0f, 0.0f));*/

	//Render
	glutPostRedisplay();
}
/// <summary>
/// This method bind the Uniform Buffers Objects used on shaders.
/// <param name="programId">Program where we create the Uniform Buffer Object.</param> 
/// </summary>  
void Scene::bindUBOs(int programId)
{
	glBindBufferBase(GL_UNIFORM_BUFFER, 4, buffer_directional_lights_id);
}
/// <summary>
/// This method generate a cube and add it to the scene.
/// </summary>
void Scene::createCubeModel()
{
	Model cube;
	cube.loadDefaultCubeModel(models.size());
	this->addModel(cube);
}
/// <summary>
/// This method generate a model from a file and add it to the scene.
/// <param name="filePath">Path of the model file.</param>  
/// </summary>
void Scene::createAssimpModel(char * filePath)
{
	Model assimp_model;
	assimp_model.loadAssimpModel(filePath);
	this->addModel(assimp_model);
}
/// <summary>
/// This method generate a point light and add it to the scene.
/// </summary>
void Scene::createPointLight()
{
	PointLight point_light;
	//point_light.setType(POINT_LIGHT);
	point_light.loadDefault();
	this->addPointLight(point_light);
}
/// <summary>
/// This method generate a directional light and add it to the scene.
/// </summary>
void Scene::createDirectionalLight()
{
	DirectionalLight directional_light;
	//directional_light.setType(DIRECTIONAL_LIGHT);
	directional_light.loadDefault();
	this->addDirectionalLight(directional_light);
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
}
#pragma endregion