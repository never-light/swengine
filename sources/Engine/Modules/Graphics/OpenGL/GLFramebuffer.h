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

class GLFramebuffer
{
public:
    GLFramebuffer(int width, int height, size_t colorComponentsCount,
                  GLTextureInternalFormat colorComponentsFormat,
                  bool createDepthStencilComponent);

    GLFramebuffer(const GLFramebuffer& framebuffer) = delete;

    ~GLFramebuffer();

    Rect getBounds() const;
    float getAspectRatio() const;

    int getWidth() const;
    int getHeight() const;

    void clearColor(const glm::vec4& color, size_t componentIndex = 0);
    void clearDepthStencil(float depthValue, int stencilValue);

    void copyColor(GLFramebuffer& target, size_t sourceComponentIndex = 0, size_t targetComponentIndex = 0);
    void copyColor(GLFramebuffer& target, const Rect& sourceRect, const Rect& targetRect,
                FramebufferCopyFilter filter, size_t sourceComponentIndex = 0, size_t targetComponentIndex = 0);

    void copyDepthStencil(GLFramebuffer& target);
    void copyDepthStencil(GLFramebuffer& target, const Rect& sourceRect, const Rect& targetRect,
                FramebufferCopyFilter filter);

    GLuint getGLHandle() const;

    GLTexture* getDepthComponent() const;
    GLTexture* getColorComponent(size_t index) const;

private:
    /*!
     * \brief GLFramebuffer Handles GL default framebuffer
     * \param width
     * \param height
     */
    GLFramebuffer(int width, int height);

private:
    void copyTo(GLFramebuffer& target, const Rect& sourceRect, const Rect& targetRect,
                GLbitfield copyMask, FramebufferCopyFilter filter,
                GLenum sourceAttachment, GLenum targetAttachment);

    void enableWritingToAllBuffers();

private:
    GLuint m_framebuffer;

    int m_width;
    int m_height;

    size_t m_colorComponentsCount;

    std::array<std::unique_ptr<GLTexture>, 4> m_colorComponents;
    GLTextureInternalFormat m_colorComponentsFormat;

    std::unique_ptr<GLTexture> m_depthComponent;

private:
    friend class GLGraphicsContext;
};

