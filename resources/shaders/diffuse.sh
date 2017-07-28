@vertexShader
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    TexCoords = aTexCoords;
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
@fragmentShader
#version 330 core
out vec4 FragColor;

struct MaterialLightmaps {
	sampler2D diffuse;
	sampler2D specular;
	int useDiffuseMap;
	int useSpecularMap;
};

struct Material {
	MaterialLightmaps lightmaps;
	vec3 diffuseColor;
	vec3 specularColor;
	vec3 ambientColor;
    float shininess;
}; 

struct LightAttenuation {
	float constant;
	float linear;
	float quadratic;
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

#define MAX_LIGHTS 4

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 cameraPosition;
uniform Light lights[MAX_LIGHTS];
uniform Material material;

// function prototypes
vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(cameraPosition - FragPos);

	vec3 result;

	for (int i = 0; i < MAX_LIGHTS; i++) {
		if (lights[i].type == 0) continue;
		
		if (lights[i].type == 1) {
			result += CalcDirLight(lights[i], norm, viewDir);
		}
		else if (lights[i].type == 2) {
			 result += CalcPointLight(lights[i], norm, FragPos, viewDir); 
		}
		else if (lights[i].type == 3) {
			result += CalcSpotLight(lights[i], norm, FragPos, viewDir);
		}
	}

	 FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
	
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	if (material.lightmaps.useDiffuseMap == 1) {
		ambient = light.ambientColor * vec3(texture(material.lightmaps.diffuse, TexCoords));
		diffuse = light.diffuseColor * diff * vec3(texture(material.lightmaps.diffuse, TexCoords));
	}
	else {
		ambient = light.ambientColor * material.ambientColor;
		diffuse = light.diffuseColor * (diff * material.diffuseColor);
	}
    
	if (material.lightmaps.useSpecularMap == 1) {
		 specular = light.specularColor * spec * vec3(texture(material.lightmaps.specular, TexCoords));
	}
	else {
		 specular = light.specularColor * spec * material.specularColor;
	}
   
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.attenuation.constant + light.attenuation.linear * distance + light.attenuation.quadratic * (distance * distance));    
    // combine results
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	if (material.lightmaps.useDiffuseMap == 1) {
		ambient = light.ambientColor * vec3(texture(material.lightmaps.diffuse, TexCoords));
		diffuse = light.diffuseColor * diff * vec3(texture(material.lightmaps.diffuse, TexCoords));
	}
	else {
		ambient = light.ambientColor * material.ambientColor;
		diffuse = light.diffuseColor * diff * material.diffuseColor;
	}
    
	if (material.lightmaps.useSpecularMap == 1) {
		 specular = light.specularColor * spec * vec3(texture(material.lightmaps.specular, TexCoords));
	}
	else {
		 specular = light.specularColor * spec * material.specularColor;
	}
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.attenuation.constant + light.attenuation.linear * distance + light.attenuation.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.innerCutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	if (material.lightmaps.useDiffuseMap == 1) {
		ambient = light.ambientColor * vec3(texture(material.lightmaps.diffuse, TexCoords));
		diffuse = light.diffuseColor * diff * vec3(texture(material.lightmaps.diffuse, TexCoords));
	}
	else {
		ambient = light.ambientColor * material.ambientColor;
		diffuse = light.diffuseColor * diff * material.diffuseColor;
	}
    
	if (material.lightmaps.useSpecularMap == 1) {
		 specular = light.specularColor * spec * vec3(texture(material.lightmaps.specular, TexCoords));
	}
	else {
		 specular = light.specularColor * spec * material.specularColor;
	}
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}