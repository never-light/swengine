#include "SceneExporter.h"

#include <fstream>
#include <filesystem>
#include <spdlog/spdlog.h>

#include <Engine/swdebug.h>
#include <Engine/Utility/xml.h>
#include <Engine/Exceptions/exceptions.h>

#include "MeshExporter.h"

SceneExporter::SceneExporter()
{

}

void SceneExporter::exportDataToDirectory(const std::string& exportDir,
  const RawScene& scene,
  const SceneExportOptions& options)
{
  ARG_UNUSED(options);

  // Save meshes

  for (const auto& meshNode : scene.meshesNodes) {
    MeshExportOptions meshExportOptions{};

    auto meshAttributes = static_cast<RawMeshAttributes>(meshNode.rawMesh.header.storedAttributesMask);

    RawMeshAttributes posNormTanUVAttributesMask = RawMeshAttributes::Positions | RawMeshAttributes::Normals
      | RawMeshAttributes::UV | RawMeshAttributes::Tangents;

    RawMeshAttributes posNormUVAttributesMask = RawMeshAttributes::Positions | RawMeshAttributes::Normals
      | RawMeshAttributes::UV;

    if ((meshAttributes & posNormTanUVAttributesMask) == posNormTanUVAttributesMask) {
      meshExportOptions.format = MeshExportFormat::Pos3Norm3Tan3UV;
    }
    else if ((meshAttributes & posNormUVAttributesMask) == posNormUVAttributesMask) {
      meshExportOptions.format = MeshExportFormat::Pos3Norm3UV;
    }
    else {
      spdlog::critical("Mesh format {} is invalid or not supported yet", static_cast<size_t>(meshAttributes));
      THROW_EXCEPTION(EngineRuntimeException,
        fmt::format("Mesh format {} is invalid or not supported yet", static_cast<size_t>(meshAttributes)));
    }

    MeshExporter exporter;
    exporter.exportToFile(getMeshExportPath(exportDir, meshNode).string(), meshNode.rawMesh, meshExportOptions);
  }

  // Generate resources

  pugi::xml_document resourcesDeclarationDocument = generateResourcesDeclarations(exportDir, scene, options);
  resourcesDeclarationDocument.save_file(getExportPath(exportDir, "resources.xml").string().c_str());

  spdlog::info("Save scene data to directory: {}", exportDir);
}

pugi::xml_document SceneExporter::generateResourcesDeclarations(
  const std::string& exportDir,
  const RawScene& scene,
  const SceneExportOptions& options)
{
  ARG_UNUSED(options);

  pugi::xml_document declarationsDocument;

  pugi::xml_node resourcesNode = declarationsDocument.append_child("resources");

  for (const auto& meshNode : scene.meshesNodes) {
    pugi::xml_node meshResourceNode = resourcesNode.append_child("resource");

    std::string meshResourceId = "resource_mesh_" + std::string(meshNode.name);

    meshResourceNode.append_attribute("type").set_value("mesh");
    meshResourceNode.append_attribute(meshResourceId.c_str()).set_value("mesh");
    meshResourceNode.append_attribute("source").set_value(getMeshExportPath(exportDir, meshNode).string().c_str());
  }

  return std::move(declarationsDocument);
}

std::filesystem::path SceneExporter::getMeshExportPath(const std::string& exportDir, const RawMeshNode& meshNode)
{
  return getExportPath(exportDir, std::string(meshNode.name) + ".mesh");
}

std::filesystem::path SceneExporter::getExportPath(const std::string& exportDir, const std::string& filename)
{
  return std::filesystem::path(exportDir) / filename;
}