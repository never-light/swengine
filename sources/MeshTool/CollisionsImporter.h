#pragma once

#include <memory>
#include <unordered_map>

#include <Engine/Modules/Physics/Resources/Raw/RawMeshCollisionData.h>

#include "MeshImporter.h"

struct CollisionsImportOptions {
  char unused_padding[1];
};

class CollisionsImporter {
 public:
  CollisionsImporter();

  std::unique_ptr<RawMeshCollisionData> importFromFile(const std::string& path,
    const CollisionsImportOptions& options);

 private:
  std::unique_ptr<RawMeshCollisionData> convertSceneToCollisionData(const aiScene& scene,
    const CollisionsImportOptions& options);

  [[nodiscard]] static RawMeshCollisionShape importCollisionShape(const std::string& meshName,
    const ImportMeshData& meshData,
    const std::unordered_map<std::string, ImportMeshData>& meshesList);

  [[nodiscard]] static RawMeshCollisionShape importCollisionSphereShape(const ImportMeshData& meshData);
  [[nodiscard]] static RawMeshCollisionShape importCollisionAABBShape(const ImportMeshData& meshData);
  [[nodiscard]] static RawMeshCollisionShape importCollisionTriangleMeshShape(const ImportMeshData& meshData,
    const std::unordered_map<std::string, ImportMeshData>& meshesList);

  [[nodiscard]] static std::vector<glm::vec3> getAiMeshVertices(const ImportMeshData& meshData);
};
