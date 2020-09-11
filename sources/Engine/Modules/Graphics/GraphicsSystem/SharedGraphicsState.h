#pragma once

#include <memory>

#include "Modules/Graphics/OpenGL/GLFramebuffer.h"
#include "Modules/Graphics/OpenGL/GLGeometryStore.h"
#include "Modules/Graphics/OpenGL/GLGraphicsContext.h"
#include "Camera.h"

struct FrameStats {
 public:
  FrameStats() = default;

  void reset();

  void increasePrimitivesCount(size_t count);
  void increaseSubMeshesCount(size_t count);
  void increaseCulledSubMeshesCount(size_t count);

  [[nodiscard]] size_t getPrimivitesCount() const;
  [[nodiscard]] size_t getSubMeshesCount() const;
  [[nodiscard]] size_t getCulledSubMeshesCount() const;

 private:
  size_t m_primitivesCount = 0;

  size_t m_subMeshesCount = 0;
  size_t m_culledSubMeshesCount = 0;
};

class SharedGraphicsState {
 public:
  SharedGraphicsState(std::shared_ptr<GLGraphicsContext> graphicsContext);

  void setActiveCamera(std::shared_ptr<Camera> camera);
  [[nodiscard]] std::shared_ptr<Camera> getActiveCamera() const;

  [[nodiscard]] const FrameStats& getFrameStats() const;
  FrameStats& getFrameStats();

  [[nodiscard]] GLFramebuffer& getDeferredFramebuffer() const;
  [[nodiscard]] GLFramebuffer& getForwardFramebuffer() const;

 private:
  std::shared_ptr<Camera> m_activeCamera;

  FrameStats m_frameStats;

  std::unique_ptr<GLFramebuffer> m_deferredFramebuffer;
  std::unique_ptr<GLFramebuffer> m_forwardFramebuffer;
};

