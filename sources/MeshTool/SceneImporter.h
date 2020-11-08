#pragma once

#include <memory>
#include <unordered_map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>

#include <tiny_gltf.h>

#include "RawScene.h"

struct SceneImportOptions {

};

class SceneImporter {
 public:
  SceneImporter();

  std::unique_ptr<RawScene> importFromFile(const std::string& path, const SceneImportOptions& options);

 private:
  void traceSceneDebugInformation(const tinygltf::Model& model, const tinygltf::Scene& scene);
  void traceAccessorDebugInformation(const tinygltf::Model& model, const tinygltf::Accessor& accessor);
  void validateScene(const tinygltf::Model& model, const tinygltf::Scene& scene);
  std::vector<RawMeshNode> convertSceneToRawData(const tinygltf::Model& model, const tinygltf::Scene& scene);
};
