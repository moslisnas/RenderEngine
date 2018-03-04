#version 330 core

layout(location = 0) in vec3 inPos;	
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inNormal;
layout(location = 4) in vec3 inTangent;

uniform mat4 modelViewProj;
uniform mat4 modelView;
uniform mat4 normal;

out vec3 color;
out vec3 pos;
out vec3 norm;
out vec2 texCoord;
out mat3 tbn;

void main()
{
	pos = (modelView * vec4(inPos, 1.0)).xyz;
	color = inColor;
	texCoord = inTexCoord;
	norm = normalize((normal * vec4(inNormal,0.0)).xyz);

	vec3 tang =  normalize((modelView * vec4(inTangent,0.0)).xyz);
	vec3 biTang = normalize(cross(norm, tang));
	tbn = transpose(mat3(tang,biTang,norm));
	
	gl_Position = modelViewProj * vec4 (inPos,1.0);
}