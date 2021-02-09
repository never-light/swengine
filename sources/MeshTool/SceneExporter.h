#pragma once

#include <memory>
#include <string>
#include <filesystem>

#include "RawScene.h"

struct SceneExportOptions {

};

class SceneExporter {
 public:
  SceneExporter();

  void exportDataToDirectory(const std::string& exportDir,
    const RawScene& scene,
    const SceneExportOptions& options);

 private:
  static pugi::xml_document generateResourcesDeclarations(
    const std::string& exportDir,
    const RawScene& scene,
    const SceneExportOptions& options);
  static void generateMaterialResourceDeclaration(pugi::xml_node resourcesNode, const RawMaterial& materialInfo);

  static pugi::xml_document generateStaticSpawnDeclarations(
    const std::string& exportDir,
    const RawScene& scene,
    const SceneExportOptions& options);

  static pugi::xml_document generateDynamicSpawnDeclarations(
    const std::string& exportDir,
    const RawScene& scene,
    const SceneExportOptions& options);

  [[nodiscard]] static std::filesystem::path getMeshExportPath(const std::string& exportDir,
    const RawMeshNode& meshNode);
  [[nodiscard]] static std::filesystem::path getColliderExportPath(const std::string& exportDir,
    const RawMeshNode& meshNode);
  [[nodiscard]] static std::filesystem::path getTextureExportPath(const std::string& exportDir,
    const RawTextureInfo& textureInfo);
  [[nodiscard]] static std::filesystem::path getSkeletonExportPath(const std::string& exportDir,
    const RawSkeleton& skeleton);
  [[nodiscard]] static std::filesystem::path getAnimationClipExportPath(const std::string& exportDir,
    const RawSkeletalAnimationClip& animationClip);
  [[nodiscard]] static std::filesystem::path getExportPath(const std::string& exportDir, const std::string& filename);

  [[nodiscard]] static std::string getMeshResourceId(const RawMeshNode& meshNode);
  [[nodiscard]] static std::string getColliderResourceId(const RawMeshNode& meshNode);
  [[nodiscard]] static std::string getTextureResourceId(const RawTextureInfo& textureInfo);
  [[nodiscard]] static std::string getMaterialResourceId(const RawMaterial& materialInfo);
  [[nodiscard]] static std::string getSkeletonResourceId(const RawSkeleton& skeleton);
  [[nodiscard]] static std::string getAnimationClipResourceId(const RawSkeletalAnimationClip& animationClip);
};
