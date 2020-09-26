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

  void setEnvironmentMaterial(std::shared_ptr<Material> material);
  [[nodiscard]] Material* getEnvironmentMaterial() const;

 private:
  std::shared_ptr<Material> m_environmentMaterial;
};

class EnvironmentRenderingSystem : public RenderingSystem {
 public:
  EnvironmentRenderingSystem(
    std::shared_ptr<GLGraphicsContext> graphicsContext,
    std::shared_ptr<GraphicsScene> graphicsScene,
    std::shared_ptr<Mesh> environmentMesh);

  ~EnvironmentRenderingSystem() override;

  void configure() override;
  void unconfigure() override;

  void update(float delta) override;
  void renderForward() override;

 private:
  std::shared_ptr<Mesh> m_environmentMesh;
};


