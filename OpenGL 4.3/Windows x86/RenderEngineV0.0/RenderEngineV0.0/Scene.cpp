#include "Scene.h"

#pragma region Constructor & destructor
/// <summary>
/// Constructor of <c>Scene</c> class.
/// </summary>
Scene::Scene()
{
	this->ambient_lighting[0] = 0.3f;
	this->ambient_lighting[1] = 0.3f;
	this->ambient_lighting[2] = 0.3f;
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
/// <summary>
/// Setter of <c>ambient_lighting</c> variable.
/// <param name="rgb">Value used for lighting ambient component on the scene.</param> 
/// </summary>
void Scene::setAmbientLighting(float rgb)
{
	this->ambient_lighting[0] = rgb;
	this->ambient_lighting[1] = rgb;
	this->ambient_lighting[2] = rgb;
}
/// <summary>
/// Setter of <c>ambient_lighting</c> variable.
/// <param name="r">The component R of the color intensity.</param>
/// <param name="g">The component G of the color intensity.</param> 
/// <param name="b">The component B of the color intensity.</param> 
/// </summary>
void Scene::setAmbientLighting(float r, float g, float b)
{
	this->ambient_lighting[0] = r;
	this->ambient_lighting[1] = g;
	this->ambient_lighting[2] = b;
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
/// This method create and assign Uniform Buffer Objects to a program.
/// <param name="programId">Program where we create the Uniform Buffer Objects.</param> 
/// </summary>
void Scene::createUBOs(int programId)
{
	block_point_lights_id = glGetUniformBlockIndex(programId, "pointLights");
	block_directional_lights_id = glGetUniformBlockIndex(programId, "directionalLights");
	block_focal_lights_id = glGetUniformBlockIndex(programId, "focalLights");

	if(block_point_lights_id != -1){
		glUniformBlockBinding(programId, block_point_lights_id, 0);
		//Loading from light vectors
		glGenBuffers(1, &buffer_point_lights_id);
		glBindBuffer(GL_UNIFORM_BUFFER, buffer_point_lights_id);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(float) * 12 * point_lights.size(), 0, GL_DYNAMIC_DRAW);
		int offset = 0;
		for (unsigned int i=0; i<point_lights.size(); i++) {
			glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float) * 3, point_lights[i].position);
			offset += 16;
			glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float) * 3, point_lights[i].diffuse_intensity);
			offset += 16;
			glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float) * 3, point_lights[i].specular_intensity);
			offset += 16;
		}
	}
	if(block_directional_lights_id != -1){
		glUniformBlockBinding(programId, block_directional_lights_id, 1);
		//Loading from light vectors
		glGenBuffers(1, &buffer_directional_lights_id);
		glBindBuffer(GL_UNIFORM_BUFFER, buffer_directional_lights_id);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(float) * 12 * directional_lights.size() , 0, GL_DYNAMIC_DRAW);
		int offset = 0;
		for (unsigned int i=0; i<directional_lights.size(); i++) {
			glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float)*3, directional_lights[i].direction);
			offset += 16;
			glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float)*3, directional_lights[i].diffuse_intensity);
			offset += 16;
			glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float)*3, directional_lights[i].specular_intensity);
			offset += 16;
		}
	}
	if(block_focal_lights_id != -1) {
		glUniformBlockBinding(programId, block_focal_lights_id, 2);
		//Loading from light vectors
		glGenBuffers(1, &buffer_focal_lights_id);
		glBindBuffer(GL_UNIFORM_BUFFER, buffer_focal_lights_id);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(float) * 24 * focal_lights.size(), 0, GL_DYNAMIC_DRAW);
		int offset = 0;
		for (unsigned int i=0; i<focal_lights.size(); i++) {
			glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float) * 3, focal_lights[i].position);
			offset += 16;
			glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float) * 3, focal_lights[i].direction);
			offset += 16;
			glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float) * 3, focal_lights[i].diffuse_intensity);
			offset += 16;
			glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float) * 3, focal_lights[i].specular_intensity);
			offset += 12;
			glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float), &focal_lights[i].apperture_angle);
			offset += 4;
			glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float), &focal_lights[i].attenuation);
			offset += 16;
		}
	}
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
/// <summary>
/// This method compile all model vertex/fragment shaders.
/// </summary>
void Scene::compileShaders()
{
	this->num_programs = models.size();
	//We use same program for diferent models with same shaders 
	/*for (unsigned int i=0; i<numModels-1; i++) {
		for (int j = i; j < numModels; j++)
			if (models[i].vertexShader.filePath == models[i].vertexShader.filePath)
				numPrograms--;
	}*/
	for (unsigned int i=0; i<num_programs; i++) {
		this->models[i].vertex_shader.id = loadShader(models[i].vertex_shader.file_path, models[i].vertex_shader.type);
		this->models[i].geometry_shader.id = loadShader(models[i].geometry_shader.file_path, models[i].geometry_shader.type); //POR HACER
		this->models[i].fragment_shader.id = loadShader(models[i].fragment_shader.file_path, models[i].fragment_shader.type);
	}
}
/// <summary>
/// This method compile and link all model programs.
/// </summary>
void Scene::compilePrograms()
{
	this->programs = new unsigned int[num_programs];
	for(unsigned int i=0; i<num_programs; i++){
		this->programs[i] = glCreateProgram();
		//Deprecated attrib binding
		/*for (unsigned int j=0; j<models[i].vertexShader.numAttribs; j++)
		glBindAttribLocation(programs[i], j, models[i].vertexShader.attribNames[j]);*/
		glAttachShader(this->programs[i], models[i].vertex_shader.id);
		//glAttachShader(this->programs[i], models[i].geometry_shader.id); //POR HACER
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
/// <summary>
/// This method add a focal light to the scene.
/// <param name="light">The focal light that will be added.</param>  
/// </summary>
void Scene::addFocalLight(FocalLight light)
{
	this->focal_lights.push_back(light);
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
		int uniform_texture_index = models[i].fragment_shader.texture_start_index;
		for (unsigned int j=0; j<models[i].textures.size(); j++) {
			if (models[i].textures[j].texture_on && models[i].fragment_shader.uniform_ids[uniform_texture_index + j] != -1) {
				glActiveTexture(GL_TEXTURE0 + j);
				glBindTexture(GL_TEXTURE_2D, models[i].textures[j].texture_id);
				glUniform1i(models[i].fragment_shader.uniform_ids[uniform_texture_index + j], j);
			}
		}

		//Lights
		glUniform3fv(models[i].fragment_shader.uniform_ids[0], 1, &ambient_lighting[0]);
		//Points
		for (unsigned int j=0; j<point_lights.size(); j++) {
			glm::mat4 light_view = selected_camera.view_matrix * point_lights[j].light_matrix;
			if (models[i].fragment_shader.uniform_ids[1] != -1)
				glUniformMatrix4fv(models[i].fragment_shader.uniform_ids[1], 1, GL_FALSE, &(light_view[0][0]));
			if (models[i].fragment_shader.uniform_ids[2] != -1)
				glUniform1i(models[i].fragment_shader.uniform_ids[2], point_lights.size());
		}
		//Directional
		for (unsigned int j=0; j<directional_lights.size(); j++) {
			glm::mat4 light_view = selected_camera.view_matrix * directional_lights[j].light_matrix;
			if (models[i].fragment_shader.uniform_ids[1] != -1)
				glUniformMatrix4fv(models[i].fragment_shader.uniform_ids[1], 1, GL_FALSE, &(light_view[0][0]));
			if (models[i].fragment_shader.uniform_ids[3] != -1)
				glUniform1i(models[i].fragment_shader.uniform_ids[3], directional_lights.size());
		}
		//Focal
		for (unsigned int j=0; j<focal_lights.size(); j++) {
			glm::mat4 light_view = selected_camera.view_matrix * focal_lights[j].light_matrix;
			if (models[i].fragment_shader.uniform_ids[1] != -1)
				glUniformMatrix4fv(models[i].fragment_shader.uniform_ids[1], 1, GL_FALSE, &(light_view[0][0]));
			if (models[i].fragment_shader.uniform_ids[4] != -1)
				glUniform1i(models[i].fragment_shader.uniform_ids[4], focal_lights.size());
		}

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
	angle = (angle > NUMBER_PI * 2.0f) ? 0 : angle + 0.01;
	for(unsigned int i=0; i<models.size(); i++){
		models[i].model_matrix = glm::mat4(1.0f);
		models[i].model_matrix = glm::translate(models[i].model_matrix, models[i].position);
		models[i].model_matrix = glm::rotate(models[i].model_matrix, models[i].rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		models[i].model_matrix = glm::rotate(models[i].model_matrix, models[i].rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		models[i].model_matrix = glm::rotate(models[i].model_matrix, models[i].rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		models[i].animate();
	}

	//Render
	glutPostRedisplay();
}
/// <summary>
/// This method update scene elements when a key of the keyboard is pressed.
/// <param name="key">Character of the pressed key.</param>  
/// <param name="x">X position of the mouse cursor when button was pressed.</param>
/// <param name="y">Y position of the mouse cursor when button was pressed.</param>
/// </summary>  
void Scene::keyboardInteraction(unsigned char key, int x, int y) {
	float keyboardAngle = NUMBER_PI / 12.0f;
	//Traslation keys
	if (key == 'w') {
		selected_camera.view_matrix = glm::rotate(selected_camera.view_matrix, selected_camera.rotation[1], glm::vec3(0.0f, 1.0f, 0.0f));
		selected_camera.view_matrix = glm::translate(selected_camera.view_matrix, glm::vec3(0.0f, 0.0f, 1.0f));
		selected_camera.view_matrix = glm::rotate(selected_camera.view_matrix, -selected_camera.rotation[1], glm::vec3(0.0f, 1.0f, 0.0f));
		selected_camera.position[2] -= 1.0f * cos(selected_camera.rotation[1]);
		selected_camera.position[0] -= 1.0f * sin(selected_camera.rotation[1]);
	}
	else if (key == 's') {
		selected_camera.view_matrix = glm::rotate(selected_camera.view_matrix, selected_camera.rotation[1], glm::vec3(0.0f, 1.0f, 0.0f));
		selected_camera.view_matrix = glm::translate(selected_camera.view_matrix, glm::vec3(0.0f, 0.0f, -1.0f));
		selected_camera.view_matrix = glm::rotate(selected_camera.view_matrix, -selected_camera.rotation[1], glm::vec3(0.0f, 1.0f, 0.0f));
		selected_camera.position[2] += 1.0f * cos(selected_camera.rotation[1]);
		selected_camera.position[0] += 1.0f * sin(selected_camera.rotation[1]);
	}
	else if (key == 'a') {
		selected_camera.view_matrix = glm::rotate(selected_camera.view_matrix, selected_camera.rotation[1], glm::vec3(0.0f, 1.0f, 0.0f));
		selected_camera.view_matrix = glm::translate(selected_camera.view_matrix, glm::vec3(1.0f, 0.0f, 0.0f));
		selected_camera.view_matrix = glm::rotate(selected_camera.view_matrix, -selected_camera.rotation[1], glm::vec3(0.0f, 1.0f, 0.0f));
		selected_camera.position[0] -= 1.0f * cos(selected_camera.rotation[1]);
		selected_camera.position[2] += 1.0f * sin(selected_camera.rotation[1]);
	}
	else if (key == 'd') {
		selected_camera.view_matrix = glm::rotate(selected_camera.view_matrix, selected_camera.rotation[1], glm::vec3(0.0f, 1.0f, 0.0f));
		selected_camera.view_matrix = glm::translate(selected_camera.view_matrix, glm::vec3(-1.0f, 0.0f, 0.0f));
		selected_camera.view_matrix = glm::rotate(selected_camera.view_matrix, -selected_camera.rotation[1], glm::vec3(0.0f, 1.0f, 0.0f));
		selected_camera.position[1] += 1.0f * cos(selected_camera.rotation[1]);
		selected_camera.position[2] -= 1.0f * sin(selected_camera.rotation[1]);
	}
	//Rotation keys
	else if (key == 'e') {
		selected_camera.view_matrix = glm::translate(selected_camera.view_matrix, glm::vec3(selected_camera.position[0], 0.0f, selected_camera.position[2]));
		selected_camera.view_matrix = glm::rotate(selected_camera.view_matrix, keyboardAngle, glm::vec3(0.0f, 1.0f, 0.0f));
		selected_camera.view_matrix = glm::translate(selected_camera.view_matrix, glm::vec3(-selected_camera.position[0], 0.0f, -selected_camera.position[2]));
		selected_camera.rotation[1] -= keyboardAngle;
	}
	else if (key == 'q') {
		selected_camera.view_matrix = glm::translate(selected_camera.view_matrix, glm::vec3(selected_camera.position[0], 0.0f, selected_camera.position[2]));
		selected_camera.view_matrix = glm::rotate(selected_camera.view_matrix, keyboardAngle, glm::vec3(0.0f, -1.0f, 0.0f));
		selected_camera.view_matrix = glm::translate(selected_camera.view_matrix, glm::vec3(-selected_camera.position[0], 0.0f, -selected_camera.position[2]));
		selected_camera.rotation[1] += keyboardAngle;
	}
	/*GIMBAL LOCK
	else if (key == 'z') {
		view = glm::rotate(view, keyboardAngle, glm::vec3(1.0f, 0.0f, 0.0f));
		rotationX -= keyboardAngle;
	}
	else if (key == 'x') {
		view = glm::rotate(view, keyboardAngle, glm::vec3(-1.0f, 0.0f, 0.0f));
		rotationX += keyboardAngle;
	}*/
}
/// <summary>
/// This method bind the Uniform Buffers Objects used on shaders.
/// <param name="programId">Program where we create the Uniform Buffer Object.</param> 
/// </summary>
void Scene::bindUBOs(int programId)
{
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, buffer_point_lights_id);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, buffer_directional_lights_id);
	glBindBufferBase(GL_UNIFORM_BUFFER, 2, buffer_focal_lights_id);
}
/// <summary>
/// This method generate a cube and add it to the scene.
/// <param name="shade_mode">Shade mode.</param>
/// </summary>
void Scene::createCubeModel(Shade shade_mode)
{
	Model cube;
	cube.loadDefaultCubeModel(shade_mode);
	this->addModel(cube);
}
/// <summary>
/// This method generate a model from a file and add it to the scene.
/// <param name="filePath">Path of the model file.</param>
/// <param name="shade_mode">Shade mode.</param>
/// </summary>
void Scene::createAssimpModel(char * filePath, Shade shade_mode)
{
	Model assimp_model;
	assimp_model.loadAssimpModel(filePath, shade_mode);
	this->addModel(assimp_model);
}
/// <summary>
/// This method generate a point light and add it to the scene.
/// </summary>
void Scene::createPointLight()
{
	PointLight point_light;
	point_light.loadDefault();
	this->addPointLight(point_light);
}
/// <summary>
/// This method generate a directional light and add it to the scene.
/// </summary>
void Scene::createDirectionalLight()
{
	DirectionalLight directional_light;
	directional_light.loadDefault();
	this->addDirectionalLight(directional_light);
}
/// <summary>
/// This method generate a focal light and add it to the scene.
/// </summary>
void Scene::createFocalLight()
{
	FocalLight focal_light;
	focal_light.loadDefault();
	this->addFocalLight(focal_light);
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