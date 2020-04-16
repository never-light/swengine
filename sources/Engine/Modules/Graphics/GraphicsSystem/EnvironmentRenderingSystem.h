#pragma once

#include <memory>

#include "Modules/Graphics/OpenGL/GLGraphicsContext.h"

#include "Material.h"
#include "Mesh.h"
#include "SharedGraphicsState.h"
#include "RenderingSystem.h"

class EnvironmentComponent {
 public:
  EnvironmentComponent();

  void setEnvironmentMaterial(const std::shared_ptr<Material> material);
  [[nodiscard]] Material* getEnvironmentMaterial() const;

 private:
  std::shared_ptr<Material> m_environmentMaterial;
};

class EnvironmentRenderingSystem : public RenderingSystem {
 public:
  EnvironmentRenderingSystem(std::shared_ptr<GLGraphicsContext> graphicsContext,
                             std::shared_ptr<SharedGraphicsState> sharedGraphicsState,
                             std::shared_ptr<Mesh> environmentMesh);

  ~EnvironmentRenderingSystem() override;

  void configure(GameWorld* gameWorld) override;
  void unconfigure(GameWorld* gameWorld) override;

  void update(GameWorld* gameWorld, float delta) override;
  void renderForward(GameWorld* gameWorld) override;

 private:
  std::shared_ptr<Mesh> m_environmentMesh;
};


