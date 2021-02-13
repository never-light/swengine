#pragma once

#include <memory>

#include "Modules/ResourceManagement/ResourcesManagement.h"
#include "Modules/Graphics/OpenGL/GLGraphicsContext.h"
#include "Modules/ECS/GameObjectsFactory.h"

#include "Modules/Graphics/OpenGL/GLMaterial.h"
#include "Modules/Graphics/OpenGL/Mesh.h"
#include "FrameStats.h"
#include "RenderingSystem.h"

class EnvironmentComponentBindingParameters {
 public:
  std::string materialResourceName;


  template<class Archive>
  void serialize(Archive& archive)
  {
    archive(
      cereal::make_nvp("material_resource", materialResourceName));
  };
};

class EnvironmentComponent {
 public:
  static constexpr bool s_isSerializable = true;
  using BindingParameters = EnvironmentComponentBindingParameters;

 public:
  EnvironmentComponent();

  void setEnvironmentMaterial(ResourceHandle<GLMaterial> material);
  [[nodiscard]] GLMaterial* getEnvironmentMaterial() const;

  [[nodiscard]] BindingParameters getBindingParameters() const;
  
 private:
  ResourceHandle<GLMaterial> m_environmentMaterial;
};

class EnvironmentComponentBinder : public GameObjectsComponentBinder<EnvironmentComponent> {
 public:
  explicit EnvironmentComponentBinder(const ComponentBindingParameters& componentParameters,
    std::shared_ptr<ResourcesManager> resourcesManager);

  void bindToObject(GameObject& gameObject) override;

 private:
  ComponentBindingParameters m_bindingParameters;
  std::shared_ptr<ResourcesManager> m_resourcesManager;
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


