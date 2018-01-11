#include "OpenGL3Texture.h"

#include <iostream>
#include <Engine\Components\Debugging\Log.h>

OpenGL3Texture::OpenGL3Texture(int width, int height, const unsigned char* data) 
	: m_textureId(NULL), 
	m_width(width), 
	m_height(height) 
{
	glGenTextures(1, &this->m_textureId);
	glBindTexture(GL_TEXTURE_2D, this->m_textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
}

OpenGL3Texture::~OpenGL3Texture() {
	if (m_textureId) {
		glDeleteTextures(1, &m_textureId);
	}
}

textureId OpenGL3Texture::getTexturePointer() const {
	return m_textureId;
}

int OpenGL3Texture::getWidth() const {
	return m_width;
}

int OpenGL3Texture::getHeight() const {
	return m_height;
}