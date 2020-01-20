#version 450 core

struct GBuffer {
	sampler2D albedo;
	sampler2D normals;
	sampler2D positions;
};

in VertexData
{
	vec2 uv;
} inVertexData;

uniform GBuffer gBuffer;

out vec4 out_fragColor;

void main() {
	out_fragColor = texture(gBuffer.albedo, inVertexData.uv) + texture(gBuffer.normals, inVertexData.uv) * 0 + 
		texture(gBuffer.positions, inVertexData.uv) * 0;
}
