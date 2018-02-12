[parameters]
UBO frame.ubo as frame
UBO object.ubo as object
UBO lighting.ubo as lighting

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

[vertexShader]
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

layout (std140) uniform frame
{
	mat4 view;
    mat4 projection;
	
	vec3 cameraPosition;
	vec3 cameraDirection;
};

layout (std140) uniform object
{
	mat4 model;
};

void main()
{
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.Normal = mat3(transpose(inverse(model))) * aNormal;  
    vs_out.TexCoords = aTexCoords;
    
	vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
	vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);

	vs_out.TBN = mat3(T, B, N);

    gl_Position = projection * view * vec4(vs_out.FragPos, 1.0);
}

[fragmentShader]
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

struct LightIntensity {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Light {	
	mat4 spaceMatrix;

	vec3 position;
	vec3 direction;
	vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
	
	float innerCutOff;
	float outerCutOff;
	
	float attenuationConstant;
	float attenuationLinear;
	float attenuationQuadratic;
	
	int type;
};

layout (std140) uniform frame
{
	mat4 view;
    mat4 projection;
	
	vec3 cameraPosition;
	vec3 cameraDirection;
};

struct Test {
	vec3 v;
	int a;
};

//uniform Light lightsArray[MAX_LIGHTS];

layout (std140) uniform lighting
{
	Light lightsArray[MAX_LIGHTS];
	int lightsCount;
};

uniform Material mtl;

uniform sampler2D shadowMap;

vec3 CalcDirLight(int lightIndex, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(int lightIndex, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(int lightIndex, vec3 normal, vec3 fragPos, vec3 viewDir);
LightIntensity CalcLightIntensity(int lightIndex, vec3 normal, vec3 lightDir, vec3 viewDir);

float ShadowCalculation(int lightIndex, vec3 fragPos)
{
	vec4 fragPosLightSpace = lightsArray[lightIndex].spaceMatrix * vec4(fragPos, 1.0);

    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(lightsArray[lightIndex].position - fragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;


}

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

	//int lightsCount = 1;
	
	for (int i = 0; i < lightsCount; i++) {
		//result += test[0].v * test[0].a - test[1].a * test[1].v;
		//result += l;
		
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
	vec3 color;
	
	if (mtl.useDiffuseMap == 1) {
		color = texture(mtl.diffuseMap, fs_in.TexCoords).rgb;
	}
	else {
		color = mtl.diffuseColor;
	}
	
	vec3 lightDir = normalize(-lightsArray[lightIndex].direction );

	LightIntensity intensity = CalcLightIntensity(lightIndex, normal, lightDir, viewDir);
	
	float shadow = ShadowCalculation(lightIndex, fs_in.FragPos);       
	//return (intensity.ambient + (1.0 - shadow) * (intensity.diffuse + intensity.specular)) * color;    
	
    return (intensity.ambient + intensity.diffuse + intensity.specular);
}

vec3 CalcPointLight(int lightIndex, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(lightsArray[lightIndex].position - fragPos);

    float distance = length(lightsArray[lightIndex].position - fragPos);
    float attenuation = 1.0 / (lightsArray[lightIndex].attenuationConstant + lightsArray[lightIndex].attenuationLinear * distance + lightsArray[lightIndex].attenuationQuadratic * (distance * distance));    

	LightIntensity intensity = CalcLightIntensity(lightIndex, normal, lightDir, viewDir);
	return (intensity.ambient * attenuation + intensity.diffuse * attenuation + intensity.specular * attenuation);
}

vec3 CalcSpotLight(int lightIndex, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(lightsArray[lightIndex].position - fragPos);

     float distance = length(lightsArray[lightIndex].position - fragPos);
    float attenuation = 1.0 / (lightsArray[lightIndex].attenuationConstant + lightsArray[lightIndex].attenuationLinear * distance + lightsArray[lightIndex].attenuationQuadratic * (distance * distance));    

    float theta = dot(lightDir, normalize(-lightsArray[lightIndex].direction)); 
    float epsilon = lightsArray[lightIndex].innerCutOff - lightsArray[lightIndex].outerCutOff;
    float _intensity = clamp((theta - lightsArray[lightIndex].outerCutOff) / epsilon, 0.0, 1.0);

	LightIntensity intensity = CalcLightIntensity(lightIndex, normal, lightDir, viewDir);
	return (intensity.ambient * _intensity * attenuation +
		intensity.diffuse * _intensity * attenuation +
		intensity.specular * _intensity * attenuation);
}