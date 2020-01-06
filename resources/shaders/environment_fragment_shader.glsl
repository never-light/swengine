#version 450 core

in vec2 uv;
out vec4 frag_colour;

uniform sampler2D environmentMap;

void main() {
	frag_colour = vec4(texture(environmentMap, uv).rgb, 1.0);
};
