#pragma once

#include <memory>

#include "Modules/ECS/GameSystemsGroup.h"
#include "RenderingSystem.h"
#include "Material.h"

class RenderingSystemsPipeline : public GameSystemsGroup {
 public:
  RenderingSystemsPipeline(std::shared_ptr<GameWorld> gameWorld,
                           std::shared_ptr<GLGraphicsContext> graphicsContext,
                           std::shared_ptr<SharedGraphicsState> sharedGraphicsState);

  void addGameSystem(std::shared_ptr<GameSystem> system) override;

  void render(GameWorld* gameWorld) override;

  void setDeferredAccumulationShadersPipeline(std::shared_ptr<GLShadersPipeline> pipeline);
  [[nodiscard]] std::shared_ptr<GLShadersPipeline> getDeferredAccumulationShadersPipeline() const;

 private:
  std::shared_ptr<GLGraphicsContext> m_graphicsContext;
  std::shared_ptr<SharedGraphicsState> m_sharedGraphicsState;

  std::shared_ptr<Material> m_deferredAccumulationMaterial;
};

