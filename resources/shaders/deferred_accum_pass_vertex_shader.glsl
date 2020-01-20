#version 450 core
  
layout (location = 0) in vec3 attrPos;
layout (location = 2) in vec2 attrUV;

out VertexData
{
	vec2 uv;
} outVertexData;

out gl_PerVertex { vec4 gl_Position; };
  
void main() {
    gl_Position = vec4(attrPos.xy, 1.0, 1.0);
	
	outVertexData.uv = attrUV;
}