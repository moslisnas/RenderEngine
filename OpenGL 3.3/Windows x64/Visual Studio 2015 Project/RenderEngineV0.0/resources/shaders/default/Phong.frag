#version 330 core

out vec4 outColor;

in vec3 color;
in vec3 pos;
in vec3 norm;
in vec2 texCoord;

uniform sampler2D colorTex;
uniform sampler2D emiTex;
uniform sampler2D specularTex;
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

//Propiedades del objeto
vec3 Ka;
vec3 Kd;
vec3 Ks;
vec3 N;
float alpha = 5000.0;
vec3 Ke;

vec3 shade()
{
	vec3 c = vec3(0.0);

	//Luz 1
	c = lIntA * Ka;
	vec3 lightPosition = vec3(1.0);
	//lightPosition = (vec4(lPos, 1.0)).xyz; //Luz se desplaza con la cámara
	lightPosition = (lightView * vec4(lPos, 1.0)).xyz; //Luz con posición invariante
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
	vec3 L2 = normalize (lDir);
	vec3 diffuse2 = lIntD2 * Kd * dot (L2,N);
	c += clamp(diffuse2, 0.0, 1.0);
	
	vec3 V2 = normalize (-pos);
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
	Ks = vec3 (1.0);
	Ks = texture(specularTex, texCoord).rgb;
	Ke = texture(emiTex, texCoord).rgb;

	N = normalize (norm);
	
	outColor = vec4(shade(), 1.0);
}