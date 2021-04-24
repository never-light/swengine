#pragma once

#include <memory>

#include "Modules/Graphics/OpenGL/GLGraphicsContext.h"
#include "RenderingSystem.h"

class MeshRenderingSystem : public RenderingSystem {
 public:
  MeshRenderingSystem(
    std::shared_ptr<GLGraphicsContext> graphicsContext,
    std::shared_ptr<GraphicsScene> graphicsScene);

  ~MeshRenderingSystem() override;

  void configure() override;
  void unconfigure() override;

  void update(float delta) override;
  void render() override;

  void enableBoundsRendering(bool isEnabled = true);
  [[nodiscard]] bool isBoundsRenderingEnabled() const;

  void enableSkeletonsRendering(bool isEnabled = true);
  [[nodiscard]] bool isSkeletonsRenderingEnabled() const;

 private:
  void debugDrawSkeleton(GameObject& gameObject);

 private:
  bool m_isBoundsRenderingEnabled{};
  bool m_isSkeletonsRenderingEnabled{};

  float m_skeletonsRenderingOffset = 0.5f;
};
