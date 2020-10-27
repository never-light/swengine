#pragma once

#include <array>
#include <vector>
#include <memory>

#include <glm/vec4.hpp>

#include "Modules/Math/Rect.h"
#include "Utility/Bitmask.h"
#include "GLTexture.h"

enum class FramebufferCopyFilter {
  Nearest, Linear
};

class GLGraphicsContext;

class GLFramebuffer {
 public:
  GLFramebuffer(int width, int height, size_t colorComponentsCount,
    GLTextureInternalFormat colorComponentsFormat,
    bool createDepthStencilComponent);

  GLFramebuffer(int width, int height, const std::vector<std::shared_ptr<GLTexture>>& colorComponents,
    std::shared_ptr<GLTexture> depthStencilComponent);

  GLFramebuffer(const GLFramebuffer& framebuffer) = delete;

  ~GLFramebuffer();

  [[nodiscard]] RectI getBounds() const;
  [[nodiscard]] float getAspectRatio() const;

  [[nodiscard]] int getWidth() const;
  [[nodiscard]] int getHeight() const;

  void clearColor(const glm::vec4& color, size_t componentIndex = 0);
  void clearDepthStencil(float depthValue, int stencilValue);

  void copyColor(GLFramebuffer& target, size_t sourceComponentIndex = 0, size_t targetComponentIndex = 0);
  void copyColor(GLFramebuffer& target, const RectI& sourceRect, const RectI& targetRect,
    FramebufferCopyFilter filter, size_t sourceComponentIndex = 0, size_t targetComponentIndex = 0);

  void copyDepthStencil(GLFramebuffer& target);
  void copyDepthStencil(GLFramebuffer& target, const RectI& sourceRect, const RectI& targetRect,
    FramebufferCopyFilter filter);

  [[nodiscard]] GLuint getGLHandle() const;

  [[nodiscard]] std::shared_ptr<GLTexture> getDepthComponent() const;
  [[nodiscard]] std::shared_ptr<GLTexture> getColorComponent(size_t index) const;

 private:

  /*!
   * \brief GLFramebuffer Handles GL default framebuffer
   * \param width
   * \param height
   */
  GLFramebuffer(int width, int height);

  void copyTo(GLFramebuffer& target, const RectI& sourceRect, const RectI& targetRect,
    GLbitfield copyMask, FramebufferCopyFilter filter,
    GLenum sourceAttachment, GLenum targetAttachment);

  void performInternalInitialization(const std::vector<std::shared_ptr<GLTexture>>& colorAttachments,
    std::shared_ptr<GLTexture> depthAttachment);
  void enableWritingToAllBuffers();

 private:
  GLuint m_framebuffer;

  int m_width;
  int m_height;

  size_t m_colorComponentsCount;

  std::array<std::shared_ptr<GLTexture>, 4> m_colorComponents;
  std::shared_ptr<GLTexture> m_depthComponent;

 private:
  friend class GLGraphicsContext;
};

