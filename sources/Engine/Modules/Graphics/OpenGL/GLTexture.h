#pragma once

#include "Modules/ResourceManagement/ResourcesManagement.h"

#include "GL.h"
#include "types.h"

class GLShader;

enum class GLTextureType {
  Texture2D, Cubemap
};

enum class GLTextureInternalFormat {
  R8 = GL_R8,
  R16 = GL_R16,
  RG8 = GL_RG8,
  RG16 = GL_RG16,
  RGB8 = GL_RGB8,
  RGBA8 = GL_RGBA8,
  RGBA16 = GL_RGBA16,
  RGB16F = GL_RGB16F,
  RGB32F = GL_RGB32F,
  SRGB8 = GL_SRGB8,
  SRGBA8 = GL_SRGB8_ALPHA8,
  Depth24 = GL_DEPTH_COMPONENT24,
  Depth24Stencil8 = GL_DEPTH24_STENCIL8
};

struct TextureTransform {
  glm::vec2 offset{0.0f, 0.0f};
  glm::vec2 scale{1.0f, 1.0f};
  float rotation{0.0f};
};

class GLTexture : public Resource {
 public:
  GLTexture(GLTextureType type, int width, int height, GLTextureInternalFormat internalFormat);
  ~GLTexture() override;

  void setData(GLenum dataFormat, GLenum dataType, const std::byte* data, size_t lodIndex = 0);
  void setCubemapFaceData(size_t faceIndex,
      GLenum dataFormat,
      GLenum dataType,
      const std::byte* data,
      size_t lodIndex = 0);

  void generateMipMaps();

  void setMinificationFilter(GLint filter);
  void setMagnificationFilter(GLint filter);
  void setWrapModeU(GLint mode);
  void setWrapModeV(GLint mode);
  void setWrapModeW(GLint mode);

  void enableAnisotropicFiltering(float quality);

  [[nodiscard]] int getWidth() const;
  [[nodiscard]] int getHeight() const;

  [[nodiscard]] GLTextureInternalFormat getInternalFormat() const;
  [[nodiscard]] GLuint getGLHandle() const;

 private:
  GLTextureType m_type;
  int m_width;
  int m_height;

  GLTextureInternalFormat m_internalFormat;

  GLuint m_texture;

 private:
  friend class GLShader;
};
