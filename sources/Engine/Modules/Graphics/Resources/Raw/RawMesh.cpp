#include "precompiled.h"

#pragma hdrstop

#include "RawMesh.h"
#include "Exceptions/exceptions.h"

#include "Utility/files.h"

RawMesh RawMesh::readFromFile(const std::string& path)
{
  if (!FileUtils::isFileExists(path)) {
    THROW_EXCEPTION(EngineRuntimeException, "Trying to read not existing mesh file " + path);
  }

  RawMesh rawMesh;

  std::ifstream meshFile(path, std::ios::binary);
  meshFile.read(reinterpret_cast<char*>(&rawMesh.header), sizeof(rawMesh.header));

  if (rawMesh.header.formatVersion != MESH_FORMAT_VERSION) {
    THROW_EXCEPTION(EngineRuntimeException, "Trying to load mesh with incompatible format version: " +
      path);
  }

  if (rawMesh.header.verticesCount == 0) {
    THROW_EXCEPTION(EngineRuntimeException, "Trying to load mesh with zero vertices count: " +
      path);
  }

  const uint16_t verticesCount = rawMesh.header.verticesCount;
  auto storedAttributesMask = static_cast<RawMeshAttributes>(rawMesh.header.storedAttributesMask);

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

  for (size_t subMeshIndex = 0; subMeshIndex < rawMesh.header.subMeshesCount; subMeshIndex++) {
    RawSubMeshDescription subMeshDescription;

    meshFile.read(reinterpret_cast<char*>(&subMeshDescription.indicesCount),
      sizeof(subMeshDescription.indicesCount));

    subMeshDescription.indices.resize(subMeshDescription.indicesCount);
    meshFile.read(reinterpret_cast<char*>(subMeshDescription.indices.data()),
      sizeof(*subMeshDescription.indices.begin()) * subMeshDescription.indicesCount);

    rawMesh.subMeshesDescriptions.push_back(subMeshDescription);
  }

  meshFile.read(reinterpret_cast<char*>(&rawMesh.aabb), sizeof(rawMesh.aabb));
  meshFile.read(reinterpret_cast<char*>(&rawMesh.inverseSceneTransform.data),
    sizeof(rawMesh.inverseSceneTransform.data));

  meshFile.close();

  return rawMesh;
}

void RawMesh::writeToFile(const std::string& path, const RawMesh& rawMesh)
{
  SW_ASSERT(rawMesh.header.formatVersion == MESH_FORMAT_VERSION);
  SW_ASSERT(rawMesh.header.subMeshesCount == rawMesh.subMeshesDescriptions.size());

  std::ofstream meshFile(path, std::ios::binary);

  const uint16_t verticesCount = rawMesh.header.verticesCount;

  meshFile.write(reinterpret_cast<const char*>(&rawMesh.header), sizeof(rawMesh.header));

  auto storedAttributesMask = static_cast<RawMeshAttributes>(rawMesh.header.storedAttributesMask);

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

  for (const RawSubMeshDescription& rawSubMeshDescription : rawMesh.subMeshesDescriptions) {
    SW_ASSERT(rawSubMeshDescription.indicesCount == rawSubMeshDescription.indices.size());

    meshFile.write(reinterpret_cast<const char*>(&rawSubMeshDescription.indicesCount),
      sizeof(rawSubMeshDescription.indicesCount));

    meshFile.write(reinterpret_cast<const char*>(rawSubMeshDescription.indices.data()),
      sizeof(*rawSubMeshDescription.indices.begin()) * rawSubMeshDescription.indicesCount);
  }

  meshFile.write(reinterpret_cast<const char*>(&rawMesh.aabb), sizeof(rawMesh.aabb));
  meshFile.write(reinterpret_cast<const char*>(&rawMesh.inverseSceneTransform.data),
    sizeof(rawMesh.inverseSceneTransform.data));

  meshFile.close();
}
