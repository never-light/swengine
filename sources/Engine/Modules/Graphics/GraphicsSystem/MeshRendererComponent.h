#pragma once

#include <memory>
#include <vector>

#include "Modules/Graphics/Resources/MeshResource.h"
#include "Modules/ResourceManagement/ResourceInstance.h"

#include "Transform.h"
#include "Material.h"

struct MeshRenderingAttributes {
  bool isStatic = true;
};

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
  void updateBounds(const glm::vec3& origin, const glm::quat& orientation);

  [[nodiscard]] const AABB& getBoundingBox() const;
  [[nodiscard]] const Sphere& getBoundingSphere() const;

  [[nodiscard]] const MeshRenderingAttributes& getAttributes() const;
  [[nodiscard]] MeshRenderingAttributes& getAttributes();

 private:
  std::shared_ptr<Mesh> m_meshInstance;
  std::vector<std::shared_ptr<Material>> m_materialsInstances;

  bool m_isCulled = false;

  MeshRenderingAttributes m_renderingAttributes;

  AABB m_boundingBox;
  Sphere m_boundingSphere;
};

