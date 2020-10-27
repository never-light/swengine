#pragma once

#include <memory>
#include <vector>

#include "Modules/Graphics/Resources/MeshResourceManager.h"

#include "Transform.h"
#include "Material.h"

struct MeshRenderingAttributes {
};

class MeshRendererComponent {
 public:
  MeshRendererComponent();

  void setMeshInstance(ResourceHandle<Mesh> instance);
  [[nodiscard]] ResourceHandle<Mesh> getMeshInstance() const;

  void setMaterialsInstances(const std::vector<ResourceHandle<Material>>& instances);
  void setMaterialInstance(size_t subMeshIndex, ResourceHandle<Material> instance);

  [[nodiscard]] ResourceHandle<Material> getMaterialInstance(size_t subMeshIndex) const;

  [[nodiscard]] const MeshRenderingAttributes& getAttributes() const;
  [[nodiscard]] MeshRenderingAttributes& getAttributes();

 private:
  ResourceHandle<Mesh> m_meshInstance;
  std::vector<ResourceHandle<Material>> m_materialsInstances;

  MeshRenderingAttributes m_renderingAttributes;
};

