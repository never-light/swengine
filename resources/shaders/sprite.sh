@vertexShader
#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;

void main()
{
    TexCoords = vertex.zw;
    //gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
	gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}

@fragmentShader
#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D spriteTexture;

void main()
{
    color = texture(spriteTexture, TexCoords);
} 
