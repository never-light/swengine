#pragma once

#include <memory>

#include "Modules/ResourceManagement/ResourcesManagement.h"
#include "Modules/Graphics/OpenGL/GLGraphicsContext.h"

#include "Modules/Graphics/OpenGL/GLMaterial.h"
#include "Modules/Graphics/OpenGL/Mesh.h"
#include "FrameStats.h"
#include "RenderingSystem.h"

class EnvironmentComponent {
 public:
  EnvironmentComponent();

  void setEnvironmentMaterial(ResourceHandle<GLMaterial> material);
  [[nodiscard]] GLMaterial* getEnvironmentMaterial() const;

 private:
  ResourceHandle<GLMaterial> m_environmentMaterial;
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
  void render() override;

 private:
  ResourceHandle<Mesh> m_environmentMesh;
};


