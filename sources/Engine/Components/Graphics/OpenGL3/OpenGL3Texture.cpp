#include "OpenGL3Texture.h"

#include <iostream>
#include <Engine\Components\Debugging\Log.h>

OpenGL3Texture::OpenGL3Texture(int width, int height, TextureInternalFormat internalFormat, TextureFormat format, TextureDataType type)
	:  Texture(width, height, internalFormat, format, type),
	m_width(width),
	m_height(height) 
{
	GLint glInternalFormat;

	if (internalFormat == TextureInternalFormat::RGB16F)
		glInternalFormat = GL_RGB16F;
	else if (internalFormat == TextureInternalFormat::RGBA)
		glInternalFormat = GL_RGBA;

	GLenum glFormat;

	if (format == TextureFormat::RGB)
		glFormat = GL_RGB;
	else if (format == TextureFormat::RGBA)
		glFormat = GL_RGBA;

	GLenum glDataType;

	if (type == TextureDataType::Float)
		glDataType = GL_FLOAT;
	else if (type == TextureDataType::UnsignedByte)
		glDataType = GL_UNSIGNED_BYTE;

	glGenTextures(1, &m_textureId);
	glBindTexture(GL_TEXTURE_2D, m_textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, glInternalFormat, width, height, 0, glFormat, glDataType, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

OpenGL3Texture::OpenGL3Texture(int width, int height, const unsigned char* data) 
	: Texture(width, height, data),
	m_textureId(NULL), 
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

GLuint OpenGL3Texture::getTexturePointer() const {
	return m_textureId;
}

int OpenGL3Texture::getWidth() const {
	return m_width;
}

int OpenGL3Texture::getHeight() const {
	return m_height;
}