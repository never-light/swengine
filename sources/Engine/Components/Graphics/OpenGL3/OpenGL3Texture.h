#pragma once

#include <unordered_map>
#include <Engine\Components\Graphics\RenderSystem\Texture.h>

#include "OpenGL3.h"

class OpenGL3Texture : public Texture {
public:
	OpenGL3Texture(Type type);
	~OpenGL3Texture();

	void lock(bool replace = false) override;

	void setPlainData(DataTarget target, int width, int height,
		InternalFormat internalFormat,
		PixelFormat format,
		DataType type,
		const unsigned char* data = nullptr
	) override;

	void setMultisamplePlainData(DataTarget target, int width, int height,
		int samplesCount,
		InternalFormat internalFormat,
		bool fixedSampleLocations
	) override;

	void setParameter(Parameter parameter, ParameterValue value) override;
	void generateMipmaps() override;
	void unlock() override;

	Texture::Type getType() const override;

	GLenum getBindTarget() const;
	GLuint getTexturePointer() const;

private:
	void freeData();

private:
	static std::unordered_map<Texture::DataTarget, GLenum> dataTargetMap;
	static std::unordered_map<Texture::InternalFormat, GLint> internalFormatMap;
	static std::unordered_map<Texture::PixelFormat, GLenum> pixelFormatMap;
	static std::unordered_map<Texture::DataType, GLenum> dataTypeMap;
	static std::unordered_map<Texture::Parameter, GLenum> parametersNameMap;
	static std::unordered_map<Texture::ParameterValue, GLint> parametersValuesMap;

private:
	Texture::Type m_type;
	GLuint m_textureId;
};