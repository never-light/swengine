#pragma once

#include <Engine\Components\Graphics\RenderSystem\Texture.h>

#include "OpenGL3.h"

class OpenGL3Texture : public Texture {
public:
	OpenGL3Texture(int width, int height, TextureInternalFormat internalFormat, TextureFormat format, TextureDataType type);
	OpenGL3Texture(int width, int height, const unsigned char* data);
	~OpenGL3Texture();

	int getWidth() const override;
	int getHeight() const override;

	GLuint getTexturePointer() const;
private:
	GLuint m_textureId;

	int m_width;
	int m_height;
};