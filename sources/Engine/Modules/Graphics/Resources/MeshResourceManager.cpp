#include "precompiled.h"

#pragma hdrstop

#include "MeshResourceManager.h"

#include <fstream>
#include <streambuf>
#include <bitset>
#include <algorithm>

#include "Modules/Graphics/GraphicsSystem/Animation/Skeleton.h"
#include "Modules/ResourceManagement/ResourcesManagement.h"
#include "Exceptions/exceptions.h"
#include "Utility/memory.h"

#include "SkeletonResourceManager.h"

#include "Modules/Graphics/Resources/Raw/RawMesh.h"

MeshResourceManager::MeshResourceManager(ResourcesManager* resourcesManager)
  : ResourceManager<Mesh, MeshResourceConfig>(resourcesManager)
{

}

MeshResourceManager::~MeshResourceManager() = default;

void MeshResourceManager::load(size_t resourceIndex)
{
  MeshResourceConfig* config = getResourceConfig(resourceIndex);

  // Read raw mesh
  RawMesh rawMesh = RawMesh::readFromFile(config->resourcePath);

  // Convert raw mesh to internal mesh object
  auto mesh = allocateResource<Mesh>(resourceIndex);

  if (!rawMesh.positions.empty()) {
    std::vector<glm::vec3> positions =
      MemoryUtils::createBinaryCompatibleVector<RawVector3, glm::vec3>(rawMesh.positions);

    mesh->setVertices(positions);
  }

  if (!rawMesh.normals.empty()) {
    std::vector<glm::vec3> normals =
      MemoryUtils::createBinaryCompatibleVector<RawVector3, glm::vec3>(rawMesh.normals);

    mesh->setNormals(normals);
  }

  if (!rawMesh.uv.empty()) {
    std::vector<glm::vec2> uv =
      MemoryUtils::createBinaryCompatibleVector<RawVector2, glm::vec2>(rawMesh.uv);

    mesh->setUV(uv);
  }

  if (!rawMesh.tangents.empty()) {
    std::vector<glm::vec3> tangents =
      MemoryUtils::createBinaryCompatibleVector<RawVector3, glm::vec3>(rawMesh.tangents);

    mesh->setTangents(tangents);
  }

  if (!rawMesh.bonesIds.empty()) {
    SW_ASSERT(rawMesh.bonesWeights.size() == rawMesh.bonesIds.size());

    std::vector<glm::u8vec4> bonesIDs =
      MemoryUtils::createBinaryCompatibleVector<RawU8Vector4, glm::u8vec4>(rawMesh.bonesIds);

    std::vector<glm::u8vec4> bonesWeights =
      MemoryUtils::createBinaryCompatibleVector<RawU8Vector4, glm::u8vec4>(rawMesh.bonesWeights);

    mesh->setSkinData(bonesIDs, bonesWeights);
  }

  for (const RawSubMeshDescription& rawSubMeshDescription : rawMesh.subMeshesDescriptions) {
    mesh->addSubMesh(rawSubMeshDescription.indices);
  }

  mesh->setAABB(AABB(rawVector3ToGLMVector3(rawMesh.aabb.min), rawVector3ToGLMVector3(rawMesh.aabb.max)));
  mesh->setInverseSceneTransform(rawMatrix4ToGLMMatrix4(rawMesh.inverseSceneTransform));

  if (config->skeletonResourceId.has_value()) {
    ResourceHandle<Skeleton> skeleton = getResourceManager()->getResource<Skeleton>(
      config->skeletonResourceId.value());

    mesh->setSkeleton(skeleton);
  }

}

void MeshResourceManager::parseConfig(size_t resourceIndex, pugi::xml_node configNode)
{
  MeshResourceConfig* resourceConfig = createResourceConfig(resourceIndex);
  resourceConfig->resourcePath = configNode.attribute("source").as_string();

  if (!FileUtils::isFileExists(resourceConfig->resourcePath)) {
    THROW_EXCEPTION(EngineRuntimeException,
      fmt::format("Mesh resource refer to not existing file", resourceConfig->resourcePath));
  }

  pugi::xml_node skeletonNode = configNode.child("skeleton");

  if (skeletonNode) {
    pugi::xml_attribute skeletonId = skeletonNode.attribute("id");

    if (!skeletonId) {
      THROW_EXCEPTION(EngineRuntimeException,
        "Skeleton attribute for a mesh is added, but resource id is not specified");
    }

    resourceConfig->skeletonResourceId = skeletonId.as_string();
  }
}
