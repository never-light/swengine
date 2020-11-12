#include "SceneExporter.h"

#include <fstream>
#include <filesystem>
#include <spdlog/spdlog.h>

#include <Engine/swdebug.h>
#include <Engine/Utility/xml.h>
#include <Engine/Exceptions/exceptions.h>
#include <Engine/Modules/Math/MathUtils.h>

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

  std::string meshesDirPath = getExportPath(exportDir, "meshes").string();

  if (!std::filesystem::exists(meshesDirPath)) {
    std::filesystem::create_directory(meshesDirPath);
  }

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

  pugi::xml_document staticSpawnDeclarationDocument = generateStaticSpawnDeclarations(exportDir, scene, options);
  staticSpawnDeclarationDocument.save_file(getExportPath(exportDir, "level_static.xml").string().c_str());

  pugi::xml_document dynamicSpawnDeclarationDocument = generateDynamicSpawnDeclarations(exportDir, scene, options);
  dynamicSpawnDeclarationDocument.save_file(getExportPath(exportDir, "level_spawn.xml").string().c_str());

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

    meshResourceNode.append_attribute("type").set_value("mesh");
    meshResourceNode.append_attribute("id").set_value(getMeshResourceId(meshNode).c_str());
    meshResourceNode.append_attribute("source").set_value(
      getMeshExportPath(exportDir, meshNode).string().c_str());
  }

  return std::move(declarationsDocument);
}

std::filesystem::path SceneExporter::getMeshExportPath(const std::string& exportDir, const RawMeshNode& meshNode) const
{
  return getExportPath(exportDir, "meshes/" + std::string(meshNode.name) + ".mesh");
}

std::filesystem::path SceneExporter::getExportPath(const std::string& exportDir, const std::string& filename) const
{
  return std::filesystem::path(exportDir) / filename;
}

pugi::xml_document SceneExporter::generateStaticSpawnDeclarations(const std::string& exportDir,
  const RawScene& scene,
  const SceneExportOptions& options)
{
  ARG_UNUSED(exportDir);
  ARG_UNUSED(options);

  pugi::xml_document staticSpawnDocument;

  pugi::xml_node objectsNode = staticSpawnDocument.append_child("objects");

  pugi::xml_node environmentNode = objectsNode.append_child("object");
  environmentNode.append_attribute("class").set_value("generic");
  environmentNode.append_attribute("spawn_name").set_value("environment");
  environmentNode.append_attribute("id").set_value("environment");

  pugi::xml_node environmentTransformComponentNode = environmentNode.append_child("transform");
  environmentTransformComponentNode.append_attribute("position").set_value("0 0 0");
  environmentTransformComponentNode.append_attribute("direction").set_value("0 0 0");

  pugi::xml_node environmentComponentNode = environmentNode.append_child("environment");
  environmentComponentNode.append_attribute("material").set_value("materials_common_environment");

  for (const auto& meshNode : scene.meshesNodes) {
    pugi::xml_node meshResourceNode = objectsNode.append_child("object");

    std::string meshSpawnName = "spawn_static_mesh_" + std::string(meshNode.name);
    std::string meshObjectId = "static_mesh_" + std::string(meshNode.name);

    meshResourceNode.append_attribute("class").set_value("generic");
    meshResourceNode.append_attribute("spawn_name").set_value(meshSpawnName.c_str());
    meshResourceNode.append_attribute("id").set_value(meshObjectId.c_str());

    pugi::xml_node transformComponentNode = meshResourceNode.append_child("transform");
    transformComponentNode.append_attribute("position").set_value(
      fmt::format("{} {} {}", meshNode.position.x, meshNode.position.y, meshNode.position.z).c_str());

    glm::vec3 directionVector = glm::eulerAngles(glm::quat(meshNode.orientation.w,
      meshNode.orientation.x,
      meshNode.orientation.y,
      meshNode.orientation.z));

    transformComponentNode.append_attribute("direction").set_value(
      fmt::format("{} {} {}", glm::degrees(directionVector.x),
        glm::degrees(directionVector.y), glm::degrees(directionVector.z)).c_str());

    transformComponentNode.append_attribute("scale").set_value(
      fmt::format("{} {} {}", meshNode.scale.x, meshNode.scale.y, meshNode.scale.z).c_str());

    pugi::xml_node visualComponentNode = meshResourceNode.append_child("visual");
    visualComponentNode.append_attribute("mesh").set_value(getMeshResourceId(meshNode).c_str());

    pugi::xml_node visualComponentMaterialsNode = visualComponentNode.append_child("materials");

    for (size_t subMeshIndex = 0; subMeshIndex < meshNode.rawMesh.subMeshesIndicesOffsets.size(); subMeshIndex++) {
      pugi::xml_node materialNode = visualComponentMaterialsNode.append_child("material");

      materialNode.append_attribute("id").set_value("materials_common_checker");
      materialNode.append_attribute("index").set_value(subMeshIndex);
    }

    pugi::xml_node rigidBodyComponentNode = meshResourceNode.append_child("rigid_body");
    rigidBodyComponentNode.append_attribute("collision_model").set_value("visual_aabb");
  }

  return std::move(staticSpawnDocument);
}

std::string SceneExporter::getMeshResourceId(const RawMeshNode& meshNode) const
{
  return "resource_mesh_" + std::string(meshNode.name);
}

pugi::xml_document SceneExporter::generateDynamicSpawnDeclarations(const std::string& exportDir,
  const RawScene& scene,
  const SceneExportOptions& options)
{
  ARG_UNUSED(exportDir);
  ARG_UNUSED(scene);
  ARG_UNUSED(options);

  pugi::xml_document dynamicSpawnDocument;
  pugi::xml_node objectsNode = dynamicSpawnDocument.append_child("objects");

  return std::move(dynamicSpawnDocument);
}
