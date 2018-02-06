#include "OpenGL3Texture.h"

#include <iostream>
#include <Engine\Components\Debugging\Log.h>

std::unordered_map<Texture::DataTarget, GLenum> OpenGL3Texture::dataTargetMap{
	{ Texture::DataTarget::_2D, GL_TEXTURE_2D },
	{ Texture::DataTarget::CubeMapPositiveX, GL_TEXTURE_CUBE_MAP_POSITIVE_X },
	{ Texture::DataTarget::CubeMapNegativeX, GL_TEXTURE_CUBE_MAP_NEGATIVE_X },
	{ Texture::DataTarget::CubeMapPositiveY, GL_TEXTURE_CUBE_MAP_POSITIVE_Y },
	{ Texture::DataTarget::CubeMapNegativeY, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y },
	{ Texture::DataTarget::CubeMapPositiveZ, GL_TEXTURE_CUBE_MAP_POSITIVE_Z },
	{ Texture::DataTarget::CubeMapNegativeZ, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z }
};

std::unordered_map<Texture::InternalFormat, GLint> OpenGL3Texture::internalFormatMap{
	{ Texture::InternalFormat::RGB, GL_RGB },
	{ Texture::InternalFormat::RGBA, GL_RGBA },
	{ Texture::InternalFormat::SRGB, GL_SRGB },
	{ Texture::InternalFormat::SRGBA, GL_SRGB_ALPHA },
	{ Texture::InternalFormat::RGB16F, GL_RGB16F },
	{ Texture::InternalFormat::RGBA16F, GL_RGBA16F },
	{ Texture::InternalFormat::SRGB8, GL_SRGB8 },
	{ Texture::InternalFormat::SRGB8A8, GL_SRGB8_ALPHA8 }
};

std::unordered_map<Texture::PixelFormat, GLenum> OpenGL3Texture::pixelFormatMap{
	{ Texture::PixelFormat::RGB, GL_RGB },
	{ Texture::PixelFormat::RGBA, GL_RGBA }
};

std::unordered_map<Texture::DataType, GLenum> OpenGL3Texture::dataTypeMap{
	{ Texture::DataType::Float, GL_FLOAT },
	{ Texture::DataType::UnsignedByte, GL_UNSIGNED_BYTE }
};

std::unordered_map<Texture::Parameter, GLenum> OpenGL3Texture::parametersNameMap{
	{ Texture::Parameter::MinFilter, GL_TEXTURE_MIN_FILTER },
	{ Texture::Parameter::MagFilter, GL_TEXTURE_MAG_FILTER },
	{ Texture::Parameter::WrapS, GL_TEXTURE_WRAP_S },
	{ Texture::Parameter::WrapT, GL_TEXTURE_WRAP_T },
	{ Texture::Parameter::WrapR, GL_TEXTURE_WRAP_R },
};

std::unordered_map<Texture::ParameterValue, GLint> OpenGL3Texture::parametersValuesMap{
	{ Texture::ParameterValue::Nearest, GL_NEAREST },
	{ Texture::ParameterValue::Linear, GL_LINEAR },
	{ Texture::ParameterValue::LinearMipmapLinear, GL_LINEAR_MIPMAP_LINEAR },
	{ Texture::ParameterValue::Repeat, GL_REPEAT },
	{ Texture::ParameterValue::ClampToEdge, GL_CLAMP_TO_EDGE }
};

OpenGL3Texture::OpenGL3Texture(Type type) 
	: Texture(type), m_type(type), m_textureId(0)
{

}

OpenGL3Texture::~OpenGL3Texture() {
	freeData();
}

void OpenGL3Texture::lock(bool replace) {
	if (replace) {
		freeData();
		glGenTextures(1, &m_textureId);
	}

	glBindTexture(getBindTarget(), m_textureId);
}

void OpenGL3Texture::setPlainData(DataTarget target, int width, int height,
	InternalFormat internalFormat,
	PixelFormat format,
	DataType type,
	const unsigned char* data
) 
{
	glTexImage2D(
		dataTargetMap[target],
		0, 
		internalFormatMap[internalFormat], 
		width, 
		height, 
		0,
		pixelFormatMap[format],
		dataTypeMap[type], 
		(data == nullptr) ? NULL : data
	);
}

void OpenGL3Texture::setMultisamplePlainData(DataTarget target, int width, int height,
	int samplesCount,
	InternalFormat internalFormat,
	bool fixedSampleLocations
)
{
	if (m_type != Texture::Type::_2DMultisample)
		throw std::exception();

	glTexImage2DMultisample(
		GL_TEXTURE_2D_MULTISAMPLE, 
		samplesCount, 
		internalFormatMap[internalFormat],
		width,
		height,
		(fixedSampleLocations) ? GL_TRUE : GL_FALSE
	);
}

void OpenGL3Texture::setParameter(Parameter parameter, ParameterValue value) {
	glTexParameteri(getBindTarget(), parametersNameMap[parameter], parametersValuesMap[value]);
}

void OpenGL3Texture::generateMipmaps() {
	if (m_type == Texture::Type::_2D)
		glGenerateMipmap(GL_TEXTURE_2D);
}

void OpenGL3Texture::unlock() {
	
}

void OpenGL3Texture::freeData() {
	if (m_textureId) {
		glDeleteTextures(1, &m_textureId);
	}
}

Texture::Type OpenGL3Texture::getType() const {
	return m_type;
}

GLenum OpenGL3Texture::getBindTarget() const {
	if (m_type == Texture::Type::_2D)
		return GL_TEXTURE_2D;
	
	if (m_type == Texture::Type::CubeMap)
		return GL_TEXTURE_CUBE_MAP;

	if (m_type == Texture::Type::_2DMultisample)
		return GL_TEXTURE_2D_MULTISAMPLE;
}

GLuint OpenGL3Texture::getTexturePointer() const {
	return m_textureId;
}