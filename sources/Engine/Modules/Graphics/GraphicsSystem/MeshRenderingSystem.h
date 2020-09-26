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

  void renderForward() override;
  void renderDeferred() override;

};
