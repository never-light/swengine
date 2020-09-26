#pragma once

#include <memory>

#include "Modules/ECS/GameSystem.h"
#include "Modules/Graphics/OpenGL/GLGraphicsContext.h"
#include "GraphicsScene.h"

class RenderingSystem : public GameSystem {
 public:
  RenderingSystem(std::shared_ptr<GLGraphicsContext> graphicsContext,
    std::shared_ptr<GraphicsScene> graphicsScene);

  virtual void renderDeferred();
  virtual void renderForward();
  virtual void renderPostProcess();

  void render() final;

 protected:
  std::shared_ptr<GLGraphicsContext> m_graphicsContext;
  std::shared_ptr<GraphicsScene> m_graphicsScene;
};

