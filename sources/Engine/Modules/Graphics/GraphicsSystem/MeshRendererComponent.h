#pragma once

#include <memory>
#include <vector>

#include "Modules/Graphics/Resources/MeshResourceManager.h"

#include "Transform.h"
#include "Modules/Graphics/OpenGL/GLMaterial.h"

struct MeshRenderingAttributes {
};

class MeshRendererComponent {
 public:
  MeshRendererComponent();

  void setMeshInstance(ResourceHandle<Mesh> instance);
  [[nodiscard]] ResourceHandle<Mesh> getMeshInstance() const;

  void setMaterialsInstances(const std::vector<ResourceHandle<GLMaterial>>& instances);
  void setMaterialInstance(size_t subMeshIndex, ResourceHandle<GLMaterial> instance);

  [[nodiscard]] ResourceHandle<GLMaterial> getMaterialInstance(size_t subMeshIndex) const;

  [[nodiscard]] const MeshRenderingAttributes& getAttributes() const;
  [[nodiscard]] MeshRenderingAttributes& getAttributes();

 private:
  ResourceHandle<Mesh> m_meshInstance;
  std::vector<ResourceHandle<GLMaterial>> m_materialsInstances;

  MeshRenderingAttributes m_renderingAttributes;
};

