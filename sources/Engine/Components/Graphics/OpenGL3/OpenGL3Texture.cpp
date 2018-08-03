#include "OpenGL3Texture.h"

#include <iostream>
#include "OpenGL3Errors.h"

#include <Engine\assertions.h>

std::unordered_map<Texture::InternalFormat, GLint> OpenGL3Texture::m_internalFormatMap{
	{ Texture::InternalFormat::R8, GL_R8 },
	{ Texture::InternalFormat::RG8, GL_RG8 },
	{ Texture::InternalFormat::RGB8, GL_RGB8 },
	{ Texture::InternalFormat::RGBA8, GL_RGBA8 },
	{ Texture::InternalFormat::SRGB8, GL_SRGB8 },
	{ Texture::InternalFormat::SRGBA8, GL_SRGB8_ALPHA8 },
};

std::unordered_map<Texture::PixelFormat, GLenum> OpenGL3Texture::m_pixelFormatMap{
	{ Texture::PixelFormat::R, GL_RED },
	{ Texture::PixelFormat::RG, GL_RG },
	{ Texture::PixelFormat::RGB, GL_RGB },
	{ Texture::PixelFormat::RGBA, GL_RGBA }
};

std::unordered_map<Texture::PixelDataType, GLenum> OpenGL3Texture::m_pixelDataTypeMap{
	{ Texture::PixelDataType::Float, GL_FLOAT },
	{ Texture::PixelDataType::UnsignedByte, GL_UNSIGNED_BYTE }
};

OpenGL3Texture::OpenGL3Texture()
	: m_texture(0)
{
}

OpenGL3Texture::~OpenGL3Texture() {
	if (m_texture != 0)
		destroy();
}

void OpenGL3Texture::create()
{
	glGenTextures(1, &m_texture);
}

void OpenGL3Texture::destroy()
{
	if (m_texture != 0) {
		glDeleteTextures(1, &m_texture);
		m_texture = 0;
	}
}

void OpenGL3Texture::bind()
{
	OPENGL3_CALL(glBindTexture(m_bindingTarget, m_texture));
}

void OpenGL3Texture::bind(unsigned int unit)
{
	OPENGL3_CALL(glActiveTexture(GL_TEXTURE0 + unit));
	bind();
}

void OpenGL3Texture::unbind()
{
	glBindTexture(m_bindingTarget, 0);
}

void OpenGL3Texture::fillMultisampleData(int samplesCount) {
	OPENGL3_CALL(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samplesCount,
		m_internalFormatMap[m_internalFormat], m_width, m_height, GL_TRUE));
}

void OpenGL3Texture::setData(PixelFormat pixelFormat, PixelDataType pixelDataType, const std::byte* data)
{
	_assert(m_target == Target::_2D);

	GLenum dataTarget;

	if (m_target == Target::_2D)
		dataTarget = GL_TEXTURE_2D;

	OPENGL3_CALL(glTexImage2D(dataTarget, 0, m_internalFormatMap[m_internalFormat],
		m_width, m_height, 0, m_pixelFormatMap[pixelFormat], m_pixelDataTypeMap[pixelDataType], data));
}

void OpenGL3Texture::setData(CubeMapFace cubeMapFace, PixelFormat pixelFormat, PixelDataType pixelDataType, const std::byte* data)
{
	OPENGL3_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + (int)cubeMapFace, 0, m_internalFormatMap[m_internalFormat],
		m_width, m_height, 0, m_pixelFormatMap[pixelFormat], m_pixelDataTypeMap[pixelDataType], data));
}

GLuint OpenGL3Texture::getTexturePointer() const
{
	return m_texture;
} 

void OpenGL3Texture::generateMipMaps() {
	if (m_target == Target::_2D)
		OPENGL3_CALL(glGenerateMipmap(GL_TEXTURE_2D));
}

void OpenGL3Texture::setMinificationFilter(Filter filter) {
	Texture::setMinificationFilter(filter);

	GLint value;

	if (filter == Filter::Linear)
		value = GL_LINEAR;
	else if (filter == Filter::Nearest)
		value = GL_NEAREST;
	else if (filter == Filter::LinearMipmapLinear)
		value = GL_LINEAR_MIPMAP_LINEAR;

	OPENGL3_CALL(glTexParameteri(m_bindingTarget, GL_TEXTURE_MIN_FILTER, value));
}

void OpenGL3Texture::setMagnificationFilter(Filter filter) {
	Texture::setMagnificationFilter(filter);

	GLint value;

	if (filter == Filter::Linear)
		value = GL_LINEAR;
	else if (filter == Filter::Nearest)
		value = GL_NEAREST;
	else if (filter == Filter::LinearMipmapLinear)
		value = GL_LINEAR_MIPMAP_LINEAR;

	OPENGL3_CALL(glTexParameteri(m_bindingTarget, GL_TEXTURE_MAG_FILTER, value));
}

void OpenGL3Texture::setWrapMode(WrapMode mode) {
	Texture::setWrapMode(mode);

	GLint value;
	
	if (mode == WrapMode::Repeat)
		value = GL_REPEAT;
	else if (mode == WrapMode::ClampToEdge)
		value = GL_CLAMP_TO_EDGE;

	OPENGL3_CALL(glTexParameteri(m_bindingTarget, GL_TEXTURE_WRAP_S, value));
	OPENGL3_CALL(glTexParameteri(m_bindingTarget, GL_TEXTURE_WRAP_T, value));
}

void OpenGL3Texture::setTarget(Texture::Target target)
{
	Texture::setTarget(target);

	if (m_target == Target::_2D)
		m_bindingTarget = GL_TEXTURE_2D;
	else if (m_target == Target::CubeMap)
		m_bindingTarget = GL_TEXTURE_CUBE_MAP;
	else if (m_target == Target::_2DMultisample)
		m_bindingTarget = GL_TEXTURE_2D_MULTISAMPLE;
}

void OpenGL3Texture::enableAnisotropicFiltering(float quality)
{
	Texture::enableAnisotropicFiltering(quality);

	OPENGL3_CALL(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, quality));
}
