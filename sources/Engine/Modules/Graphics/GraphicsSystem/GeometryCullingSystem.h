#pragma once

#include <memory>

#include "Modules/Graphics/OpenGL/GLGraphicsContext.h"
#include "SharedGraphicsState.h"
#include "RenderingSystem.h"

class GeometryCullingSystem : public RenderingSystem {
 public:
  GeometryCullingSystem(std::shared_ptr<GLGraphicsContext> graphicsContext,
    std::shared_ptr<SharedGraphicsState> sharedGraphicsState);

  ~GeometryCullingSystem() override;

  void configure(GameWorld* gameWorld) override;
  void unconfigure(GameWorld* gameWorld) override;

  void update(GameWorld* gameWorld, float delta) override;

  void beforeRender(GameWorld* gameWorld) override;
  void afterRender(GameWorld* gameWorld) override;
};
