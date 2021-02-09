#include "precompiled.h"

#pragma hdrstop

#include "Mesh.h"
#include "Exceptions/exceptions.h"

Mesh::Mesh(bool isDynamic, size_t minStorageCapacity)
  : m_isDynamic(isDynamic),
    m_minStorageCapacity(minStorageCapacity)
{

}

Mesh::~Mesh() = default;

void Mesh::setVertices(const std::vector<glm::vec3>& vertices)
{
  SW_ASSERT(m_geometryStore == nullptr || m_isDynamic &&
    !m_vertices.empty());

  m_vertices = vertices;
  setAttributeOutdated(MeshAttributes::Positions);
}

void Mesh::addSubMesh(const std::vector<uint16_t>& indices)
{
  SW_ASSERT(m_geometryStore == nullptr && "Sub-mesh adding after geometry buffer formation is forbidden");
  SW_ASSERT(!m_vertices.empty());

  m_needGeometryBufferUpdate = true;

  m_indices.push_back(indices);
  calculateSubMeshesOffsets();
}

void Mesh::setIndices(const std::vector<uint16_t>& indices, size_t subMeshIndex)
{
  SW_ASSERT(subMeshIndex < m_indices.size());
  SW_ASSERT(m_geometryStore == nullptr || !m_indices.empty());

  m_needGeometryBufferUpdate = true;
  m_needUpdateIndices = true;

  m_indices[subMeshIndex] = indices;

  calculateSubMeshesOffsets();
}

void Mesh::setNormals(const std::vector<glm::vec3>& normals)
{
  SW_ASSERT(m_geometryStore == nullptr || m_isDynamic &&
    !m_normals.empty());

  m_normals = normals;
  setAttributeOutdated(MeshAttributes::Normals);
}

void Mesh::setTangents(const std::vector<glm::vec3>& tangents)
{
  SW_ASSERT(m_geometryStore == nullptr || m_isDynamic &&
    !m_tangents.empty() &&
    tangents.size() <= m_geometryStore->getVerticesCapacity());

  m_tangents = tangents;
  setAttributeOutdated(MeshAttributes::Tangents);
}

void Mesh::setUV(const std::vector<glm::vec2>& uv)
{
  SW_ASSERT(m_geometryStore == nullptr || m_isDynamic &&
    !m_uv.empty());

  m_uv = uv;
  setAttributeOutdated(MeshAttributes::UV);
}

void Mesh::setSkinData(const std::vector<glm::u8vec4>& bonesIDs, const std::vector<glm::u8vec4>& bonesWeights)
{
  SW_ASSERT(m_geometryStore == nullptr || m_isDynamic &&
    !m_bonesIDs.empty());

  SW_ASSERT(m_geometryStore == nullptr || m_isDynamic &&
    !m_bonesWeights.empty());

  m_bonesIDs = bonesIDs;
  m_bonesWeights = bonesWeights;
}

bool Mesh::hasVertices() const
{
  return !m_vertices.empty();
}

bool Mesh::hasNormals() const
{
  return !m_normals.empty();
}

bool Mesh::hasTangents() const
{
  return !m_tangents.empty();
}

bool Mesh::hasUV() const
{
  return !m_uv.empty();
}

bool Mesh::isSkinned() const
{
  return !m_bonesIDs.empty();
}

bool Mesh::hasSkeleton() const
{
  return m_skeleton.has_value();
}

size_t Mesh::getSubMeshesCount() const
{
  return m_indices.size();
}

size_t Mesh::getSubMeshIndicesOffset(size_t subMeshIndex) const
{
  SW_ASSERT(subMeshIndex < m_indices.size());

  return m_subMeshesIndicesOffsets[subMeshIndex];
}

size_t Mesh::getSubMeshIndicesCount(size_t subMeshIndex) const
{
  SW_ASSERT(subMeshIndex < m_indices.size());

  return m_indices[subMeshIndex].size();
}

GLGeometryStore* Mesh::getGeometryStore()
{
  updateGeometryBuffer();

  return m_geometryStore.get();
}

void Mesh::setAABB(const AABB& aabb)
{
  m_aabb = aabb;
}

const AABB& Mesh::getAABB() const
{
  return m_aabb;
}

void Mesh::setSkeleton(ResourceHandle<Skeleton> skeleton)
{
  m_skeleton = skeleton;
}

ResourceHandle<Skeleton> Mesh::getSkeleton() const
{
  return m_skeleton.value();
}

void Mesh::calculateSubMeshesOffsets()
{
  uint32_t partialIndicesSum = 0;

  m_subMeshesIndicesOffsets.resize(m_indices.size());

  for (size_t subMeshIndex = 0; subMeshIndex < m_indices.size(); subMeshIndex++) {
    m_subMeshesIndicesOffsets[subMeshIndex] = partialIndicesSum;
    partialIndicesSum += static_cast<uint32_t>(m_indices[subMeshIndex].size());
  }
}

void Mesh::updateGeometryBuffer()
{
  if (!m_needGeometryBufferUpdate) {
    return;
  }

  MeshAttributesSet meshAttributesMask = MeshAttributes::Empty;

  if (hasVertices()) {
    meshAttributesMask = meshAttributesMask | MeshAttributes::Positions;
  }

  if (hasNormals()) {
    meshAttributesMask = meshAttributesMask | MeshAttributes::Normals;
  }

  if (hasTangents()) {
    meshAttributesMask = meshAttributesMask | MeshAttributes::Tangents;
  }

  if (hasUV()) {
    meshAttributesMask = meshAttributesMask | MeshAttributes::UV;
  }

  if (isSkinned()) {
    SW_ASSERT(!m_bonesIDs.empty() && !m_bonesWeights.empty());

    meshAttributesMask = meshAttributesMask | MeshAttributes::BonesIDs | MeshAttributes::BonesWeights;
  }

  std::vector<uint16_t> indices;

  if (m_geometryStore == nullptr || m_needUpdateIndices) {
    for (const auto& subMeshIndices : m_indices) {
      indices.insert(indices.end(), subMeshIndices.begin(), subMeshIndices.end());
    }
  }

  if (m_geometryStore == nullptr) {
    GLenum storageFlags = GL_NONE;

    if (m_isDynamic) {
      storageFlags |= GL_DYNAMIC_STORAGE_BIT;
    }

    if (meshAttributesMask == MESH_FORMAT_POS_NORM_UV || meshAttributesMask == MESH_FORMAT_POS_NORM_TAN_UV) {
      if (meshAttributesMask == MESH_FORMAT_POS_NORM_TAN_UV) {
        spdlog::warn("Tangents attributes for a mesh will be ignored");
      }

      //geometryStore = new GLGeometryStore(constructVerticesList<VertexPos3Norm3UV>(), indices);
      m_geometryStore = std::make_unique<GLGeometryStore>(VerticesPos3Norm3UVSoA{
        .positions = &m_vertices,
        .normals = &m_normals,
        .uv = &m_uv
      }, indices, storageFlags, m_minStorageCapacity);
    }
    else if (meshAttributesMask == MESH_FORMAT_POS_NORM_UV_SKINNED) {
      m_geometryStore = std::make_unique<GLGeometryStore>(VertexPos3Norm3UVSkinnedSoA{
        .positions = &m_vertices,
        .normals = &m_normals,
        .uv = &m_uv,
        .bonesIds = &m_bonesIDs,
        .bonesWeights = &m_bonesWeights
      }, indices, storageFlags, m_minStorageCapacity);
    }
    else {
      THROW_EXCEPTION(EngineRuntimeException, "Unsupported vertex buffer layout");
    }
  }
  else {
    if (m_needUpdateAttributes.count() > 0) {
      if (meshAttributesMask == MESH_FORMAT_POS_NORM_UV) {
        SW_ASSERT(m_vertices.size() == m_normals.size() &&
          m_vertices.size() == m_uv.size());

        m_geometryStore->updateVertices(VerticesPos3Norm3UVSoA{
          .positions = (m_needUpdateAttributes[size_t(MeshAttributes::Positions)]) ? &m_vertices : nullptr,
          .normals = (m_needUpdateAttributes[size_t(MeshAttributes::Normals)]) ? &m_normals : nullptr,
          .uv = (m_needUpdateAttributes[size_t(MeshAttributes::UV)]) ? &m_uv : nullptr
        });
      }
      else if (meshAttributesMask == MESH_FORMAT_POS_NORM_UV_SKINNED) {
        SW_ASSERT(m_vertices.size() == m_normals.size() &&
          m_vertices.size() == m_uv.size() &&
          m_vertices.size() == m_bonesIDs.size() &&
          m_vertices.size() == m_bonesWeights.size());

        m_geometryStore = std::make_unique<GLGeometryStore>(VertexPos3Norm3UVSkinnedSoA{
          .positions = (m_needUpdateAttributes[size_t(MeshAttributes::Positions)]) ? &m_vertices : nullptr,
          .normals = (m_needUpdateAttributes[size_t(MeshAttributes::Normals)]) ? &m_normals : nullptr,
          .uv = (m_needUpdateAttributes[size_t(MeshAttributes::UV)]) ? &m_uv : nullptr,
          .bonesIds = (m_needUpdateAttributes[size_t(MeshAttributes::BonesIDs)]) ? &m_bonesIDs : nullptr,
          .bonesWeights = (m_needUpdateAttributes[size_t(MeshAttributes::BonesWeights)]) ? &m_bonesWeights : nullptr,
        });
      }

      m_needUpdateAttributes.reset();
    }

    if (m_needUpdateIndices) {
      m_geometryStore->updateIndices(indices);
      m_needUpdateIndices = false;
    }
  }

  m_needGeometryBufferUpdate = false;
}

void Mesh::setAttributeOutdated(MeshAttributes attribute, bool isOutdated)
{
  m_needUpdateAttributes[size_t(attribute)] = isOutdated;
  m_needGeometryBufferUpdate = true;
}
