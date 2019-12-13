#version 450 core
  
layout (location = 0) in vec3 pos;
layout (location = 2) in vec2 textureCoordinates;


struct Scene {
	mat4 projection;
};

struct Transform {
	mat4 localToScreen;
};

uniform Scene scene;
uniform Transform transform;

out vec2 uv;
out gl_PerVertex { vec4 gl_Position; };
  
void main() {
	uv = textureCoordinates;
    gl_Position = scene.projection * transform.localToScreen * vec4(pos.xy, 0.0, 1.0);
}