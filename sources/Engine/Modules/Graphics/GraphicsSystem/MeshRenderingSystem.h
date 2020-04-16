#pragma once

#include <memory>

#include "Modules/Graphics/OpenGL/GLGraphicsContext.h"
#include "SharedGraphicsState.h"
#include "RenderingSystem.h"

class MeshRenderingSystem : public RenderingSystem {
 public:
  MeshRenderingSystem(std::shared_ptr<GLGraphicsContext> graphicsContext,
                      std::shared_ptr<SharedGraphicsState> sharedGraphicsState);

  ~MeshRenderingSystem() override;

  void configure(GameWorld* gameWorld) override;
  void unconfigure(GameWorld* gameWorld) override;

  void update(GameWorld* gameWorld, float delta) override;

  void renderForward(GameWorld* gameWorld) override;
  void renderDeferred(GameWorld* gameWorld) override;
};
