#version 450 core

layout (location = 0) out vec4 gAlbedo;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gPosition;

in VertexData
{
	vec3 position;
	vec3 normal;
	vec2 uv;
} inVertexData;

uniform sampler2D tex;

void main() {
	gAlbedo = vec4(texture(tex, inVertexData.uv).rgb, 1.0);
	gNormal = inVertexData.normal;
	gPosition = inVertexData.position;
};
