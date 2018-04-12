#include "Shader.h"

#pragma region Constructor & destructor
/// <summary>
/// Constructor of <c>Shader</c> class.
/// </summary>
Shader::Shader()
{
	this->file_path = NULL;
	this->num_uniforms = 0;
	this->num_attribs = 0;
}
/// <summary>
/// Destructor of <c>Shader</c> class.
/// </summary>
Shader::~Shader()
{
}
#pragma endregion

#pragma region Shaders loading methods
/// <summary>
/// This method load more usual uniform variables for vertex shader.
/// </summary>
void Shader::loadCommonUniformsVertex()
{
	this->num_uniforms = 3;
	this->uniform_ids = new unsigned int[num_uniforms];
	char* aux = new char[9]; aux = "modelView";
	this->uniform_names.push_back(aux);
	aux = new char[13]; aux = "modelViewProj";
	this->uniform_names.push_back(aux);
	aux = new char[6]; aux = "normal";
	this->uniform_names.push_back(aux);
}
/// <summary>
/// This method load more usual attributes for vertex shader.
/// </summary>
void Shader::loadCommonAttributesVertex()
{
	this->num_attribs = 5;
	this->attrib_ids = new unsigned int[num_attribs];
	char* aux = new char[5]; aux = "inPos";
	this->attrib_names.push_back(aux);
	aux = new char[7]; aux = "inColor";
	this->attrib_names.push_back(aux);
	aux = new char[8]; aux = "inNormal";
	this->attrib_names.push_back(aux);
	aux = new char[9]; aux = "inTangent";
	this->attrib_names.push_back(aux);
	aux = new char[10]; aux = "inTexCoord";
	this->attrib_names.push_back(aux);
}
/// <summary>
/// This method load more usual uniform variables for fragment shader.
/// </summary>
void Shader::loadCommonUniformsFragment()
{
	this->num_uniforms = 9;
	this->uniform_ids = new unsigned int[num_uniforms];
	char* aux = new char[16]; aux = "ambientIntensity";
	this->uniform_names.push_back(aux);
	aux = new char[9]; aux = "lightView";
	this->uniform_names.push_back(aux);
	aux = new char[14]; aux = "numPointLights";
	this->uniform_names.push_back(aux);
	aux = new char[19]; aux = "numDirectionalLights";
	this->uniform_names.push_back(aux);
	aux = new char[14]; aux = "numFocalLights";
	this->uniform_names.push_back(aux);
	aux = new char[8]; aux = "colorTex";
	this->uniform_names.push_back(aux);
	aux = new char[6]; aux = "emiTex";
	this->uniform_names.push_back(aux);
	aux = new char[10]; aux = "specularTex";
	this->uniform_names.push_back(aux);
	aux = new char[7]; aux = "normTex";
	this->uniform_names.push_back(aux);
}

#pragma region Default shaders
/// <summary>
/// This method load a vertex shader for classic Phong shading method.
/// </summary>
void Shader::loadPhongVertexShader()
{
	this->file_path = "../resources/shaders/default/Phong.vert";
	this->type = GL_VERTEX_SHADER;
	loadCommonUniformsVertex();
	loadCommonAttributesVertex();
}
/// <summary>
/// This method load a fragment shader for classic Phong shading method.
/// </summary>
void Shader::loadPhongFragmentShader()
{
	this->file_path = "../resources/shaders/default/Phong.frag";
	this->type = GL_FRAGMENT_SHADER;
	loadCommonUniformsFragment();
}
/// <summary>
/// This method load a vertex shader for a bumped Phong shading method.
/// </summary>
void Shader::loadPhongBumpVertexShader()
{
	this->file_path = "../resources/shaders/default/PhongBump.vert";
	this->type = GL_VERTEX_SHADER;
	loadCommonUniformsVertex();
	loadCommonAttributesVertex();
}
/// <summary>
/// This method load a fragment shader for a bumped Phong shading method.
/// </summary>
void Shader::loadPhongBumpFragmentShader()
{
	this->file_path = "../resources/shaders/default/PhongBump.frag";
	this->type = GL_FRAGMENT_SHADER;
	loadCommonUniformsFragment();
}
/// <summary>
/// This method load a vertex shader for a toon shading method.
/// </summary>
void Shader::loadToonVertexShader()
{
	this->file_path = "../resources/shaders/default/Toon.vert";
	this->type = GL_VERTEX_SHADER;
	loadCommonUniformsVertex();
	loadCommonAttributesVertex();
}
/// <summary>
/// This method load a fragment shader for a toon shading method.
/// </summary>
void Shader::loadToonFragmentShader()
{
	this->file_path = "../resources/shaders/default/Toon.frag";
	this->type = GL_FRAGMENT_SHADER;
	loadCommonUniformsFragment();
}
#pragma endregion

#pragma endregion