#version 450 core
  
in vec2 uv;
out vec4 FragColor;
  
struct Widget {
	vec4 backgroundColor;
	sampler2D backgroundTexture;
	bool useBackgroundTexture;
		
	sampler2D colorAlphaTexture;
	bool useColorAlphaTexture;
		
	//int borderWidth;
	//vec3 borderColor;
};

uniform Widget widget;

void main() {
	if (widget.useBackgroundTexture) {
		vec4 textureData = texture(widget.backgroundTexture, uv);
		FragColor = textureData;
	}
	else {
		FragColor = widget.backgroundColor;
		
		if (widget.useColorAlphaTexture) {
			FragColor.a = texture(widget.colorAlphaTexture, uv).r;
		}
		
	}
}