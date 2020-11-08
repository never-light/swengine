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
  pugi::xml_document generateResourcesDeclarations(
    const std::string& exportDir,
    const RawScene& scene,
    const SceneExportOptions& options);

//  pugi::xml_document generateStaticSpawnDeclarations(const RawScene& scene,
//    const SceneExportOptions& options);

  std::filesystem::path getMeshExportPath(const std::string& exportDir, const RawMeshNode& meshNode);
  std::filesystem::path getExportPath(const std::string& exportDir, const std::string& filename);

};
