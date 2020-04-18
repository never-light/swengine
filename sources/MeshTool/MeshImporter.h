#pragma once

#include <memory>
#include <unordered_map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>

#include <Engine/Modules/Graphics/Resources/Raw/RawMesh.h>
#include <Engine/Modules/Graphics/Resources/Raw/RawSkeleton.h>

struct MeshImportOptions {
  bool flipUV = false;
  bool joinIdenticalVertices = false;
  bool calculateTangents = false;
  bool glueByMaterials = false;
  bool loadSkin = false;
  int maxBonesPerVertex = 4;
};

struct ImportBoneData {
  int boneId;
  RawBone rawBone;
};

struct ImportMeshData {
  const aiMesh* mesh;
  aiMatrix4x4 sceneTransfromationMatrix;
};

class MeshImporter {
 public:
  MeshImporter();

  std::unique_ptr<RawMesh> importFromFile(const std::string& path, const MeshImportOptions& options);

 private:
  std::unique_ptr<RawMesh> convertSceneToMesh(const aiScene& scene,
    const RawSkeleton* skeleton,
    const MeshImportOptions& options);

  void collectMeshes(const aiScene& scene,
    const aiNode& sceneNode,
    std::unordered_map<std::string, ImportMeshData>& meshesList,
    const aiMatrix4x4& parentNodeTransform) const;

  std::unique_ptr<RawSkeleton> getSkeleton(const std::string& path, const MeshImportOptions& options) const;

  std::unordered_map<std::string, int> getBonesMap(const RawSkeleton& skeleton) const;
};
