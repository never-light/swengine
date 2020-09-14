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

  void configure() override;
  void unconfigure() override;

  void update(float delta) override;

  void beforeRender() override;
  void afterRender() override;
};
