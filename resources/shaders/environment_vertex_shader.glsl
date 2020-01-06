#version 450 core
  
layout (location = 0) in vec3 pos;
layout (location = 2) in vec2 uvvv;


struct Scene {
	mat4 worldToCamera;
	mat4 cameraToProjection;
};

uniform Scene scene;

out vec2 uv;
out gl_PerVertex { vec4 gl_Position; };
  
void main() {
	uv = uvvv;
	vec4 skyboxVertexPos = scene.cameraToProjection * scene.worldToCamera * vec4(pos, 1.0);
	gl_Position = skyboxVertexPos.xyww;
}