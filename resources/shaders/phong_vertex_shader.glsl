#version 450 core
  
layout (location = 0) in vec3 pos;
layout (location = 2) in vec2 uvvv;


struct Scene {
	mat4 worldToCamera;
	mat4 cameraToProjection;
};

struct Transform {
	mat4 localToWorld;
};

uniform Scene scene;
uniform Transform transform;

out vec2 uv;
out gl_PerVertex { vec4 gl_Position; };
  
void main() {
	uv = uvvv;
	gl_Position = scene.cameraToProjection * scene.worldToCamera * transform.localToWorld * vec4(pos, 1.0);
}