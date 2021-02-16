#include "SceneExporter.h"

#include <fstream>
#include <filesystem>
#include <spdlog/spdlog.h>

#include <glm/gtx/string_cast.hpp>

#include <Engine/swdebug.h>
#include <Engine/Utility/xml.h>
#include <Engine/Exceptions/exceptions.h>
#include <Engine/Modules/Math/MathUtils.h>

#include "MeshExporter.h"
#include "CollisionsExporter.h"
#include "SkeletonExporter.h"
#include "AnimationExporter.h"

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

  std::string collidersDirPath = getExportPath(exportDir, "colliders").string();

  if (!std::filesystem::exists(collidersDirPath)) {
    std::filesystem::create_directory(collidersDirPath);
  }

  std::string texturesDirPath = getExportPath(exportDir, "textures").string();

  if (!std::filesystem::exists(texturesDirPath)) {
    std::filesystem::create_directory(texturesDirPath);
  }

  std::string skeletonsDirPath = getExportPath(exportDir, "skeletons").string();

  if (!std::filesystem::exists(skeletonsDirPath)) {
    std::filesystem::create_directory(skeletonsDirPath);
  }

  std::string animationClipsDirPath = getExportPath(exportDir, "animations").string();

  if (!std::filesystem::exists(animationClipsDirPath)) {
    std::filesystem::create_directory(animationClipsDirPath);
  }

  for (const RawSkeletonDescription& skeleton : scene.skeletons) {
    SkeletonExportOptions skeletonExportOptions{};

    SkeletonExporter exporter;
    exporter.exportToFile(getSkeletonExportPath(exportDir, skeleton.skeleton).string(),
      skeleton.skeleton,
      skeletonExportOptions);
  }

  for (const RawSkeletalAnimationClip& animationClip : scene.animationClips) {
    AnimationExportOptions animationClipExportOptions{};

    AnimationExporter exporter;
    exporter.exportToFile(getAnimationClipExportPath(exportDir, animationClip).string(),
      animationClip, animationClipExportOptions);
  }

  for (const auto& meshNode : scene.meshesNodes) {
    MeshExportOptions meshExportOptions{};

    auto meshAttributes = static_cast<RawMeshAttributes>(meshNode.rawMesh.header.storedAttributesMask);

    RawMeshAttributes posNormTanUVAttributesMask = RawMeshAttributes::Positions | RawMeshAttributes::Normals
      | RawMeshAttributes::UV | RawMeshAttributes::Tangents;

    RawMeshAttributes posNormUVAttributesMask = RawMeshAttributes::Positions | RawMeshAttributes::Normals
      | RawMeshAttributes::UV;

    RawMeshAttributes posNormUVSkinnedAttributesMask = RawMeshAttributes::Positions | RawMeshAttributes::Normals
      | RawMeshAttributes::UV | RawMeshAttributes::BonesIDs | RawMeshAttributes::BonesWeights;

    RawMeshAttributes posNormTanUVSkinnedAttributesMask = RawMeshAttributes::Positions | RawMeshAttributes::Normals
      | RawMeshAttributes::UV | RawMeshAttributes::BonesIDs | RawMeshAttributes::BonesWeights;

    if ((meshAttributes & posNormUVSkinnedAttributesMask) == posNormUVSkinnedAttributesMask) {
      meshExportOptions.format = MeshExportFormat::Pos3Norm3UVSkinned;
    }
    else if ((meshAttributes & posNormTanUVSkinnedAttributesMask) == posNormTanUVSkinnedAttributesMask) {
      meshExportOptions.format = MeshExportFormat::Pos3Norm3Tan3UVSkinned;
    }
    else if ((meshAttributes & posNormTanUVAttributesMask) == posNormTanUVAttributesMask) {
      meshExportOptions.format = MeshExportFormat::Pos3Norm3UV;
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

    if (meshNode.collisionsResolutionEnabled) {
      const auto& collisionData = meshNode.collisionData;

      if (!collisionData.collisionShapes.empty()) {
        CollisionsExporter collisionsExporter;
        CollisionsExportOptions collisionsExportOptions{};

        collisionsExporter.exportToFile(getColliderExportPath(exportDir, meshNode).string(),
          collisionData, collisionsExportOptions);
      }
    }
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

  // Meshes export
  for (const auto& meshNode : scene.meshesNodes) {
    pugi::xml_node meshResourceNode = resourcesNode.append_child("resource");

    meshResourceNode.append_attribute("type").set_value("mesh");
    meshResourceNode.append_attribute("id").set_value(getMeshResourceId(meshNode).c_str());
    meshResourceNode.append_attribute("source").set_value(
      getMeshExportPath(exportDir, meshNode).string().c_str());

    if (!meshNode.collisionData.collisionShapes.empty()) {
      pugi::xml_node colliderResourceNode = resourcesNode.append_child("resource");

      colliderResourceNode.append_attribute("type").set_value("collision");
      colliderResourceNode.append_attribute("id").set_value(getColliderResourceId(meshNode).c_str());
      colliderResourceNode.append_attribute("source").set_value(
        getColliderExportPath(exportDir, meshNode).string().c_str());
    }
  }

  std::map<std::string, RawMaterial> materialsToExport;

  for (const auto& meshNode : scene.meshesNodes) {
    for (const auto& material : meshNode.materials) {
      if (material.has_value()) {
        std::string materialResourceName = getMaterialResourceId(material.value());

        materialsToExport[materialResourceName] = material.value();
      }
    }
  }

  // Textures export
  std::map<std::string, RawTextureInfo> texturesToExport;

  for (const auto& materialIt : materialsToExport) {
    const RawMaterial& material = materialIt.second;

    if (material.baseColorTextureInfo.has_value()) {
      texturesToExport[material.baseColorTextureInfo->textureTmpPath] = material.baseColorTextureInfo.value();
    }
  }

  for (auto&[textureTmpPath, textureInfo] : texturesToExport) {
    std::string textureExportPath = getTextureExportPath(exportDir, textureInfo).string();

    if (!std::filesystem::exists(textureExportPath)) {
      spdlog::info("Export texture {}", textureExportPath);

      std::filesystem::copy_file(textureTmpPath, textureExportPath,
        std::filesystem::copy_options::overwrite_existing);
    }

    // TODO: use texture settings presets here instead of specifying them all

    pugi::xml_node textureResourceNode = resourcesNode.append_child("resource");
    textureResourceNode.append_attribute("type").set_value("texture");
    textureResourceNode.append_attribute("id").set_value(getTextureResourceId(textureInfo).c_str());
    textureResourceNode.append_attribute("source").set_value(textureExportPath.c_str());

    textureResourceNode.append_child("type").append_child(pugi::node_pcdata).set_value("2d");
    textureResourceNode.append_child("format").append_child(pugi::node_pcdata).set_value("rgb8");
    textureResourceNode.append_child("generate_mipmaps").append_child(pugi::node_pcdata).set_value("true");
    textureResourceNode.append_child("min_filter").append_child(pugi::node_pcdata).set_value("linear_mipmap_linear");
    textureResourceNode.append_child("mag_filter").append_child(pugi::node_pcdata).set_value("linear");

    pugi::xml_node textureResourceWrapParametersNode = textureResourceNode.append_child("wrap");
    textureResourceWrapParametersNode.append_attribute("u").set_value("repeat");
    textureResourceWrapParametersNode.append_attribute("v").set_value("repeat");
  }

  // Materials export
  for (const auto& materialIt : materialsToExport) {
    const RawMaterial& material = materialIt.second;
    generateMaterialResourceDeclaration(resourcesNode, material);
  }

  // Skeletons export

  for (const RawSkeletonDescription& skeletonDescription : scene.skeletons) {
    pugi::xml_node skeletonResourceNode = resourcesNode.append_child("resource");
    skeletonResourceNode.append_attribute("type").set_value("skeleton");
    skeletonResourceNode.append_attribute("id").set_value(getSkeletonResourceId(skeletonDescription.skeleton).c_str());

    std::string skeletonExportPath = getSkeletonExportPath(exportDir, skeletonDescription.skeleton).string();
    skeletonResourceNode.append_attribute("source").set_value(skeletonExportPath.c_str());
  }

  // Animations export

  for (const RawSkeletalAnimationClip& animationClip : scene.animationClips) {
    pugi::xml_node animationClipResourceNode = resourcesNode.append_child("resource");
    animationClipResourceNode.append_attribute("type").set_value("animation");
    animationClipResourceNode.append_attribute("id").set_value(getAnimationClipResourceId(animationClip).c_str());

    std::string animationClipExportPath = getAnimationClipExportPath(exportDir, animationClip).string();
    animationClipResourceNode.append_attribute("source").set_value(animationClipExportPath.c_str());
  }

  // Animation states machines export

  for (const RawSkeletonDescription& skeletonDescription : scene.skeletons) {
    pugi::xml_node statesMachineResourceNode = resourcesNode.append_child("resource");
    statesMachineResourceNode.append_attribute("type").set_value("animation_states_machine");
    statesMachineResourceNode.append_attribute("id")
      .set_value(getStatesMachineResourceId(skeletonDescription.skeleton).c_str());

    pugi::xml_node skeletonNode = statesMachineResourceNode.append_child("skeleton");
    skeletonNode.append_attribute("id").set_value(getSkeletonResourceId(skeletonDescription.skeleton).c_str());

    pugi::xml_node statesNode = statesMachineResourceNode.append_child("states");

    for (size_t animationClipIndex : skeletonDescription.animationClips) {
      const RawSkeletalAnimationClip& rawClip = scene.animationClips[animationClipIndex];

      pugi::xml_node stateNode = statesNode.append_child("state");
      stateNode.append_attribute("name").set_value(getAnimationClipResourceId(rawClip).c_str());

      {
        pugi::xml_node animationHierarchyNode = stateNode.append_child("anim");

        pugi::xml_node simpleAnimationNode = animationHierarchyNode.append_child("node");
        simpleAnimationNode.append_attribute("type").set_value("clip");

        pugi::xml_node clipNode = simpleAnimationNode.append_child("clip");
        clipNode.append_attribute("scale").set_value(1.0);
        clipNode.append_attribute("id").set_value(getAnimationClipResourceId(rawClip).c_str());
      }

      pugi::xml_node finalActionNode = stateNode.append_child("final_action");
      finalActionNode.append_attribute("type").set_value("repeat");

      pugi::xml_node transitionsNode = stateNode.append_child("transitions");
      ARG_UNUSED(transitionsNode);
    }
  }

  return std::move(declarationsDocument);
}

void SceneExporter::generateMaterialResourceDeclaration(pugi::xml_node resourcesNode, const RawMaterial& materialInfo)
{
  std::string materialName = getMaterialResourceId(materialInfo);

  pugi::xml_node materialResourceNode = resourcesNode.append_child("resource");
  materialResourceNode.append_attribute("type").set_value("material");
  materialResourceNode.append_attribute("id").set_value(materialName.c_str());
  materialResourceNode.append_attribute("rendering_stage").set_value("deferred");
  materialResourceNode.append_attribute("parameters_set").set_value("opaque_mesh");

  pugi::xml_node shadersPipelineNode = materialResourceNode.append_child("shaders_pipeline");

  // TODO: set skinned vertex shader for skinned meshes
  pugi::xml_node vertexShaderNode = shadersPipelineNode.append_child("vertex");
  vertexShaderNode.append_attribute("id").set_value(
    (materialInfo.isSkinned) ? "deferred_gpass_vertex_shader_skinned" : "deferred_gpass_vertex_shader");

  pugi::xml_node fragmentShaderNode = shadersPipelineNode.append_child("fragment");
  fragmentShaderNode.append_attribute("id").set_value("deferred_gpass_fragment_shader");

  pugi::xml_node parametersNode = materialResourceNode.append_child("params");

  pugi::xml_node baseColorFactorParameterNode = parametersNode.append_child("param");
  baseColorFactorParameterNode.append_attribute("shader").set_value("fragment");
  baseColorFactorParameterNode.append_attribute("type").set_value("color");
  baseColorFactorParameterNode.append_attribute("name").set_value("base_color");
  baseColorFactorParameterNode.append_attribute("value").set_value(fmt::format("{} {} {} {}",
    materialInfo.baseColorFactor.x,
    materialInfo.baseColorFactor.y,
    materialInfo.baseColorFactor.z,
    materialInfo.baseColorFactor.w).c_str());

  if (materialInfo.baseColorTextureInfo.has_value()) {
    pugi::xml_node baseColorTextureParameterNode = parametersNode.append_child("param");
    baseColorTextureParameterNode.append_attribute("shader").set_value("fragment");
    baseColorTextureParameterNode.append_attribute("type").set_value("texture");
    baseColorTextureParameterNode.append_attribute("name").set_value("base_color_map");
    baseColorTextureParameterNode.append_attribute("id").set_value(
      getTextureResourceId(materialInfo.baseColorTextureInfo.value()).c_str());
    baseColorTextureParameterNode.append_attribute("slot").set_value(0);

    const auto& baseColorTextureTransform = materialInfo.baseColorTextureInfo->textureTransform;

    if (baseColorTextureTransform.has_value()) {
      baseColorTextureParameterNode.append_attribute("offset").set_value(fmt::format("{} {}",
        baseColorTextureTransform->offset.x, baseColorTextureTransform->offset.y).c_str());
      baseColorTextureParameterNode.append_attribute("scale").set_value(fmt::format("{} {}",
        baseColorTextureTransform->scale.x, baseColorTextureTransform->scale.y).c_str());
      baseColorTextureParameterNode.append_attribute("rotation").set_value(fmt::format("{}",
        baseColorTextureTransform->rotation).c_str());
    }
  }

  pugi::xml_node baseColorUseTextureParameterNode = parametersNode.append_child("param");
  baseColorUseTextureParameterNode.append_attribute("shader").set_value("fragment");
  baseColorUseTextureParameterNode.append_attribute("type").set_value("bool");
  baseColorUseTextureParameterNode.append_attribute("name").set_value("use_base_color_map");
  baseColorUseTextureParameterNode.append_attribute("value").set_value(
    materialInfo.baseColorTextureInfo.has_value());
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
    if (!isMeshNodeDynamic(meshNode)) {
      generateSpawnDeclaration(scene, meshNode, objectsNode);
    }
  }

  return std::move(staticSpawnDocument);
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

  for (const auto& meshNode : scene.meshesNodes) {
    if (isMeshNodeDynamic(meshNode)) {
      generateSpawnDeclaration(scene, meshNode, objectsNode);
    }
  }

  return std::move(dynamicSpawnDocument);
}

void SceneExporter::generateSpawnDeclaration(
  const RawScene& scene,
  const RawMeshNode& meshNode,
  pugi::xml_node& objectsNode)
{
  pugi::xml_node meshResourceNode = objectsNode.append_child("object");

  std::string meshType = isMeshNodeDynamic(meshNode) ? "dynamic" : "static";

  std::string meshSpawnName = "spawn_" + meshType + "_mesh_" + meshNode.name;
  std::string meshObjectId = meshType + "_mesh_" + meshNode.name;

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

  if (meshNode.skeletonIndex != RawMeshNode::NONE_SKELETON) {
    visualComponentNode.append_attribute("skeleton").set_value(
      getSkeletonResourceId(scene.skeletons[meshNode.skeletonIndex].skeleton).c_str());
  }

  pugi::xml_node visualComponentMaterialsNode = visualComponentNode.append_child("materials");

  for (size_t subMeshIndex = 0; subMeshIndex < meshNode.rawMesh.subMeshesDescriptions.size(); subMeshIndex++) {
    pugi::xml_node materialNode = visualComponentMaterialsNode.append_child("material");

    std::string materialResourceId = (meshNode.skeletonIndex != RawMeshNode::NONE_SKELETON) ?
      "materials_common_checker_skinned" : "materials_common_checker";

    if (meshNode.materials[subMeshIndex].has_value()) {
      materialResourceId = getMaterialResourceId(meshNode.materials[subMeshIndex].value());
    }

    materialNode.append_attribute("id").set_value(materialResourceId.c_str());
    materialNode.append_attribute("index").set_value(subMeshIndex);
  }

  if (meshNode.skeletonIndex != RawMeshNode::NONE_SKELETON) {
    const RawSkeleton& skeleton = scene.skeletons[meshNode.skeletonIndex].skeleton;

    pugi::xml_node animationComponentNode = meshResourceNode.append_child("animation");
    animationComponentNode.append_attribute("skeleton").set_value(getSkeletonResourceId(skeleton).c_str());
    animationComponentNode.append_attribute("state_machine").set_value(getStatesMachineResourceId(skeleton).c_str());
    animationComponentNode.append_attribute("start_state").set_value(
      getAnimationClipResourceId(scene.animationClips
        .at(*scene.skeletons[meshNode.skeletonIndex].animationClips.begin())).c_str());
  }

  if (meshNode.collisionsResolutionEnabled) {
    pugi::xml_node rigidBodyComponentNode = meshResourceNode.append_child("rigid_body");

    if (meshNode.collisionData.collisionShapes.empty()) {
      rigidBodyComponentNode.append_attribute("collision_model").set_value("visual_aabb");
    }
    else {
      rigidBodyComponentNode.append_attribute("collision_model").set_value(
        getColliderResourceId(meshNode).c_str());
    }
  }
}

std::filesystem::path SceneExporter::getMeshExportPath(const std::string& exportDir, const RawMeshNode& meshNode)
{
  return getExportPath(exportDir, "meshes/" + std::string(meshNode.name) + ".mesh");
}

std::filesystem::path SceneExporter::getExportPath(const std::string& exportDir, const std::string& filename)
{
  return std::filesystem::path(exportDir) / filename;
}

std::filesystem::path SceneExporter::getColliderExportPath(const std::string& exportDir,
  const RawMeshNode& meshNode)
{
  return getExportPath(exportDir, fmt::format("colliders/{}.collider", std::string(meshNode.name)));
}

std::filesystem::path SceneExporter::getTextureExportPath(const std::string& exportDir,
  const RawTextureInfo& textureInfo)
{
  return getExportPath(exportDir, fmt::format("textures/{}",
    std::filesystem::path(textureInfo.textureTmpPath).filename().string()));
}

std::filesystem::path SceneExporter::getSkeletonExportPath(const std::string& exportDir, const RawSkeleton& skeleton)
{
  return getExportPath(exportDir, "skeletons/" + getSkeletonResourceId(skeleton) + ".skeleton");
}

std::filesystem::path SceneExporter::getAnimationClipExportPath(const std::string& exportDir,
  const RawSkeletalAnimationClip& animationClip)
{
  return getExportPath(exportDir, "animations/" + getAnimationClipResourceId(animationClip) + ".anim");
}

std::string SceneExporter::getMeshResourceId(const RawMeshNode& meshNode)
{
  std::string meshName = meshNode.name;
  SW_ASSERT(!meshName.empty());

  return "resource_mesh_" + meshName;
}

std::string SceneExporter::getColliderResourceId(const RawMeshNode& meshNode)
{
  std::string colliderName = meshNode.name;
  SW_ASSERT(!colliderName.empty());

  return "resource_mesh_collider_" + colliderName;
}

std::string SceneExporter::getTextureResourceId(const RawTextureInfo& textureInfo)
{
  SW_ASSERT(!textureInfo.textureBaseName.empty());

  return "resource_texture_" + textureInfo.textureBaseName;
}

std::string SceneExporter::getSkeletonResourceId(const RawSkeleton& skeleton)
{
  std::string skeletonName = skeleton.header.name;
  SW_ASSERT(!skeletonName.empty());

  return "resource_skeleton_" + skeletonName;
}

std::string SceneExporter::getAnimationClipResourceId(const RawSkeletalAnimationClip& animationClip)
{
  std::string animationClipName = animationClip.header.name;
  SW_ASSERT(!animationClipName.empty());

  return "resource_anim_clip_" + animationClipName;
}

std::string SceneExporter::getMaterialResourceId(const RawMaterial& materialInfo)
{
  std::string materialName = std::string(materialInfo.name);
  SW_ASSERT(!materialName.empty());

  std::string materialResourceName = "resource_material_" + materialName;

  // NOTE: it is assumed that material name is always unique and can not be empty, so there is not two material info instances
  //  with the same name.
  /*
  if (materialName.empty()) {
    materialName = "unnamed";
  }

  size_t materialIndex = 1;

  while (processedMaterials.contains(materialResourceName)) {
    materialResourceName = "resource_material_" + materialName + "_" + std::to_string(materialIndex);
    materialIndex++;
  }
   */

  return materialResourceName;
}

std::string SceneExporter::getStatesMachineResourceId(const RawSkeleton& skeleton)
{
  return "resource_states_machine_" + getSkeletonResourceId(skeleton);
}

bool SceneExporter::isMeshNodeDynamic(const RawMeshNode& meshNode)
{
  return meshNode.skeletonIndex != RawMeshNode::NONE_SKELETON;
}

