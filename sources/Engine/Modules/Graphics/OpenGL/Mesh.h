#pragma once

#include <vector>
#include <utility>
#include <memory>
#include <optional>
#include <bitset>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Modules/ResourceManagement/ResourcesManagement.h"
#include "Modules/Graphics/OpenGL/GLGeometryStore.h"
#include "Modules/Graphics/GraphicsSystem/Animation/Skeleton.h"
#include "Modules/Math/geometry.h"

enum class MeshAttributes {
  Empty = 0,
  Positions = 1,
  Normals = 2,
  UV = 4,
  Tangents = 8,
  BonesIDs = 16,
  BonesWeights = 32,
};

inline constexpr MeshAttributes operator|(MeshAttributes a, MeshAttributes b)
{
  return static_cast<MeshAttributes>(static_cast<unsigned int>(a) | static_cast<unsigned int>(b));
}

inline constexpr MeshAttributes operator&(MeshAttributes a, MeshAttributes b)
{
  return static_cast<MeshAttributes>(static_cast<unsigned int>(a) & static_cast<unsigned int>(b));
}

inline constexpr MeshAttributes operator~(MeshAttributes a)
{
  return static_cast<MeshAttributes>(~static_cast<unsigned int>(a));
}

using MeshAttributesSet = MeshAttributes;

class Mesh : public Resource {
 public:
  explicit Mesh(bool isDynamic = false, size_t minStorageCapacity = 0);
  ~Mesh() override;

  [[nodiscard]] size_t addSubMesh(const std::vector<uint16_t>& indices);
  void setIndices(const std::vector<uint16_t>& indices, size_t subMeshIndex);

  void setVertices(const std::vector<glm::vec3>& vertices);
  void setNormals(const std::vector<glm::vec3>& normals);
  void setTangents(const std::vector<glm::vec3>& tangents);
  void setUV(const std::vector<glm::vec2>& uv);

  void setSkinData(const std::vector<glm::u8vec4>& bonesIDs, const std::vector<glm::u8vec4>& bonesWeights);

  [[nodiscard]] bool hasVertices() const;
  [[nodiscard]] bool hasNormals() const;
  [[nodiscard]] bool hasTangents() const;
  [[nodiscard]] bool hasUV() const;
  [[nodiscard]] bool isSkinned() const;
  [[nodiscard]] bool hasSkeleton() const;

  void setSubMeshesIndices(const std::vector<uint16_t>& indices, const std::vector<uint16_t>& subMeshesOffsets);

  [[nodiscard]] size_t getSubMeshesCount() const;
  [[nodiscard]] size_t getSubMeshIndicesOffset(size_t subMeshIndex) const;
  [[nodiscard]] size_t getSubMeshIndicesCount(size_t subMeshIndex) const;

  GLGeometryStore* getGeometryStore();

  void setAABB(const AABB& aabb);
  [[nodiscard]] const AABB& getAABB() const;

  void setSkeleton(ResourceHandle<Skeleton> skeleton);
  [[nodiscard]] ResourceHandle<Skeleton> getSkeleton() const;

 private:
  void calculateSubMeshesOffsets();

  void updateGeometryBuffer();

  void setAttributeOutdated(MeshAttributes attribute, bool isOutdated = true);

 private:
  std::unique_ptr<GLGeometryStore> m_geometryStore;

  std::vector<glm::vec3> m_vertices;

  std::vector<std::vector<uint16_t>> m_indices;
  std::vector<size_t> m_subMeshesOffsets;

  std::vector<glm::vec3> m_normals;
  std::vector<glm::vec3> m_tangents;
  std::vector<glm::vec2> m_uv;
  std::vector<glm::u8vec4> m_bonesIDs;
  std::vector<glm::u8vec4> m_bonesWeights;

  bool m_needGeometryBufferUpdate = false;

  AABB m_aabb;

  std::optional<ResourceHandle<Skeleton>> m_skeleton;
  bool m_isDynamic = false;

  std::bitset<32> m_needUpdateAttributes;
  bool m_needUpdateIndices = false;

  size_t m_minStorageCapacity = 0;

 private:
  static constexpr MeshAttributesSet MESH_FORMAT_POS_NORM_UV =
    MeshAttributes::Positions | MeshAttributes::Normals | MeshAttributes::UV;

  static constexpr MeshAttributesSet MESH_FORMAT_POS_NORM_TAN_UV =
    MeshAttributes::Positions | MeshAttributes::Normals | MeshAttributes::Tangents | MeshAttributes::UV;

  static constexpr MeshAttributesSet MESH_FORMAT_POS_NORM_UV_SKINNED =
    MeshAttributes::Positions | MeshAttributes::Normals | MeshAttributes::UV |
      MeshAttributes::BonesIDs | MeshAttributes::BonesWeights;
};
