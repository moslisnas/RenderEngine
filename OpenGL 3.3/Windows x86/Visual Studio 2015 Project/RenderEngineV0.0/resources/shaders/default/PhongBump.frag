#version 330 core

#define MAX_NUM_TOTAL_LIGHTS 100 //Total of 300: 100 points + 100 directionals + 100 focals
out vec4 outColor;

in vec3 color;
in vec3 pos;
in vec3 norm;
in vec2 texCoord;
in mat3 tbn;

uniform sampler2D colorTex;
uniform sampler2D emiTex;
uniform sampler2D specularTex;
uniform sampler2D normTex;

//Lights
struct PointLight{
	vec3 p_light_position;
	vec3 p_light_diffuse_intensity;
	vec3 p_light_specular_intensity;
};
layout (std140) uniform pointLights{ //binding=0 OpenGL 4.3
	PointLight p_lights[MAX_NUM_TOTAL_LIGHTS];
};
struct DirectionalLight{
	vec3 d_light_direction;
	vec3 d_light_diffuse_intensity;
	vec3 d_light_specular_intensity;
};
layout(std140) uniform directionalLights{ //binding=1 OpenGL 4.3
	DirectionalLight d_lights[MAX_NUM_TOTAL_LIGHTS];
};
struct FocalLight{
	vec3 f_light_position;
	vec3 f_light_direction;
	vec3 f_light_diffuse_intensity;
	vec3 f_light_specular_intensity;
	float f_apperture_angle;
	float f_attenuation;
};
layout(std140) uniform focalLights{ //binding=2 OpenGL 4.3
	FocalLight f_lights[MAX_NUM_TOTAL_LIGHTS];
};

uniform vec3 ambientIntensity;
uniform int numPointLights;
uniform int numDirectionalLights;
uniform int numFocalLights;
uniform mat4 lightView;


//Luces
uniform mat4 lightView;
uniform vec3 lPos;
uniform vec3 lIntA;
uniform vec3 lIntD;
uniform vec3 lIntS;
uniform mat4 lightView2;
uniform vec3 lDir;
uniform vec3 lIntA2;
uniform vec3 lIntD2;
uniform vec3 lIntS2;

//Object properties
vec3 Ka;
vec3 Kd;
vec3 Ks;
vec3 N;
float alpha = 5000.0;
vec3 Ke;

vec3 shade()
{
	vec3 c = vec3(0.0);
	
    vec3 EyeDirection_tangentspace = normalize(tbn * (-pos));
	//Luz 1
	c = lIntA * Ka;
	vec3 lightPosition = vec3(1.0);
	lightPosition = (lightView * vec4(lPos, 1.0)).xyz; //Luz con posici�n invariante
	vec3 LightPosition_tangentspace = normalize(tbn * lightPosition);
	vec3 L = normalize (lightPosition - pos);
	vec3 diffuse = lIntD * Kd * dot (L,N);
	c += clamp(diffuse, 0.0, 1.0);
	
	vec3 V = normalize (-pos);
	vec3 R = normalize (reflect (-L,N));
	float factor = max (dot (R,V), 0.01);
	vec3 specular = lIntS*Ks*pow(factor,alpha);
	c += clamp(specular, 0.0, 1.0);

	//Luz 2
	c += lIntA2 * Ka;
	vec3 LightDirection_tangentspace = normalize(tbn * lDir);
	vec3 L2 = normalize (LightDirection_tangentspace);
	vec3 diffuse2 = lIntD2 * Kd * dot (L2,N);
	c += clamp(diffuse2, 0.0, 1.0);
	
	vec3 V2 = normalize (EyeDirection_tangentspace);
	vec3 R2 = normalize (reflect (-L2,N));
	float factor2 = max (dot (R2,V2), 0.01);
	vec3 specular2 = lIntS2*Ks*pow(factor2,alpha);
	c += clamp(specular2, 0.0, 1.0);

	c+=Ke;
	
	return c;
}

void main()
{
	Ka = texture(colorTex, texCoord).rgb;
	Kd = Ka;
	Ks = texture(specularTex, texCoord).rgb;
	Ke = texture(emiTex, texCoord).rgb;
	
	N = normalize(texture(normTex, texCoord).rgb*2.0 - 1.0);
	
	outColor = vec4(shade(), 1.0);
}