#include "precompiled.h"

#pragma hdrstop

#include "Mesh.h"
#include "Exceptions/exceptions.h"

Mesh::Mesh() {

}

Mesh::~Mesh() {

}

void Mesh::setVertices(const std::vector<glm::vec3>& vertices) {
  SW_ASSERT(m_vertices.size() == 0);

  m_vertices = vertices;
  m_needGeometryBufferUpdate = true;
}

size_t Mesh::addSubMesh(const std::vector<uint16_t>& indices) {
  SW_ASSERT(m_vertices.size() > 0);

  m_needGeometryBufferUpdate = true;

  m_indices.push_back(indices);
  calculateSubMeshesOffsets();

  return m_indices.size() - 1;
}

void Mesh::setIndices(const std::vector<uint16_t>& indices, size_t subMeshIndex) {
  SW_ASSERT(subMeshIndex < m_indices.size());
  SW_ASSERT(m_vertices.size() > 0);

  m_needGeometryBufferUpdate = true;
  calculateSubMeshesOffsets();

  m_indices[subMeshIndex] = indices;
}

void Mesh::setNormals(const std::vector<glm::vec3>& normals) {
  SW_ASSERT(m_vertices.size() > 0 && normals.size() == m_vertices.size());

  m_needGeometryBufferUpdate = true;

  m_normals = normals;
}

void Mesh::setTangents(const std::vector<glm::vec3>& tangents) {
  SW_ASSERT(m_vertices.size() > 0 && tangents.size() == m_vertices.size());

  m_needGeometryBufferUpdate = true;

  m_tangents = tangents;
}

void Mesh::setUV(const std::vector<glm::vec2>& uv) {
  SW_ASSERT(m_vertices.size() > 0 && uv.size() == m_vertices.size());

  m_needGeometryBufferUpdate = true;

  m_uv = uv;
}

void Mesh::setSkinData(const std::vector<glm::u8vec4>& bonesIDs, const std::vector<glm::u8vec4>& bonesWeights) {
  SW_ASSERT(m_vertices.size() > 0 && bonesIDs.size() == m_vertices.size() &&
      bonesWeights.size() == m_vertices.size());

  m_bonesIDs = bonesIDs;
  m_bonesWeights = bonesWeights;
}

bool Mesh::hasVertices() const {
  return m_vertices.size() > 0;
}

bool Mesh::hasNormals() const {
  return m_normals.size() > 0;
}

bool Mesh::hasTangents() const {
  return m_tangents.size() > 0;
}

bool Mesh::hasUV() const {
  return m_uv.size() > 0;
}

bool Mesh::isSkinned() const {
  return m_bonesIDs.size() > 0;
}

bool Mesh::hasSkeleton() const {
  return m_skeleton != nullptr;
}

void Mesh::setSubMeshesIndices(const std::vector<uint16_t>& indices, const std::vector<uint16_t>& subMeshesOffsets) {
  for (size_t subMeshIndex = 0; subMeshIndex < subMeshesOffsets.size(); subMeshIndex++) {
    auto startIt = indices.begin() + subMeshesOffsets[subMeshIndex];
    auto endIt = (subMeshIndex == subMeshesOffsets.size() - 1) ? indices.end() :
        indices.begin() + (subMeshesOffsets[subMeshIndex + 1] - 1);

    RETURN_VALUE_UNUSED(addSubMesh(std::vector<uint16_t>(startIt, endIt)));
  }
}

size_t Mesh::getSubMeshesCount() const {
  return m_indices.size();
}

size_t Mesh::getSubMeshIndicesOffset(size_t subMeshIndex) const {
  SW_ASSERT(subMeshIndex < m_indices.size());

  return m_subMeshesOffsets[subMeshIndex];
}

size_t Mesh::getSubMeshIndicesCount(size_t subMeshIndex) const {
  SW_ASSERT(subMeshIndex < m_indices.size());

  return m_indices[subMeshIndex].size();
}

GLGeometryStore* Mesh::getGeometryStore() {
  updateGeometryBuffer();

  return m_geometryStore.get();
}

void Mesh::setAABB(const AABB& aabb) {
  m_aabb = aabb;
}

const AABB& Mesh::getAABB() const {
  return m_aabb;
}

void Mesh::setSkeleton(std::shared_ptr<Skeleton> skeleton) {
  m_skeleton = skeleton;
}

std::shared_ptr<Skeleton> Mesh::getSkeleton() const {
  return m_skeleton;
}

void Mesh::calculateSubMeshesOffsets() {
  m_subMeshesOffsets.clear();
  m_subMeshesOffsets.push_back(0);

  for (size_t subMeshIndex = 1; subMeshIndex < m_indices.size(); subMeshIndex++) {
    m_subMeshesOffsets.push_back(m_subMeshesOffsets[subMeshIndex - 1] + m_indices[subMeshIndex - 1].size());
  }
}

template<>
std::vector<VertexPos3Norm3UV> Mesh::constructVerticesList() const {
  std::vector<VertexPos3Norm3UV> vertices(m_vertices.size());

  for (size_t vertexIndex = 0; vertexIndex < m_vertices.size(); vertexIndex++) {
    vertices[vertexIndex].pos = m_vertices[vertexIndex];
    vertices[vertexIndex].norm = m_normals[vertexIndex];
    vertices[vertexIndex].uv = m_uv[vertexIndex];
  }

  return vertices;
}

template<>
std::vector<VertexPos3Norm3UVSkinned> Mesh::constructVerticesList() const {
  std::vector<VertexPos3Norm3UVSkinned> vertices(m_vertices.size());

  for (size_t vertexIndex = 0; vertexIndex < m_vertices.size(); vertexIndex++) {
    vertices[vertexIndex].pos = m_vertices[vertexIndex];
    vertices[vertexIndex].norm = m_normals[vertexIndex];
    vertices[vertexIndex].uv = m_uv[vertexIndex];
    vertices[vertexIndex].bonesIds = m_bonesIDs[vertexIndex];
    vertices[vertexIndex].bonesWeights = m_bonesWeights[vertexIndex];
  }

  return vertices;
}

void Mesh::updateGeometryBuffer() {
  if (!m_needGeometryBufferUpdate) {
    return;
  }

  std::vector<uint16_t> indices;

  for (const auto& subMeshIndices : m_indices) {
    indices.insert(indices.end(), subMeshIndices.begin(), subMeshIndices.end());
  }

  GLGeometryStore* geometryStore = nullptr;

  MeshAttributes meshAttributesMask = MeshAttributes::Empty;

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
    SW_ASSERT(m_bonesIDs.size() > 0 && m_bonesWeights.size() > 0);

    meshAttributesMask = meshAttributesMask | MeshAttributes::BonesIDs | MeshAttributes::BonesWeights;
  }

  if (meshAttributesMask == (MeshAttributes::Positions | MeshAttributes::Normals | MeshAttributes::UV)) {
    geometryStore = new GLGeometryStore(constructVerticesList<VertexPos3Norm3UV>(), indices);
  }
  else if (meshAttributesMask == (MeshAttributes::Positions | MeshAttributes::Normals | MeshAttributes::UV |
      MeshAttributes::BonesIDs | MeshAttributes::BonesWeights)) {
    geometryStore = new GLGeometryStore(constructVerticesList<VertexPos3Norm3UVSkinned>(), indices);
  }

  if (geometryStore == nullptr) {
      THROW_EXCEPTION(EngineRuntimeException, "Unsupported vertex buffer layout");
  }

  m_geometryStore.reset(geometryStore);
  m_needGeometryBufferUpdate = false;
}
