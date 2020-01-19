#version 450 core

in vec2 uv;
out vec4 frag_colour;

uniform sampler2D environmentMap;

void main() {
	vec2 texCoords = vec2(1.0 - uv.x, 1.0 - uv.y);
	frag_colour = vec4(texture(environmentMap, texCoords).rgb, 1.0);
};
