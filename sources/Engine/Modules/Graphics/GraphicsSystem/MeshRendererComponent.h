#pragma once

#include <memory>
#include <vector>

#include "Modules/Graphics/Resources/MeshResource.h"
#include "Modules/ResourceManagement/ResourceInstance.h"

#include "Transform.h"
#include "Material.h"

class MeshRendererComponent {
 public:
  MeshRendererComponent();

  void setMeshInstance(std::shared_ptr<Mesh> instance);
  [[nodiscard]] std::shared_ptr<Mesh> getMeshInstance() const;

  void setMaterialsInstances(const std::vector<std::shared_ptr<Material>>& instances);
  void setMaterialInstance(size_t subMeshIndex, std::shared_ptr<Material> instance);

  [[nodiscard]] std::shared_ptr<Material> getMaterialInstance(size_t subMeshIndex) const;

  void cull(bool culled = true);
  [[nodiscard]] bool isCulled() const;

  void updateBounds(const glm::mat4& transformation);

  [[nodiscard]] const AABB& getAABB() const;

 private:
  std::shared_ptr<Mesh> m_meshInstance;
  std::vector<std::shared_ptr<Material>> m_materialsInstances;

  bool m_isCulled = false;

  AABB m_aabb;
};

