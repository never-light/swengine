#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>

#include <memory>

struct AssimpMeshLoadOptions {
  bool flipUV = false;
  bool joinIdenticalVertices = false;
  bool calculateTangents = false;
  bool glueByMaterials = false;
  int maxBonexPerVertex = 4;
};

class AssimpScene {
 public:
  AssimpScene(const std::string& path, const AssimpMeshLoadOptions& options);
  ~AssimpScene();

  const aiScene& getScene() const;

 private:
  Assimp::Importer m_sceneImporter;
  const aiScene* m_scene;
};

class AssimpMeshLoader {
 public:
  static std::unique_ptr<AssimpScene> loadScene(const std::string& path, const AssimpMeshLoadOptions& options);

 private:
  AssimpMeshLoader() = delete;
};

