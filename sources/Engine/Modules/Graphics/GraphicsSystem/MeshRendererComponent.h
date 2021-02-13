#pragma once

#include <memory>
#include <vector>
#include <cereal/types/vector.hpp>
#include <cereal/types/utility.hpp>

#include "Modules/Graphics/Resources/MeshResourceManager.h"

#include "Transform.h"
#include "Modules/Graphics/OpenGL/GLMaterial.h"
#include "Modules/ECS/GameObjectsFactory.h"

template<class Archive>
inline void serialize(Archive& archive, std::pair<std::string, size_t>& p)
{
  archive(p.first, p.second);
}

class MeshRendererComponentBindingParameters {
 public:
  std::string meshResourceName;
  std::string skeletonResourceName;

  std::vector<std::pair<std::string, size_t>> materials;

  template<class Archive>
  void serialize(Archive& archive)
  {
    archive(
      cereal::make_nvp("mesh_resource", meshResourceName),
      cereal::make_nvp("skeleton_resource", skeletonResourceName),
      cereal::make_nvp("submeshes_materials", materials));
  };

};

struct MeshRenderingAttributes {
};

class MeshRendererComponent {
 public:
  static constexpr bool s_isSerializable = true;
  using BindingParameters = MeshRendererComponentBindingParameters;

 public:
  MeshRendererComponent();

  void setMeshInstance(ResourceHandle<Mesh> instance);
  [[nodiscard]] ResourceHandle<Mesh> getMeshInstance() const;

  void setMaterialsInstances(const std::vector<ResourceHandle<GLMaterial>>& instances);
  void setMaterialInstance(size_t subMeshIndex, ResourceHandle<GLMaterial> instance);

  [[nodiscard]] ResourceHandle<GLMaterial> getMaterialInstance(size_t subMeshIndex) const;

  [[nodiscard]] const MeshRenderingAttributes& getAttributes() const;
  [[nodiscard]] MeshRenderingAttributes& getAttributes();

  [[nodiscard]] BindingParameters getBindingParameters() const;

 private:
  ResourceHandle<Mesh> m_meshInstance;
  std::vector<ResourceHandle<GLMaterial>> m_materialsInstances;

  MeshRenderingAttributes m_renderingAttributes;
};

class MeshRendererComponentBinder : public GameObjectsComponentBinder<MeshRendererComponent> {
 public:
  explicit MeshRendererComponentBinder(const ComponentBindingParameters& componentParameters,
    std::shared_ptr<ResourcesManager> resourcesManager);

  void bindToObject(GameObject& gameObject) override;

 private:
  ComponentBindingParameters m_bindingParameters;
  std::shared_ptr<ResourcesManager> m_resourcesManager;
};
