#pragma once

#include <unordered_map>
#include <Engine/Components/Graphics/RenderSystem/Texture.h>

#include "GL.h"

class GLTexture : public Texture {
public:
	GLTexture();
	~GLTexture();

	virtual void create() override;
	virtual void destroy() override;

	virtual void bind() override;
	virtual void bind(unsigned int unit) override;
	virtual void unbind() override;

	virtual void fillMultisampleData(int samplesCount) override;
	virtual void setData(PixelFormat pixelFormat, PixelDataType pixelDataType, const std::byte* data) override;
	virtual void setData(CubeMapFace cubeMapFace, PixelFormat pixelFormat, PixelDataType pixelDataType, const std::byte* data) override;

	GLuint getTexturePointer() const; 
	GLenum getBindingTarget() const;

	virtual void generateMipMaps() override;

	virtual void setMinificationFilter(Filter filter) override;
	virtual void setMagnificationFilter(Filter filter) override;
	virtual void setWrapMode(WrapMode mode) override;

	virtual void setTarget(Texture::Target target) override;

	virtual void enableAnisotropicFiltering(float quality);
private:
	GLuint m_texture;
	GLenum m_bindingTarget;

private:
	static std::unordered_map<Texture::InternalFormat, GLint> m_internalFormatMap;
	static std::unordered_map<Texture::PixelFormat, GLenum> m_pixelFormatMap;
	static std::unordered_map<Texture::PixelDataType, GLenum> m_pixelDataTypeMap;
};