[vertex]
#version 330 core

layout (location = 0) in vec4 vertex;

struct Transform {
	mat4 localToWorld;
	mat4 projection;
};

uniform Transform transform;

out vec2 uv;

void main()
{
	uv = vertex.zw;
    gl_Position = transform.projection * transform.localToWorld * vec4(vertex.xy, 0.0, 1.0);
}
[/vertex]

[fragment]
#version 330 core

in vec2 uv;
out vec4 FragColor;

struct Quad {
	vec4 color;
	sampler2D texture;
	bool useTexture;
	bool useFirstChannel;
};

uniform Quad quad;

void main() {
    //FragColor = vec4(texture(material.diffuseTexture, uv).rgb, 1.0);
	
	if (quad.useTexture) {
		vec4 textureData = texture(quad.texture, uv);
		
		if (quad.useFirstChannel) {
			FragColor = vec4(quad.color.rgb, textureData.r);
		}
		else 
			FragColor = textureData;
	}
	else
		FragColor = quad.color;
}
[/fragment]