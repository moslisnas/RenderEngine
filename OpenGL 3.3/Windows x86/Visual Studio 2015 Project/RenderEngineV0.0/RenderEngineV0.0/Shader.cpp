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

#pragma region Shaders loading methods POR HACER-->ARREGLAR LO DE VARIAS LUCES
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
	this->num_uniforms = 13;
	this->uniform_ids = new unsigned int[num_uniforms];
	char* aux = new char[9]; aux = "lightView";
	this->uniform_names.push_back(aux);
	aux = new char[4]; aux = "lPos";
	this->uniform_names.push_back(aux);
	aux = new char[5]; aux = "lIntA";
	this->uniform_names.push_back(aux);
	aux = new char[5]; aux = "lIntD";
	this->uniform_names.push_back(aux);
	aux = new char[5]; aux = "lIntS";
	this->uniform_names.push_back(aux);
	aux = new char[4]; aux = "lDir";
	this->uniform_names.push_back(aux);
	aux = new char[6]; aux = "lIntA2";
	this->uniform_names.push_back(aux);
	aux = new char[6]; aux = "lIntD2";
	this->uniform_names.push_back(aux);
	aux = new char[6]; aux = "lIntS2";
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
/// This method load a vertex shader for classic Phong ilumination method.
/// </summary>
void Shader::loadPhongVertexShader()
{
	this->file_path = "../resources/shaders/default/Phong.vert";
	this->type = GL_VERTEX_SHADER;
	loadCommonUniformsVertex();
	loadCommonAttributesVertex();
}
/// <summary>
/// This method load a fragment shader for classic Phong ilumination method.
/// </summary>
void Shader::loadPhongFragmentShader()
{
	this->file_path = "../resources/shaders/default/Phong.frag";
	this->type = GL_FRAGMENT_SHADER;
	loadCommonUniformsFragment();
}
#pragma endregion

#pragma endregion