#version 330

out vec4 outColor;

in vec3 color;
in vec3 pos;
in vec3 norm;
in vec2 texCoord;

//Textures
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

//Vectors
vec3 N;
vec3 V;
vec3 L;

//Object properties
vec3 Ka;
vec3 Kd;
vec3 Ks;
vec3 Ke;
float nSpecular;

int colorLOD = 6;
float shapeThickness = 0.3;
float scaleFactor = 1.0/colorLOD;

vec3 shade()
{
	vec3 c = vec3(0.0);

	//Color discretization
	float diffuse = max(0.0, dot(L,N));
	float specular = 0.0;
	float specMask;
	vec3 Id2 = lIntD2 * Kd * floor(diffuse*colorLOD) * scaleFactor;

	vec3 R = reflect (-L,N);
	if(dot(N,L) > 0.0)
		specular = dot(Ks,Ks) * pow(max(0.0, dot(R,V)), nSpecular);
	specMask = (pow(dot(R,V), nSpecular) > 0.4) ? 1 : 0;

	vec3 Is2 = vec3(specular*specMask);
	c = lIntA2*Ka + Id2 + Is2;

	//Silueta.
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
	//Vectors
	N = normalize(norm);
	V = normalize(-pos);
	L = normalize(lDir);
	
	//Object properties
	Ka = vec3(1.0, 0.0, 0.0);
	Kd = Ka;
	Ks = vec3(0.2);

	outColor = vec4(shade(), 1.0);
}