#pragma once

#include <memory>

#include "Modules/ResourceManagement/ResourceManager.h"
#include "Modules/Graphics/OpenGL/GLGraphicsContext.h"

#include "Material.h"
#include "Mesh.h"
#include "SharedGraphicsState.h"
#include "RenderingSystem.h"

class EnvironmentComponent {
 public:
  EnvironmentComponent();

  void setEnvironmentMaterial(ResourceHandle<Material> material);
  [[nodiscard]] Material* getEnvironmentMaterial() const;

 private:
  ResourceHandle<Material> m_environmentMaterial;
};

class EnvironmentRenderingSystem : public RenderingSystem {
 public:
  EnvironmentRenderingSystem(
    std::shared_ptr<GLGraphicsContext> graphicsContext,
    std::shared_ptr<GraphicsScene> graphicsScene,
    ResourceHandle<Mesh> environmentMesh);

  ~EnvironmentRenderingSystem() override;

  void configure() override;
  void unconfigure() override;

  void update(float delta) override;
  void renderForward() override;

 private:
  ResourceHandle<Mesh> m_environmentMesh;
};


