@parameters
matrix4 transform.model as model
matrix4 transform.view as view
matrix4 transform.projection as projection
vector3 camera.position as cameraPosition

LightsArray light.sources as lightsArray

Color material.diffuseColor as mtl.diffuseColor
Texture material.diffuseMap as mtl.diffuseMap
bool material.useDiffuseMap as mtl.useDiffuseMap

Color material.specularColor as mtl.specularColor
Texture material.specularMap as mtl.specularMap
bool material.useSpecularMap as mtl.useSpecularMap

Texture material.normalMap as mtl.normalMap
bool material.useNormalMap as mtl.useNormalMap

Color material.ambientColor as mtl.ambientColor
float material.shininess as mtl.shininess

@vertexShader
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;

out VS_OUT {
    vec3 FragPos;
	vec3 Normal;
    vec2 TexCoords;
	mat3 TBN;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.Normal = mat3(transpose(inverse(model))) * aNormal;  
    vs_out.TexCoords = aTexCoords;
    
	vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
	vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));
	// re-orthogonalize T with respect to N
	T = normalize(T - dot(T, N) * N);
	// then retrieve perpendicular vector B with the cross product of T and N
	vec3 B = cross(N, T);

	vs_out.TBN = mat3(T, B, N);

    gl_Position = projection * view * vec4(vs_out.FragPos, 1.0);
}

@fragmentShader
#version 330 core
#define MAX_LIGHTS 4

in VS_OUT {
    vec3 FragPos;
	vec3 Normal;
    vec2 TexCoords;
	mat3 TBN;
} fs_in;

out vec4 FragColor;

struct Material {
	vec3 diffuseColor;
	sampler2D diffuseMap;
	int useDiffuseMap;
	
	vec3 specularColor;
	sampler2D specularMap;
	int useSpecularMap;

	vec3 ambientColor;
    float shininess;
	
	sampler2D normalMap;
	int useNormalMap;
}; 

struct LightAttenuation {
	float constant;
	float linear;
	float quadratic;
};

struct LightIntensity {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Light {
	vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
	
	vec3 direction;
	vec3 position;
	
	float innerCutOff;
    float outerCutOff;
	
	LightAttenuation attenuation;
	
	int type;
};

uniform Light lightsArray[MAX_LIGHTS];

uniform vec3 cameraPosition;
uniform Material mtl;

vec3 CalcDirLight(int lightIndex, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(int lightIndex, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(int lightIndex, vec3 normal, vec3 fragPos, vec3 viewDir);
LightIntensity CalcLightIntensity(int lightIndex, vec3 normal, vec3 lightDir, vec3 viewDir);

void main() {
	vec3 norm;
	vec3 viewDir;
	
	if (mtl.useNormalMap == 1) {
		norm = texture(mtl.normalMap, fs_in.TexCoords).rgb;
		norm = normalize(norm * 2.0 - 1.0);
		norm = normalize(fs_in.TBN * norm); 
	}
    else {
		norm = normalize(fs_in.Normal);
	}
	
	viewDir = normalize(cameraPosition - fs_in.FragPos);

	vec3 result;

	for (int i = 0; i < MAX_LIGHTS; i++) {
		if (lightsArray[i].type == 0) continue;
		
		if (lightsArray[i].type == 1) {
			result += CalcDirLight(i, norm, viewDir);
		}
		else if (lightsArray[i].type == 2) {
			result += CalcPointLight(i, norm, fs_in.FragPos, viewDir); 
		}
		else if (lightsArray[i].type == 3) {
			result += CalcSpotLight(i, norm, fs_in.FragPos, viewDir);
		}
	}

	 FragColor = vec4(result, 1.0);
}

LightIntensity CalcLightIntensity(int lightIndex, vec3 normal, vec3 lightDir, vec3 viewDir) {
	float diffusePower = max(dot(normal, lightDir), 0.0);
	
	vec3 halfwayDir = normalize(lightDir + viewDir);
    float specularPower = pow(max(dot(normal, halfwayDir), 0.0), mtl.shininess);
	
	LightIntensity intensity;
	
	if (mtl.useDiffuseMap == 1) {
		intensity.ambient = lightsArray[lightIndex].ambientColor * vec3(texture(mtl.diffuseMap, fs_in.TexCoords));
		intensity.diffuse = lightsArray[lightIndex].diffuseColor * diffusePower * vec3(texture(mtl.diffuseMap, fs_in.TexCoords));
	}
	else {
		intensity.ambient = lightsArray[lightIndex].ambientColor * mtl.ambientColor;
		intensity.diffuse = lightsArray[lightIndex].diffuseColor * (diffusePower * mtl.diffuseColor);
	}

	if (mtl.useSpecularMap == 1) {
		intensity.specular = lightsArray[lightIndex].specularColor * specularPower * vec3(texture(mtl.specularMap, fs_in.TexCoords));
	}
	else {
		intensity.specular = lightsArray[lightIndex].specularColor * specularPower * mtl.specularColor;
	}
	
	return intensity;
}

vec3 CalcDirLight(int lightIndex, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-lightsArray[lightIndex].direction );

	LightIntensity intensity = CalcLightIntensity(lightIndex, normal, lightDir, viewDir);
    return (intensity.ambient + intensity.diffuse + intensity.specular);
}

vec3 CalcPointLight(int lightIndex, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(lightsArray[lightIndex].position - fragPos);

    float distance = length(lightsArray[lightIndex].position - fragPos);
    float attenuation = 1.0 / (lightsArray[lightIndex].attenuation.constant + lightsArray[lightIndex].attenuation.linear * distance + lightsArray[lightIndex].attenuation.quadratic * (distance * distance));    

	LightIntensity intensity = CalcLightIntensity(lightIndex, normal, lightDir, viewDir);
	return (intensity.ambient * attenuation + intensity.diffuse * attenuation + intensity.specular * attenuation);
}

vec3 CalcSpotLight(int lightIndex, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(lightsArray[lightIndex].position - fragPos);

     float distance = length(lightsArray[lightIndex].position - fragPos);
    float attenuation = 1.0 / (lightsArray[lightIndex].attenuation.constant + lightsArray[lightIndex].attenuation.linear * distance + lightsArray[lightIndex].attenuation.quadratic * (distance * distance));    

    float theta = dot(lightDir, normalize(-lightsArray[lightIndex].direction)); 
    float epsilon = lightsArray[lightIndex].innerCutOff - lightsArray[lightIndex].outerCutOff;
    float _intensity = clamp((theta - lightsArray[lightIndex].outerCutOff) / epsilon, 0.0, 1.0);

	LightIntensity intensity = CalcLightIntensity(lightIndex, normal, lightDir, viewDir);
	return (intensity.ambient * _intensity * attenuation +
		intensity.diffuse * _intensity * attenuation +
		intensity.specular * _intensity * attenuation);
}