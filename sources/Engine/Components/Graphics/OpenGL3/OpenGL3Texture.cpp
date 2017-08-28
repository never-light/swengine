#include "OpenGL3Texture.h"

#include <iostream>
#include <Engine\Components\Debugging\Log.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

OpenGL3Texture::OpenGL3Texture() : m_textureId(NULL) {

}

OpenGL3Texture::~OpenGL3Texture() {
	unload();
}

void OpenGL3Texture::loadFromFile(const std::string& filename) {
	if (m_textureId) {
		errlog() << "Текстура уже загружена, попытка повторной загрузки из <" << filename << ">";
	}

	glGenTextures(1, &this->m_textureId);
	glBindTexture(GL_TEXTURE_2D, this->m_textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int nrChannels;
	unsigned char *data = stbi_load(filename.c_str(), &m_width, &m_height, &nrChannels, STBI_rgb_alpha);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		errlog() << "Ошибка загрузки текстуры <" << filename << ">";
	}

	stbi_image_free(data);
}

void OpenGL3Texture::unload() {
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