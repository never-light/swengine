#include "GLTexture.h"

#include <iostream>
#include "GLDebug.h"

#include <Engine/assertions.h>

std::unordered_map<Texture::InternalFormat, GLint> GLTexture::m_internalFormatMap{
	{ Texture::InternalFormat::SRGB8, GL_SRGB8 },
	{ Texture::InternalFormat::SRGBA8, GL_SRGB8_ALPHA8 },

	{ Texture::InternalFormat::R8, GL_R8 },
	{ Texture::InternalFormat::RG8, GL_RG8 },
	{ Texture::InternalFormat::RGB8, GL_RGB8 },
	{ Texture::InternalFormat::RGBA8, GL_RGBA8 },

	{ Texture::InternalFormat::R16, GL_R16 },
	{ Texture::InternalFormat::RG16, GL_RG16 },
	{ Texture::InternalFormat::RGB16, GL_RGB16 },
	{ Texture::InternalFormat::RGBA16, GL_RGBA16 },

	{ Texture::InternalFormat::R16F, GL_R16F },
	{ Texture::InternalFormat::RG16F, GL_RG16F },
	{ Texture::InternalFormat::RGB16F, GL_RGB16F },
	{ Texture::InternalFormat::RGBA16F, GL_RGBA16F },
	{ Texture::InternalFormat::R11FG11FB10F, GL_R11F_G11F_B10F },

	{ Texture::InternalFormat::R32F, GL_R32F },
	{ Texture::InternalFormat::RG32F, GL_RG32F },
	{ Texture::InternalFormat::RGB32F, GL_RGB32F },
	{ Texture::InternalFormat::RGBA32F, GL_RGBA32F },

	{ Texture::InternalFormat::Depth24Stencil8, GL_DEPTH24_STENCIL8 },
	{ Texture::InternalFormat::Depth24, GL_DEPTH_COMPONENT24 },
	{ Texture::InternalFormat::Depth32, GL_DEPTH_COMPONENT32F },
};

std::unordered_map<Texture::PixelFormat, GLenum> GLTexture::m_pixelFormatMap{
	{ Texture::PixelFormat::R, GL_RED },
	{ Texture::PixelFormat::RG, GL_RG },
	{ Texture::PixelFormat::RGB, GL_RGB },
	{ Texture::PixelFormat::RGBA, GL_RGBA },
	{ Texture::PixelFormat::DepthStencil, GL_DEPTH_STENCIL },
	{ Texture::PixelFormat::Depth, GL_DEPTH_COMPONENT },
};

std::unordered_map<Texture::PixelDataType, GLenum> GLTexture::m_pixelDataTypeMap{
	{ Texture::PixelDataType::Float, GL_FLOAT },
	{ Texture::PixelDataType::UnsignedByte, GL_UNSIGNED_BYTE },
	{ Texture::PixelDataType::UnsignedInt24_8, GL_UNSIGNED_INT_24_8 }
};

GLTexture::GLTexture()
	: m_texture(0)
{
}

GLTexture::~GLTexture() {
	if (m_texture != 0)
		destroy();
}

void GLTexture::create()
{
	glGenTextures(1, &m_texture);
}

void GLTexture::destroy()
{
	if (m_texture != 0) {
		glDeleteTextures(1, &m_texture);
		m_texture = 0;
	}
}

void GLTexture::bind()
{
	GL_CALL(glBindTexture(m_bindingTarget, m_texture));
}

void GLTexture::bind(unsigned int unit)
{
	GL_CALL(glActiveTexture(GL_TEXTURE0 + unit));
	bind();
}

void GLTexture::unbind()
{
	glBindTexture(m_bindingTarget, 0);
}

void GLTexture::fillMultisampleData(int samplesCount) {
	GL_CALL(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samplesCount,
		m_internalFormatMap[m_internalFormat], m_width, m_height, GL_TRUE));
}

void GLTexture::setData(PixelFormat pixelFormat, PixelDataType pixelDataType, const std::byte* data)
{
	_assert(m_target == Target::_2D);

	GLenum dataTarget;

	if (m_target == Target::_2D)
		dataTarget = GL_TEXTURE_2D;

	GL_CALL(glTexImage2D(dataTarget, 0, m_internalFormatMap[m_internalFormat],
		m_width, m_height, 0, m_pixelFormatMap[pixelFormat], m_pixelDataTypeMap[pixelDataType], data));
}

void GLTexture::setData(CubeMapFace cubeMapFace, PixelFormat pixelFormat, PixelDataType pixelDataType, const std::byte* data)
{
	GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + (int)cubeMapFace, 0, m_internalFormatMap[m_internalFormat],
		m_width, m_height, 0, m_pixelFormatMap[pixelFormat], m_pixelDataTypeMap[pixelDataType], data));
}

GLuint GLTexture::getTexturePointer() const
{
	return m_texture;
}
GLenum GLTexture::getBindingTarget() const
{
	return m_bindingTarget;
}


void GLTexture::generateMipMaps() {
	if (m_target == Target::_2D)
		GL_CALL(glGenerateTextureMipmap(m_texture));
}

void GLTexture::setMinificationFilter(Filter filter) {
	Texture::setMinificationFilter(filter);

	GLint value;

	if (filter == Filter::Linear)
		value = GL_LINEAR;
	else if (filter == Filter::Nearest)
		value = GL_NEAREST;
	else if (filter == Filter::LinearMipmapLinear)
		value = GL_LINEAR_MIPMAP_LINEAR;

	GL_CALL(glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, value));
}

void GLTexture::setMagnificationFilter(Filter filter) {
	Texture::setMagnificationFilter(filter);

	GLint value;

	if (filter == Filter::Linear)
		value = GL_LINEAR;
	else if (filter == Filter::Nearest)
		value = GL_NEAREST;
	else if (filter == Filter::LinearMipmapLinear)
		value = GL_LINEAR_MIPMAP_LINEAR;

	GL_CALL(glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, value));
}

void GLTexture::setWrapMode(WrapMode mode) {
	Texture::setWrapMode(mode);

	GLint value;
	
	if (mode == WrapMode::Repeat)
		value = GL_REPEAT;
	else if (mode == WrapMode::ClampToEdge)
		value = GL_CLAMP_TO_EDGE;

	GL_CALL(glTextureParameteri(m_texture, GL_TEXTURE_WRAP_S, value));
	GL_CALL(glTextureParameteri(m_texture, GL_TEXTURE_WRAP_T, value));
}

void GLTexture::setTarget(Texture::Target target)
{
	Texture::setTarget(target);

	if (m_target == Target::_2D)
		m_bindingTarget = GL_TEXTURE_2D;
	else if (m_target == Target::CubeMap)
		m_bindingTarget = GL_TEXTURE_CUBE_MAP;
	else if (m_target == Target::_2DMultisample)
		m_bindingTarget = GL_TEXTURE_2D_MULTISAMPLE;
}

void GLTexture::enableAnisotropicFiltering(float quality)
{
	Texture::enableAnisotropicFiltering(quality);

	GL_CALL(glTextureParameterf(m_texture, GL_TEXTURE_MAX_ANISOTROPY_EXT, quality));
}