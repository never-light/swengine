#include "precompiled.h"

#pragma hdrstop

#include "RawMesh.h"
#include "Exceptions/exceptions.h"

RawMesh RawMesh::readFromFile(const std::string& path)
{
  RawMesh rawMesh;

  std::ifstream meshFile(path, std::ios::binary);
  meshFile.read(reinterpret_cast<char*>(&rawMesh.header), sizeof(rawMesh.header));

  if (rawMesh.header.formatVersion != MESH_FORMAT_VERSION) {
    THROW_EXCEPTION(EngineRuntimeException, "Trying to load mesh with incompatible format version");
  }

  if (rawMesh.header.verticesCount == 0) {
    THROW_EXCEPTION(EngineRuntimeException, "Trying to load mesh with zero vertices count");
  }

  const uint16_t verticesCount = rawMesh.header.verticesCount;
  RawMeshAttributes storedAttributesMask = static_cast<RawMeshAttributes>(rawMesh.header.storedAttributesMask);

  if ((storedAttributesMask & RawMeshAttributes::Positions) != RawMeshAttributes::Empty) {
    rawMesh.positions.resize(rawMesh.header.verticesCount);
    meshFile.read(reinterpret_cast<char*>(rawMesh.positions.data()),
      sizeof(*rawMesh.positions.begin()) * verticesCount);
  }

  if ((storedAttributesMask & RawMeshAttributes::Normals) != RawMeshAttributes::Empty) {
    rawMesh.normals.resize(rawMesh.header.verticesCount);
    meshFile.read(reinterpret_cast<char*>(rawMesh.normals.data()),
      sizeof(*rawMesh.normals.begin()) * verticesCount);
  }

  if ((storedAttributesMask & RawMeshAttributes::Tangents) != RawMeshAttributes::Empty) {
    rawMesh.tangents.resize(rawMesh.header.verticesCount);
    meshFile.read(reinterpret_cast<char*>(rawMesh.tangents.data()),
      sizeof(*rawMesh.tangents.begin()) * verticesCount);
  }

  if ((storedAttributesMask & RawMeshAttributes::UV) != RawMeshAttributes::Empty) {
    rawMesh.uv.resize(rawMesh.header.verticesCount);
    meshFile.read(reinterpret_cast<char*>(rawMesh.uv.data()),
      sizeof(*rawMesh.uv.begin()) * verticesCount);
  }

  if ((storedAttributesMask & RawMeshAttributes::BonesIDs) != RawMeshAttributes::Empty) {
    rawMesh.bonesIds.resize(rawMesh.header.verticesCount);
    meshFile.read(reinterpret_cast<char*>(rawMesh.bonesIds.data()),
      sizeof(*rawMesh.bonesIds.begin()) * verticesCount);
  }

  if ((storedAttributesMask & RawMeshAttributes::BonesWeights) != RawMeshAttributes::Empty) {
    rawMesh.bonesWeights.resize(rawMesh.header.verticesCount);
    meshFile.read(reinterpret_cast<char*>(rawMesh.bonesWeights.data()),
      sizeof(*rawMesh.bonesWeights.begin()) * verticesCount);
  }

  const uint16_t indicesCount = rawMesh.header.indicesCount;

  rawMesh.indices.resize(indicesCount);
  meshFile.read(reinterpret_cast<char*>(rawMesh.indices.data()),
    sizeof(*rawMesh.indices.begin()) * indicesCount);

  const uint16_t subMeshesIndicesOffsetsCount = rawMesh.header.subMeshesIndicesOffsetsCount;

  rawMesh.subMeshesIndicesOffsets.resize(subMeshesIndicesOffsetsCount);
  meshFile.read(reinterpret_cast<char*>(rawMesh.subMeshesIndicesOffsets.data()),
    sizeof(*rawMesh.subMeshesIndicesOffsets.begin()) * subMeshesIndicesOffsetsCount);

  meshFile.read(reinterpret_cast<char*>(&rawMesh.aabb), sizeof(rawMesh.aabb));

  meshFile.close();

  return rawMesh;
}

void RawMesh::writeToFile(const std::string& path, const RawMesh& rawMesh)
{
  SW_ASSERT(rawMesh.header.formatVersion == MESH_FORMAT_VERSION);

  SW_ASSERT(rawMesh.header.indicesCount == rawMesh.indices.size() &&
    rawMesh.header.subMeshesIndicesOffsetsCount == rawMesh.subMeshesIndicesOffsets.size());

  std::ofstream meshFile(path, std::ios::binary);

  const uint16_t verticesCount = rawMesh.header.verticesCount;

  meshFile.write(reinterpret_cast<const char*>(&rawMesh.header), sizeof(rawMesh.header));

  RawMeshAttributes storedAttributesMask = static_cast<RawMeshAttributes>(rawMesh.header.storedAttributesMask);

  if ((storedAttributesMask & RawMeshAttributes::Positions) != RawMeshAttributes::Empty) {
    SW_ASSERT(rawMesh.positions.size() == rawMesh.header.verticesCount);

    meshFile.write(reinterpret_cast<const char*>(rawMesh.positions.data()),
      sizeof(*rawMesh.positions.begin()) * verticesCount);
  }

  if ((storedAttributesMask & RawMeshAttributes::Normals) != RawMeshAttributes::Empty) {
    SW_ASSERT(rawMesh.normals.size() == rawMesh.header.verticesCount);

    meshFile.write(reinterpret_cast<const char*>(rawMesh.normals.data()),
      sizeof(*rawMesh.normals.begin()) * verticesCount);
  }

  if ((storedAttributesMask & RawMeshAttributes::Tangents) != RawMeshAttributes::Empty) {
    SW_ASSERT(rawMesh.tangents.size() == rawMesh.header.verticesCount);

    meshFile.write(reinterpret_cast<const char*>(rawMesh.tangents.data()),
      sizeof(*rawMesh.tangents.begin()) * verticesCount);
  }

  if ((storedAttributesMask & RawMeshAttributes::UV) != RawMeshAttributes::Empty) {
    SW_ASSERT(rawMesh.uv.size() == rawMesh.header.verticesCount);

    meshFile.write(reinterpret_cast<const char*>(rawMesh.uv.data()),
      sizeof(*rawMesh.uv.begin()) * verticesCount);
  }

  if ((storedAttributesMask & RawMeshAttributes::BonesIDs) != RawMeshAttributes::Empty) {
    SW_ASSERT(rawMesh.bonesIds.size() == rawMesh.header.verticesCount);

    meshFile.write(reinterpret_cast<const char*>(rawMesh.bonesIds.data()),
      sizeof(*rawMesh.bonesIds.begin()) * verticesCount);
  }

  if ((storedAttributesMask & RawMeshAttributes::BonesWeights) != RawMeshAttributes::Empty) {
    SW_ASSERT(rawMesh.bonesWeights.size() == rawMesh.header.verticesCount);

    meshFile.write(reinterpret_cast<const char*>(rawMesh.bonesWeights.data()),
      sizeof(*rawMesh.bonesWeights.begin()) * verticesCount);
  }

  const uint16_t indicesCount = rawMesh.header.indicesCount;

  meshFile.write(reinterpret_cast<const char*>(rawMesh.indices.data()),
    sizeof(*rawMesh.indices.begin()) * indicesCount);

  const uint16_t subMeshesIndicesOffsetsCount = rawMesh.header.subMeshesIndicesOffsetsCount;

  meshFile.write(reinterpret_cast<const char*>(rawMesh.subMeshesIndicesOffsets.data()),
    sizeof(*rawMesh.subMeshesIndicesOffsets.begin()) * subMeshesIndicesOffsetsCount);

  meshFile.write(reinterpret_cast<const char*>(&rawMesh.aabb), sizeof(rawMesh.aabb));

  meshFile.close();
}
