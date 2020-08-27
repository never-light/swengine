#include "CollisionsImporter.h"

#include <spdlog/spdlog.h>

#include <string>
#include <bitset>
#include <Engine/Exceptions/exceptions.h>
#include <Engine/Utility/memory.h>
#include <Engine/swdebug.h>

#include "utils.h"
#include "AssimpMeshLoader.h"


CollisionsImporter::CollisionsImporter()
{

}

std::unique_ptr<RawMeshCollisionData> CollisionsImporter::importFromFile(const std::string& path,
  const CollisionsImportOptions& options)
{
  spdlog::info("Load source mesh: {}", path);

  AssimpMeshLoadOptions assimpOptions;
  assimpOptions.loadRiggingData = false;
  assimpOptions.calculateTangents = false;
  assimpOptions.flipUV = false;
  assimpOptions.glueByMaterials = false;
  assimpOptions.joinIdenticalVertices = false;
  assimpOptions.maxBonexPerVertex = 0;

  std::unique_ptr<AssimpScene> scene = AssimpMeshLoader::loadScene(path, assimpOptions);

  spdlog::info("Source mesh is loaded");
  spdlog::info("Start collision data parsing");

  std::unique_ptr<RawMeshCollisionData> collisionData = convertSceneToCollisionData(scene->getScene(), options);

  spdlog::info("Collision data is parsed, {} colliders", collisionData->header.collisionShapesCount);

  return collisionData;
}

std::unique_ptr<RawMeshCollisionData> CollisionsImporter::convertSceneToCollisionData(const aiScene& scene,
  const CollisionsImportOptions& options)
{
  ARG_UNUSED(options);

  spdlog::info("Start to import collision data");

  std::unordered_map<std::string, ImportMeshData> meshesList;

  aiMatrix4x4 rootTransform;
  aiIdentityMatrix4(&rootTransform);

  MeshImporter::collectMeshes(scene, *scene.mRootNode, meshesList, rootTransform);

  if (meshesList.empty()) {
    THROW_EXCEPTION(EngineRuntimeException, "Failed to import collision data, colliders geometry is not found");
  }

  std::vector<RawMeshCollisionShape> collisionShapes;

  for (auto[subMeshName, subMeshPtr] : meshesList) {
    if (subMeshName.starts_with("collider")) {
      spdlog::info("Import collider {}", subMeshName);

      RawMeshCollisionShape collisionShape = importCollisionShape(subMeshName, subMeshPtr, meshesList);
      collisionShapes.push_back(collisionShape);
    }
    else {
      spdlog::info("Skip submesh {}", subMeshName);
    }
  }

  // Collision data formation

  bool triangleMeshColliderFound = std::find_if(collisionShapes.begin(), collisionShapes.end(),
    [](const auto& shape) { return shape.type == RawMeshCollisionShapeType::TriangleMesh; }) != collisionShapes.end();

  if (triangleMeshColliderFound && collisionShapes.size() > 1) {
    THROW_EXCEPTION(EngineRuntimeException,
      "Triangle mesh collider options can not be used for meshes with another colliders");
  }

  std::unique_ptr<RawMeshCollisionData> collisionData = std::make_unique<RawMeshCollisionData>();
  collisionData->collisionShapes = collisionShapes;

  collisionData->header.formatVersion = MESH_COLLISION_DATA_FORMAT_VERSION;
  collisionData->header.collisionShapesCount = static_cast<uint16_t>(collisionData->collisionShapes.size());

  return collisionData;
}


RawMeshCollisionShape CollisionsImporter::importCollisionShape(const std::string& meshName,
  const ImportMeshData& meshData,
  const std::unordered_map<std::string, ImportMeshData>& meshesList)
{
  const aiMesh& subMesh = *meshData.mesh;

  if (!subMesh.HasPositions()) {
    THROW_EXCEPTION(EngineRuntimeException,
      "Collider " + meshName + " does not have positions data");
  }

  if (meshName.starts_with("collider_sphere_")) {
    return importCollisionSphereShape(meshData);
  }
  else if (meshName.starts_with("collider_aabb_")) {
    return importCollisionAABBShape(meshData);
  }
  else if (meshName.starts_with("collider_triangle_mesh_")) {
    return importCollisionTriangleMeshShape(meshData, meshesList);
  }
  else {
    THROW_EXCEPTION(EngineRuntimeException,
      "Collider " + meshName + " has unsupported type");
  }
}

RawMeshCollisionShape CollisionsImporter::importCollisionSphereShape(const ImportMeshData& meshData)
{
  auto vertices = getAiMeshVertices(meshData);

  Sphere sphere = GeometryUtils::restoreSphereByVerticesList(vertices);

  float radius = sphere.getRadius();
  glm::vec3 origin = sphere.getOrigin();

  RawMeshCollisionShape rawShape{};
  rawShape.type = RawMeshCollisionShapeType::Sphere;
  rawShape.sphere.radius = radius;
  rawShape.sphere.origin = {origin.x, origin.y, origin.z};

  return rawShape;
}

RawMeshCollisionShape CollisionsImporter::importCollisionAABBShape(const ImportMeshData& meshData)
{
  auto vertices = getAiMeshVertices(meshData);

  AABB aabb = GeometryUtils::restoreAABBByVerticesList(vertices);

  glm::vec3 min = aabb.getMin();
  glm::vec3 max = aabb.getMax();

  RawMeshCollisionShape rawShape{};
  rawShape.type = RawMeshCollisionShapeType::AABB;
  rawShape.aabb.min = {min.x, min.y, min.z};
  rawShape.aabb.max = {max.x, max.y, max.z};

  return rawShape;
}

RawMeshCollisionShape CollisionsImporter::importCollisionTriangleMeshShape(const ImportMeshData& meshData,
  const std::unordered_map<std::string, ImportMeshData>& meshesList)
{
  ARG_UNUSED(meshData);

  std::vector<glm::vec3> vertices;

  for (auto[subMeshName, subMeshPtr] : meshesList) {
    if (!subMeshName.starts_with("collider")) {
      auto subMeshVertices = getAiMeshVertices(subMeshPtr);

      vertices.insert(std::end(vertices), std::begin(subMeshVertices), std::end(subMeshVertices));
    }
  }

  RawMeshCollisionShape rawShape{};
  rawShape.type = RawMeshCollisionShapeType::TriangleMesh;
  rawShape.triangleMesh.header.verticesCount = static_cast<uint16_t>(vertices.size());
  rawShape.triangleMesh.vertices = MemoryUtils::createBinaryCompatibleVector<glm::vec3, RawVector3>(vertices);

  return rawShape;
}

std::vector<glm::vec3> CollisionsImporter::getAiMeshVertices(const ImportMeshData& meshData)
{
  const aiMesh& subMesh = *meshData.mesh;

  SW_ASSERT(subMesh.HasPositions());

  std::vector<glm::vec3> positions;

  for (size_t faceIndex = 0; faceIndex < subMesh.mNumFaces; faceIndex++) {
    const aiFace& face = subMesh.mFaces[faceIndex];

    if (face.mNumIndices != 3) {
      THROW_EXCEPTION(EngineRuntimeException,
        "Mesh " + std::string(meshData.mesh->mName.C_Str()) + " has non-triangle vertex");
    }

    for (size_t indexNumber = 0; indexNumber < 3; indexNumber++) {
      aiVector3D position = subMesh.mVertices[face.mIndices[indexNumber]];

      positions.push_back(aiVec3ToGlm(position));
    }
  }

  return positions;
}
