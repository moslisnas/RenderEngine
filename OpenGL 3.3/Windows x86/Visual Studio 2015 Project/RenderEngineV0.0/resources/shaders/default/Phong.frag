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
float alpha = 5000.0;
vec3 Ke;

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
		//lightPosition = (vec4(p_lights[i].p_light_position, 1.0)).xyz; //Luz se desplaza con la cámara
		lightPosition = (lightView * vec4(p_lights[i].p_light_position, 1.0)).xyz; //Luz con posición invariante
		//Diffuse
		vec3 L = normalize (lightPosition - pos);
		vec3 diffuse = p_lights[i].p_light_diffuse_intensity * Kd * dot (L,N);
		c += clamp(diffuse, 0.0, 1.0);
		//Specular
		vec3 R = normalize (reflect (-L,N));
		float factor = max (dot (R,V), 0.01);
		vec3 specular = p_lights[i].p_light_specular_intensity * Ks * pow(factor,alpha);
		c += clamp(specular, 0.0, 1.0);
	}

	//Directional Lights
	for(int i=0; i<numDirectionalLights; i++){
		//Diffuse
		vec3 L = normalize(d_lights[i].d_light_direction);
		vec3 diffuse = d_lights[i].d_light_diffuse_intensity * Kd * dot(L,N);
		c += clamp(diffuse, 0.0, 1.0);
		//Specular
		vec3 R = normalize (reflect (-L,N));
		float factor = max (dot (R,V), 0.01);
		vec3 specular = d_lights[i].d_light_specular_intensity * Ks * pow(factor,alpha);
		c += clamp(specular, 0.0, 1.0);
	}

	//Focal Lights
	for(int i=0; i<numFocalLights; i++){
		//Position
		vec3 lightPosition = vec3(1.0);
		//lightPosition = (vec4(f_lights[i].f_light_position, 1.0)).xyz; //Luz se desplaza con la cámara
		lightPosition = (lightView * vec4(f_lights[i].f_light_position, 1.0)).xyz; //Luz con posición invariante
		//Diffuse
		vec3 L = normalize (lightPosition - pos);
		vec3 diffuse = f_lights[i].f_light_diffuse_intensity * Kd * dot (L,N);
		float Ip = 0;
		float cosineAngle = cos(f_lights[i].f_apperture_angle);
		if(dot(f_lights[0].f_light_direction,-L) > cosineAngle){
			Ip = pow((dot(f_lights[0].f_light_direction, -L) - cosineAngle) / (1-cosineAngle), 0.5);
		}
		c += clamp(Ip*diffuse, 0.0, 1.0);
		//Specular
		vec3 R = normalize (reflect (-L,N));
		float factor = max (dot (R,V), 0.01);
		vec3 specular = f_lights[i].f_light_specular_intensity * Ks * pow(factor,alpha);
		c += clamp(Ip*specular, 0.0, 1.0);
	}

	//Emissive
	c+=Ke;
	
	return c;
}

void main()
{
	Ka = texture(colorTex, texCoord).rgb;
	Kd = Ka;
	Ks = texture(specularTex, texCoord).rgb;
	Ke = texture(emiTex, texCoord).rgb;

	N = normalize (norm);
	
	outColor = vec4(shade(), 1.0);
}