#include "precompiled.h"

#pragma hdrstop

#include "SharedGraphicsState.h"

SharedGraphicsState::SharedGraphicsState(std::shared_ptr<GLGraphicsContext> graphicsContext)
{
  int framebufferWidth = graphicsContext->getDefaultFramebuffer().getWidth();
  int framebufferHeight = graphicsContext->getDefaultFramebuffer().getHeight();

  std::shared_ptr<GLTexture> mrtAlbedo = std::make_shared<GLTexture>(GLTextureType::Texture2D,
    framebufferWidth,
    framebufferHeight,
    GLTextureInternalFormat::RGBA8);

  std::shared_ptr<GLTexture> mrtNormals = std::make_shared<GLTexture>(GLTextureType::Texture2D,
    framebufferWidth,
    framebufferHeight,
    GLTextureInternalFormat::RGB16F);

  std::shared_ptr<GLTexture> mrtPositions = std::make_shared<GLTexture>(GLTextureType::Texture2D,
    framebufferWidth,
    framebufferHeight,
    GLTextureInternalFormat::RGB32F);

  std::shared_ptr<GLTexture> depthStencil = std::make_shared<GLTexture>(GLTextureType::Texture2D,
    framebufferWidth,
    framebufferHeight,
    GLTextureInternalFormat::Depth24Stencil8);

  m_deferredFramebuffer = std::make_unique<GLFramebuffer>(framebufferWidth,
    framebufferHeight,
    std::vector{mrtAlbedo, mrtNormals, mrtPositions},
    depthStencil);

  std::shared_ptr<GLTexture> forwardAlbedo = std::make_shared<GLTexture>(GLTextureType::Texture2D,
    framebufferWidth,
    framebufferHeight,
    GLTextureInternalFormat::RGBA8);

  m_forwardFramebuffer = std::make_unique<GLFramebuffer>(framebufferWidth, framebufferHeight,
    std::vector{forwardAlbedo}, depthStencil);
}

void SharedGraphicsState::setActiveCamera(std::shared_ptr<Camera> camera)
{
  m_activeCamera = camera;
}

std::shared_ptr<Camera> SharedGraphicsState::getActiveCamera() const
{
  return m_activeCamera;
}

const FrameStats& SharedGraphicsState::getFrameStats() const
{
  return m_frameStats;
}

FrameStats& SharedGraphicsState::getFrameStats()
{
  return m_frameStats;
}

GLFramebuffer& SharedGraphicsState::getDeferredFramebuffer() const
{
  return *m_deferredFramebuffer.get();
}

GLFramebuffer& SharedGraphicsState::getForwardFramebuffer() const
{
  return *m_forwardFramebuffer.get();
}

void FrameStats::reset()
{
  m_primitivesCount = 0;
  m_subMeshesCount = 0;
  m_culledSubMeshesCount = 0;
}

void FrameStats::increasePrimitivesCount(size_t count)
{
  m_primitivesCount += count;
}

void FrameStats::increaseSubMeshesCount(size_t count)
{
  m_subMeshesCount += count;
}

void FrameStats::increaseCulledSubMeshesCount(size_t count)
{
  m_culledSubMeshesCount += count;
}

size_t FrameStats::getPrimivitesCount() const
{
  return m_primitivesCount;
}

size_t FrameStats::getSubMeshesCount() const
{
  return m_subMeshesCount;
}

size_t FrameStats::getCulledSubMeshesCount() const
{
  return m_culledSubMeshesCount;
}
