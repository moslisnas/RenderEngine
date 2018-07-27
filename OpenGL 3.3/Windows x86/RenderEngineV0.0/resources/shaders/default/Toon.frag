#version 330 core

#define MAX_NUM_TOTAL_LIGHTS 100 //Total of 300: 100 points + 100 directionals + 100 focals
out vec4 outColor;

in vec3 color;
in vec3 pos;
in vec3 norm;
in vec2 texCoord;

//Textures
uniform sampler2D colorTex;
uniform sampler2D emiTex;
uniform sampler2D specularTex;

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

//Object properties
vec3 Ka;
vec3 Kd;
vec3 Ks;
vec3 N;
float alpha = 50.0;
vec3 Ke;

int colorLOD = 6;
float shapeThickness = 0.3;
float scaleFactor = 1.0/colorLOD;

vec3 shade()
{
	vec3 c = vec3(0.0);

	//Ambient
	c += ambientIntensity * Ka;
	vec3 V = normalize (-pos);

	//Point Lights
	for(int i=0; i<numPointLights; i++){
		//Position
		vec3 lightPosition = vec3(1.0);
		lightPosition = (lightView * vec4(p_lights[i].p_light_position, 1.0)).xyz;
		//Diffuse
		vec3 L = normalize (lightPosition - pos);
		float diffuse = max(0.0, dot(L,N));
		float specular = 0.0;
		float specMask;
		vec3 Id = p_lights[i].p_light_diffuse_intensity * Kd * floor(diffuse*colorLOD) * scaleFactor;
		//Specular
		vec3 R = reflect (-L,N);
		if(dot(N,L) > 0.0)
			specular = dot(p_lights[i].p_light_specular_intensity,p_lights[i].p_light_specular_intensity) * pow(max(0.0, dot(R,V)), alpha);
		specMask = (pow(dot(R,V), alpha) > 0.4) ? 1 : 0;
		vec3 Is = vec3(specular*specMask);
		c += Id + Is;
	}

	//Directional Lights
	for(int i=0; i<numDirectionalLights; i++){
		//Diffuse
		vec3 L = normalize(d_lights[i].d_light_direction);
		float diffuse = max(0.0, dot(L,N));
		float specular = 0.0;
		float specMask;
		vec3 Id = d_lights[i].d_light_diffuse_intensity * Kd * floor(diffuse*colorLOD) * scaleFactor;
		//Specular
		vec3 R = reflect (-L,N);
		if(dot(N,L) > 0.0)
			specular = dot(d_lights[i].d_light_specular_intensity,d_lights[i].d_light_specular_intensity) * pow(max(0.0, dot(R,V)), alpha);
		specMask = (pow(dot(R,V), alpha) > 0.4) ? 1 : 0;
		vec3 Is = vec3(specular*specMask);
		c += Id + Is;
	}

	//Focal Lights
	for(int i=0; i<numFocalLights; i++){
		//Position
		vec3 lightPosition = vec3(1.0);
		lightPosition = (lightView * vec4(f_lights[i].f_light_position, 1.0)).xyz;

		//Diffuse
		vec3 L = normalize (lightPosition - pos);
		float diffuse = max(0.0, dot(L,N));
		float specular = 0.0;
		float specMask;
		vec3 Id = f_lights[i].f_light_diffuse_intensity * Kd * max(0.0, dot(L,N)) * floor(diffuse*colorLOD) * scaleFactor;
		float Ip = 0;
		float cosineAngle = cos(f_lights[i].f_apperture_angle);
		if(dot(f_lights[0].f_light_direction,-L) > cosineAngle){
			Ip = pow((dot(f_lights[0].f_light_direction, -L) - cosineAngle) / (1-cosineAngle), 0.5);
		}
		Id = clamp(Ip*Id, 0.0, 1.0);

		//Specular
		vec3 R = reflect (-L,N);
		if(dot(N,L) > 0.0)
			specular = dot(f_lights[i].f_light_specular_intensity,f_lights[i].f_light_specular_intensity) * pow(max(0.0, dot(R,V)), alpha);
		specMask = (pow(dot(R,V), alpha) > 0.4) ? 1 : 0;
		vec3 Is = vec3(Ip*specular*specMask);
		c += Id + Is;
	}

	//Shape.
	float nDotv = dot(N,V);
	float nMod = dot(N,N);
	float vMod = dot(V,V);
	//If cosin is lower than right term, then the fragment is on shape
	if((nDotv/(nMod*vMod)) < shapeThickness)
		c = vec3(0.0);

	return c;
}

void main()
{	
	//Object properties
	Ka = vec3(1.0, 0.0, 0.0);
	Kd = Ka;
	Ks = vec3(0.2);

	N = normalize(norm);

	outColor = vec4(shade(), 1.0);
}