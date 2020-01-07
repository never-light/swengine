#include "GLTexture.h"
#include "GLDebug.h"

GLTexture::GLTexture(GLTextureType type, int width, int height, GLTextureInternalFormat internalFormat)
    : m_type(type),
      m_width(width),
      m_height(height),
      m_internalFormat(internalFormat)
{
    GL_CALL_BLOCK_BEGIN();

    glCreateTextures(GL_TEXTURE_2D, 1, &m_texture);
    glTextureStorage2D(m_texture, 1, static_cast<GLenum>(internalFormat), width, height);

    GL_CALL_BLOCK_END();
}

GLTexture::~GLTexture()
{
    GL_CALL(glDeleteTextures(1, &m_texture));
}

void GLTexture::setData(GLenum dataFormat, GLenum dataType, const byte* data, size_t lodIndex)
{
    SW_ASSERT(m_type == GLTextureType::Texture2D);

    GL_CALL(glTextureSubImage2D(m_texture, static_cast<GLint>(lodIndex), 0, 0, m_width, m_height, dataFormat, dataType,
                        static_cast<const void*>(data)));
}

void GLTexture::setCubemapFaceData(size_t faceIndex, GLenum dataFormat, GLenum dataType, const byte* data, size_t lodIndex)
{
    SW_ASSERT(m_type == GLTextureType::Cubemap);

    GL_CALL(glTextureSubImage3D(m_texture, static_cast<GLint>(lodIndex), 0, 0, static_cast<GLint>(faceIndex),
                        m_width, m_height, 1, dataFormat, dataType, static_cast<const void*>(data)));
}

void GLTexture::generateMipMaps()
{
    GL_CALL(glGenerateTextureMipmap(m_texture));
}

void GLTexture::setMinificationFilter(GLint filter)
{
    GL_CALL(glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, filter));
}

void GLTexture::setMagnificationFilter(GLint filter)
{
    GL_CALL(glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, filter));
}

void GLTexture::setWrapModeU(GLint mode)
{
    GL_CALL(glTextureParameteri(m_texture, GL_TEXTURE_WRAP_S, mode));
}

void GLTexture::setWrapModeV(GLint mode)
{
    GL_CALL(glTextureParameteri(m_texture, GL_TEXTURE_WRAP_T, mode));
}

void GLTexture::setWrapModeW(GLint mode)
{
    GL_CALL(glTextureParameteri(m_texture, GL_TEXTURE_WRAP_R, mode));
}

void GLTexture::enableAnisotropicFiltering(float quality)
{
    GL_CALL(glTextureParameterf(m_texture, GL_TEXTURE_MAX_ANISOTROPY, quality));
}

int GLTexture::getWidth() const
{
    return m_width;
}

int GLTexture::getHeight() const
{
    return m_height;
}

GLTextureInternalFormat GLTexture::getInternalFormat() const
{
    return m_internalFormat;
}

GLuint GLTexture::getGLHandle() const
{
    return m_texture;
}
