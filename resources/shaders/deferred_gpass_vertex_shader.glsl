#version 450 core
  
layout (location = 0) in vec3 attrPos;
layout (location = 1) in vec3 attrNorm;
layout (location = 2) in vec2 attrUV;

struct Scene {
	mat4 worldToCamera;
	mat4 cameraToProjection;
};

struct Transform {
	mat4 localToWorld;
};

uniform Scene scene;
uniform Transform transform;

out VertexData
{
	vec3 position;
	vec3 normal;
	vec2 uv;
} outVertexData;

out gl_PerVertex { vec4 gl_Position; };
  
void main() {
	gl_Position = scene.cameraToProjection * scene.worldToCamera * transform.localToWorld * vec4(attrPos, 1.0);
	
	outVertexData.position = attrPos.xyz;
	outVertexData.uv = attrUV;

	mat3 dirToWorldTransform = transpose(inverse(mat3(transform.localToWorld)));	
	outVertexData.normal = dirToWorldTransform * attrNorm;
}