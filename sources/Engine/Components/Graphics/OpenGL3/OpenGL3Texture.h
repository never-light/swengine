#pragma once

#include <Engine\Components\Graphics\RenderSystem\Texture.h>

#include "OpenGL3.h"

typedef GLuint textureId;

class OpenGL3Texture : public Texture {
public:
	OpenGL3Texture(int width, int height, const unsigned char* data);
	~OpenGL3Texture();

	int getWidth() const override;
	int getHeight() const override;

	textureId getTexturePointer() const;
private:
	textureId m_textureId;

	int m_width;
	int m_height;
};