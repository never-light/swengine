#pragma once

#include <memory>
#include <unordered_map>
#include <functional>

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
  static void traceSceneNodeDebugInformation(const tinygltf::Model& model,
    const tinygltf::Scene& scene,
    const glm::mat4& parentNodeTransform,
    const tinygltf::Node& node);
  static void traceAccessorDebugInformation(const tinygltf::Model& model, const tinygltf::Accessor& accessor);

  void validateScene(const tinygltf::Model& model, const tinygltf::Scene& scene);
  static void validateSceneNode(const tinygltf::Model& model,
    const tinygltf::Scene& scene,
    const glm::mat4& parentNodeTransform,
    const tinygltf::Node& node);
  static void validateTexture(const tinygltf::Model& model, const tinygltf::Texture& texture);

  [[nodiscard]] std::vector<RawMeshNode> convertSceneToRawData(const tinygltf::Model& model,
    const tinygltf::Scene& scene);

  RawMeshNode convertMeshNodeToRawData(const tinygltf::Model& model,
    const tinygltf::Scene& scene,
    const glm::mat4& nodeTransform,
    const tinygltf::Node& node);

  [[nodiscard]] std::vector<glm::vec3> convertMeshToVerticesList(const tinygltf::Model& model,
    const tinygltf::Scene& scene,
    const glm::mat4& nodeTransform,
    const tinygltf::Node& node);

  [[nodiscard]] static RawTextureInfo exportTextureToTempLocation(
    const tinygltf::Model& model,
    const tinygltf::TextureInfo& textureInfo);

  static void raiseImportError(const std::string& error);

  [[nodiscard]] static glm::mat4 getMeshNodeTransform(const tinygltf::Node& meshNode);

  static std::tuple<const unsigned char*, size_t> getAttributeBufferStorage(const tinygltf::Model& model,
    const tinygltf::Accessor& accessor);

  void traverseScene(const tinygltf::Model& model, const tinygltf::Scene& scene,
    const std::function<void(const tinygltf::Model&,
      const tinygltf::Scene&,
      const glm::mat4&,
      const tinygltf::Node&)>& visitor,
    bool withMeshesOnly = true);

  void traverseSceneInternal(const tinygltf::Model& model, const tinygltf::Scene& scene,
    const glm::mat4& parentNodeTransform,
    const tinygltf::Node& node,
    const std::function<void(const tinygltf::Model&,
      const tinygltf::Scene&,
      const glm::mat4&,
      const tinygltf::Node&)>& visitor,
    bool withMeshesOnly = true);

  [[nodiscard]] static std::filesystem::path getTextureTmpExportPath(
    const tinygltf::Model& model,
    const tinygltf::Texture& texture,
    size_t index = 0);
};
