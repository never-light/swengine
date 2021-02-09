#include "SceneImporter.h"

#include <fstream>
#include <filesystem>

#include <spdlog/spdlog.h>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/string_cast.hpp>

#include <Engine/swdebug.h>
#include <Engine/Exceptions/exceptions.h>

#include <Engine/Utility/strings.h>
#include <Engine/Modules/Math/MathUtils.h>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <tiny_gltf.h>
#include <Engine/Modules/Math/geometry.h>
#include <Engine/Utility/containers.h>

SceneImporter::SceneImporter()
{

}

std::unique_ptr<RawScene> SceneImporter::importFromFile(const std::string& path, const SceneImportOptions& options)
{
  ARG_UNUSED(options);

  spdlog::info("Start to convert scene {}", path);

  tinygltf::Model model;
  tinygltf::TinyGLTF loader;
  std::string errors;
  std::string warnings;

  bool loadingResult{};

  if (path.ends_with("glb")) {
    loadingResult = loader.LoadBinaryFromFile(&model, &errors, &warnings, path);
  }
  else {
    loadingResult = loader.LoadASCIIFromFile(&model, &errors, &warnings, path);
  }

  if (!warnings.empty()) {
    spdlog::warn("Warnings: {}", warnings);
  }

  if (!errors.empty()) {
    spdlog::warn("Errors: {}", warnings);
  }
  if (!loadingResult) {
    raiseImportError(fmt::format("It is impossible to load glTF scene {}, result {}", path, loadingResult));
  }

  tinygltf::Scene& scene = model.scenes[model.defaultScene];

  if (std::filesystem::exists("mesh_tool_tmp")) {
    std::filesystem::remove_all("mesh_tool_tmp");
  }

  std::filesystem::create_directory("mesh_tool_tmp");

  traceSceneDebugInformation(model, scene);
  validateScene(model, scene);

  m_modelNodesGlobalTransforms.clear();
  m_modelNodesSkeletons.clear();
  m_modelNodesParents.resize(model.nodes.size(), -1);

  for (size_t nodeIndex = 0; nodeIndex < model.nodes.size(); nodeIndex++) {
    m_nodesIds[&model.nodes[nodeIndex]] = static_cast<int16_t>(nodeIndex);
  }

  for (size_t skinIndex = 0; skinIndex < model.skins.size(); skinIndex++) {
    for (size_t skinNodeIndex : model.skins[skinIndex].joints) {
      m_modelNodesSkeletons[skinNodeIndex] = skinIndex;
    }
  }

  traverseScene(model, scene, [this](const tinygltf::Model& model,
    const tinygltf::Scene& scene,
    const glm::mat4& nodeTransform,
    const tinygltf::Node& node) {
    int16_t currentNodeIndex = getNodeIndex(model, scene, node);
    this->m_modelNodesGlobalTransforms[currentNodeIndex] = nodeTransform;

    for (size_t childNodeIndex : node.children) {
      this->m_modelNodesParents[childNodeIndex] = static_cast<int16_t>(currentNodeIndex);
    }
  }, false);

  auto rawScene = std::make_unique<RawScene>();
  rawScene->skeletons = convertSceneSkeletonsToRawData(model, scene);
  rawScene->animationClips = convertSceneAnimationsToRawData(model, scene);
  rawScene->meshesNodes.clear();

  std::vector<RawMeshNodeImportData> rawMeshNodesImportData = convertSceneToRawData(model, scene);

  for (const auto& rawMeshImportData : rawMeshNodesImportData) {
    rawScene->meshesNodes.push_back(rawMeshImportData.rawNode);
  }

  return std::move(rawScene);
}

void SceneImporter::traceSceneDebugInformation(const tinygltf::Model& model, const tinygltf::Scene& scene)
{
  spdlog::info("Scene: \"{}\", nodes_count {}", scene.name, scene.nodes.size());

  traverseScene(model, scene, [](const tinygltf::Model& model,
    const tinygltf::Scene& scene,
    const glm::mat4& parentNodeTransform,
    const tinygltf::Node& node) {
    traceSceneNodeDebugInformation(model, scene, parentNodeTransform, node);
  });
}

void SceneImporter::traceSceneNodeDebugInformation(const tinygltf::Model& model,
  const tinygltf::Scene& scene,
  const glm::mat4& parentNodeTransform,
  const tinygltf::Node& node)
{
  ARG_UNUSED(parentNodeTransform);
  ARG_UNUSED(scene);

  const tinygltf::Mesh& mesh = model.meshes[node.mesh];

  spdlog::info("Node \"{}\" (mesh {}, children_count {})", node.name, mesh.name, node.children.size());

  spdlog::info("  Mesh: {}", mesh.name);

  for (auto& primitive : mesh.primitives) {
    spdlog::info("    Indices_count: {}, mode {}", primitive.indices, primitive.mode);

    const tinygltf::Accessor& indexAccessor =
      model.accessors[primitive.indices];

    spdlog::info("   IndexAccessor:");
    traceAccessorDebugInformation(model, indexAccessor);

    for (auto& attribute : primitive.attributes) {
      spdlog::info("    Attribute: {}", attribute.first);
      traceAccessorDebugInformation(model, model.accessors[attribute.second]);
    }
  }
}

void SceneImporter::traceAccessorDebugInformation(const tinygltf::Model& model, const tinygltf::Accessor& accessor)
{
  spdlog::info("      buffer_index: {}", accessor.bufferView);
  spdlog::info("      data_type: {}", accessor.componentType);
  spdlog::info("      components_count: {}", accessor.type);
  spdlog::info("      buffer_offset: {}", accessor.byteOffset);
  spdlog::info("      count: {}", accessor.count);
  spdlog::info("      stride: {}", accessor.ByteStride(model.bufferViews[accessor.bufferView]));
  spdlog::info("      sparse: {}", accessor.sparse.isSparse);
}

void SceneImporter::validateScene(const tinygltf::Model& model, const tinygltf::Scene& scene)
{
  // TODO: it seems that models with multiple buffers can be imported now,
  //  but test is needed.
//  if (model.buffers.size() != 1) {
//    raiseImportError("Models with multiple buffers are not supported yet");
//  }

  traverseScene(model, scene, [](const tinygltf::Model& model,
    const tinygltf::Scene& scene,
    const glm::mat4& parentNodeTransform,
    const tinygltf::Node& node) {
    validateSceneNode(model, scene, parentNodeTransform, node);
  });
}

void SceneImporter::validateSceneNode(const tinygltf::Model& model,
  const tinygltf::Scene& scene,
  const glm::mat4& parentNodeTransform,
  const tinygltf::Node& node)
{
  ARG_UNUSED(parentNodeTransform);
  ARG_UNUSED(scene);

  const tinygltf::Mesh& mesh = model.meshes[node.mesh];

  if (node.children.size() > 1) {
    raiseImportError("Nodes hierarchies are not supported yet, so it is needed to flatten the scene");
  }

  if (!node.weights.empty()) {
    raiseImportError("Nodes morph targets are not supported yet\"");
  }

  if (node.skin != -1) {
    if (!node.children.empty()) {
      raiseImportError("Nodes with skinning data can not have children");
    }
  }

  for (const auto& primitive : mesh.primitives) {
    if (primitive.mode != TINYGLTF_MODE_TRIANGLES) {
      raiseImportError("Non-triangle primitives are not supported yet");
    }

    const tinygltf::Accessor& indexAccessor =
      model.accessors[primitive.indices];

    if (indexAccessor.sparse.isSparse) {
      raiseImportError("Sparse accessors are not supported yet, so it is needed to flatten the index buffers");
    }

    if (indexAccessor.normalized || indexAccessor.componentType != TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT ||
      indexAccessor.type != TINYGLTF_TYPE_SCALAR) {
      raiseImportError("Index accessors should not be normalized and should have scalar unsigned short type");
    }

    const tinygltf::BufferView& indexBufferView = model.bufferViews[indexAccessor.bufferView];

    if (indexBufferView.byteStride != 0) {
      raiseImportError("Buffer views stride attribute are not supported yet");
    }

    if (primitive.attributes.empty()) {
      raiseImportError("Primitive should have at least one vertex attribute");
    }

    size_t firstAttributeCount = model.accessors[(*primitive.attributes.begin()).second].count;

    for (auto& attribute : primitive.attributes) {
      const tinygltf::Accessor& accessor = model.accessors[attribute.second];

      if (accessor.count != firstAttributeCount) {
        raiseImportError("All vertices attributes should have equal number of values");
      }

      if (accessor.sparse.isSparse) {
        raiseImportError("Sparse accessors are not supported yet, so it is needed to flatten the vertex buffers");
      }

      if (attribute.first == "POSITION") {
        if (accessor.normalized || accessor.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT ||
          accessor.type != TINYGLTF_TYPE_VEC3) {
          raiseImportError("Position accessors should not be normalized and should have vec3 type");
        }
      }
      else if (attribute.first == "NORMAL") {
        if (accessor.normalized || accessor.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT ||
          accessor.type != TINYGLTF_TYPE_VEC3) {
          raiseImportError("Normal accessors should not be normalized and should have vec3 type");
        }
      }
      else if (attribute.first == "TANGENT") {
        if (accessor.normalized || accessor.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT ||
          accessor.type != TINYGLTF_TYPE_VEC4) {
          raiseImportError("Tangent accessors should not be normalized and should have vec4 type");
        }
      }
      else if (attribute.first == "TEXCOORD_0") {
        if (accessor.normalized || accessor.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT ||
          accessor.type != TINYGLTF_TYPE_VEC2) {
          raiseImportError("UV0 accessors should not be normalized and should have vec2 type");
        }
      }
      else if (attribute.first == "JOINTS_0") {
        if (accessor.normalized || accessor.componentType != TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT ||
          accessor.type != TINYGLTF_TYPE_VEC4) {
          raiseImportError("Joints accessors should not be normalized and should have vec4 type");
        }
      }
      else if (attribute.first == "WEIGHTS_0") {
        if (accessor.normalized || accessor.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT ||
          accessor.type != TINYGLTF_TYPE_VEC4) {
          raiseImportError("Weights accessors should not be normalized and should have vec2 type");
        }
      }
      else if (attribute.first == "COLOR_0") {
        spdlog::warn("Color vertices attribute for mesh {} will be ignored", node.name);
      }
      else {
        raiseImportError(fmt::format("Attribute {} is not supported yet", attribute.first));
      }
    }

    if (primitive.material != -1) {
      const tinygltf::Material& material = model.materials[primitive.material];

      const tinygltf::PbrMetallicRoughness& pbrParameters = material.pbrMetallicRoughness;

      if (pbrParameters.baseColorTexture.texCoord != 0) {
        raiseImportError(fmt::format("Multiple UV-channels are not supported yet"));
      }

      if (pbrParameters.baseColorTexture.index != -1) {
        validateTexture(model, model.textures[pbrParameters.baseColorTexture.index]);
      }
    }
  }
}

void SceneImporter::validateTexture(const tinygltf::Model& model, const tinygltf::Texture& texture)
{
  if (texture.sampler != -1) {
    raiseImportError(fmt::format("Texture samplers are not supported yet"));
  }

  if (texture.source == -1) {
    raiseImportError(fmt::format("Texture should have image source"));
  }

  const tinygltf::Image& textureImage = model.images[texture.source];

  if (textureImage.bufferView == -1) {
    raiseImportError(fmt::format("Non-buffered textures loading are not supported yet"));
  }

  std::set<std::string> allowedTexturesMimeTypes = {"image/jpeg", "image/png", "image/bmp"};

  if (!allowedTexturesMimeTypes.contains(textureImage.mimeType)) {
    raiseImportError(fmt::format("Texture mime type {} is not supported yet", textureImage.mimeType));
  }
}

std::vector<RawMeshNodeImportData> SceneImporter::convertSceneToRawData(const tinygltf::Model& model,
  const tinygltf::Scene& scene)
{
  ARG_UNUSED(model);
  ARG_UNUSED(scene);

  std::vector<RawMeshNodeImportData> rawNodesList;

  std::unordered_map<size_t, std::vector<size_t>> skinsToRawMeshesMap;

  traverseScene(model, scene, [this, &rawNodesList, &skinsToRawMeshesMap](const tinygltf::Model& model,
    const tinygltf::Scene& scene,
    const glm::mat4& nodeTransform,
    const tinygltf::Node& node) {
    if (node.name.find("collision") != std::string::npos) {
      return;
    }

    rawNodesList.push_back(RawMeshNodeImportData{
      .rawNode = convertMeshNodeToRawData(model, scene, nodeTransform, node),
      .sceneNodeIndex = static_cast<int32_t>(getNodeIndex(model, scene, node))});

    if (node.skin != -1) {
      skinsToRawMeshesMap[node.skin].push_back(rawNodesList.size() - 1);
    }
  });

  spdlog::info("Try to merge meshes with the same skeleton");

  for (const auto&[skinIndex, skinMeshesList] : skinsToRawMeshesMap) {
    ARG_UNUSED(skinIndex);

    // Verify that all meshes with the same skeleton has the same global transform
    bitmask64 commonAttributesMask = rawNodesList[skinMeshesList[0]].rawNode.rawMesh.header.storedAttributesMask;

    for (size_t skinMeshIndex = 1; skinMeshIndex < skinMeshesList.size(); skinMeshIndex++) {
      auto rawMeshCurrentIndex = static_cast<int16_t>(skinMeshesList[skinMeshIndex]);
      auto rawMeshPreviousIndex = static_cast<int16_t>(skinMeshesList[skinMeshIndex - 1]);

      auto& currentRawMeshNode = rawNodesList[rawMeshCurrentIndex];
      auto& previousRawMeshNode = rawNodesList[rawMeshPreviousIndex];

      bool globalTransformsEquals = MathUtils::isEqual(m_modelNodesGlobalTransforms.at(
        static_cast<int16_t>(currentRawMeshNode.sceneNodeIndex)),
        m_modelNodesGlobalTransforms.at(
          static_cast<int16_t>(previousRawMeshNode.sceneNodeIndex)));

      if (!globalTransformsEquals) {
        raiseImportError("All meshes nodes with the same skin should have the same global transform");
      }

      bool localTransformsEquals = MathUtils::isEqual(glm::make_vec3(currentRawMeshNode.rawNode.position.data),
        glm::make_vec3(previousRawMeshNode.rawNode.position.data), 1e-4f) &&
        MathUtils::isEqual(glm::make_vec3(currentRawMeshNode.rawNode.scale.data),
          glm::make_vec3(previousRawMeshNode.rawNode.scale.data), 1e-4f) &&
        MathUtils::isEqual(glm::make_quat(currentRawMeshNode.rawNode.orientation.data),
          glm::make_quat(previousRawMeshNode.rawNode.orientation.data), 1e-4f);

      if (!localTransformsEquals) {
        raiseImportError("All meshes nodes with the same skin should have the same local transform");
      }

      if (currentRawMeshNode.rawNode.rawMesh.header.storedAttributesMask !=
        previousRawMeshNode.rawNode.rawMesh.header.storedAttributesMask) {
        bitmask64 attributesMaskDifference = currentRawMeshNode.rawNode.rawMesh.header.storedAttributesMask ^
          previousRawMeshNode.rawNode.rawMesh.header.storedAttributesMask;

        if (static_cast<RawMeshAttributes>(attributesMaskDifference) == RawMeshAttributes::Tangents) {
          spdlog::warn("Merged mesh {} doesn't have tangents, so they will be generated",
            currentRawMeshNode.rawNode.name);

          const auto& vertices = currentRawMeshNode.rawNode.rawMesh.positions;
          const auto& uv = currentRawMeshNode.rawNode.rawMesh.uv;
          auto& tangents = currentRawMeshNode.rawNode.rawMesh.tangents;

          tangents.resize(currentRawMeshNode.rawNode.rawMesh.header.verticesCount);

          for (RawSubMeshDescription& subMeshDescription : currentRawMeshNode.rawNode.rawMesh.subMeshesDescriptions) {
            for (size_t indexNumber = 0; indexNumber < subMeshDescription.indicesCount; indexNumber += 3) {
              size_t index0 = subMeshDescription.indices[indexNumber];
              size_t index1 = subMeshDescription.indices[indexNumber + 1];
              size_t index2 = subMeshDescription.indices[indexNumber + 2];

              glm::vec3 tangent = MathUtils::generateTangent(
                glm::make_vec3(vertices[index0].data),
                glm::make_vec3(vertices[index1].data),
                glm::make_vec3(vertices[index2].data),
                glm::make_vec2(uv[index0].data),
                glm::make_vec2(uv[index1].data),
                glm::make_vec2(uv[index2].data));

              tangents[index0] = {tangent.x, tangent.y, tangent.z};
              tangents[index1] = {tangent.x, tangent.y, tangent.z};
              tangents[index2] = {tangent.x, tangent.y, tangent.z};
            }

          }

          currentRawMeshNode.rawNode.rawMesh.header.storedAttributesMask |=
            static_cast<bitmask64>(RawMeshAttributes::Tangents);
        }
        else {
          raiseImportError("All meshes nodes with the same skin should have the same attributes mask");
        }
      }

      commonAttributesMask |= currentRawMeshNode.rawNode.rawMesh.header.storedAttributesMask;
    }

    size_t totalVerticesCount = 0;

    for (unsigned long long skinMeshIndex : skinMeshesList) {
      auto& rawMeshNode = rawNodesList[skinMeshIndex].rawNode;
      auto& rawMesh = rawMeshNode.rawMesh;

      totalVerticesCount += rawMesh.header.verticesCount;
    }

    if (totalVerticesCount > std::numeric_limits<uint16_t>::max()) {
      raiseImportError(fmt::format("Max vertices count for merged mesh is {}", std::numeric_limits<uint16_t>::max()));
    }

    std::string mergedMeshNodeName = std::string("merged_") + rawNodesList[*skinMeshesList.begin()].rawNode.name;

    RawMeshNode mergedRawNode{};
    strncpy_s(mergedRawNode.name, mergedMeshNodeName.c_str(), sizeof(mergedRawNode.name));

    mergedRawNode.rawMesh.header.formatVersion = MESH_FORMAT_VERSION;
    mergedRawNode.rawMesh.header.verticesCount = 0;
    mergedRawNode.rawMesh.header.storedAttributesMask = commonAttributesMask;
    mergedRawNode.rawMesh.header.subMeshesCount = 0;

    mergedRawNode.collisionData.header.collisionShapesCount = 0;
    mergedRawNode.collisionData.header.formatVersion = MESH_COLLISION_DATA_FORMAT_VERSION;

    auto& mergedRawMesh = mergedRawNode.rawMesh;

    mergedRawMesh.aabb = rawNodesList[skinMeshesList[0]].rawNode.rawMesh.aabb;

    for (unsigned long long skinMeshIndex : skinMeshesList) {
      auto& rawMeshNode = rawNodesList[skinMeshIndex].rawNode;
      auto& rawMesh = rawMeshNode.rawMesh;

      SW_ASSERT(rawMesh.header.storedAttributesMask == commonAttributesMask);

      size_t verticesOffset = mergedRawMesh.header.verticesCount;

      mergedRawMesh.header.verticesCount += rawMesh.header.verticesCount;
      mergedRawMesh.header.subMeshesCount += rawMesh.header.subMeshesCount;

      auto mergedAttributes = static_cast<RawMeshAttributes>(commonAttributesMask);

      if ((mergedAttributes & RawMeshAttributes::Positions) != RawMeshAttributes::Empty) {
        ContainersUtils::append(mergedRawMesh.positions, rawMesh.positions);
      }

      if ((mergedAttributes & RawMeshAttributes::UV) != RawMeshAttributes::Empty) {
        ContainersUtils::append(mergedRawMesh.uv, rawMesh.uv);
      }

      if ((mergedAttributes & RawMeshAttributes::Normals) != RawMeshAttributes::Empty) {
        ContainersUtils::append(mergedRawMesh.normals, rawMesh.normals);
      }

      if ((mergedAttributes & RawMeshAttributes::Tangents) != RawMeshAttributes::Empty) {
        ContainersUtils::append(mergedRawMesh.tangents, rawMesh.tangents);
      }

      if ((mergedAttributes & RawMeshAttributes::BonesIDs) != RawMeshAttributes::Empty) {
        ContainersUtils::append(mergedRawMesh.bonesIds, rawMesh.bonesIds);
      }

      if ((mergedAttributes & RawMeshAttributes::BonesWeights) != RawMeshAttributes::Empty) {
        ContainersUtils::append(mergedRawMesh.bonesWeights, rawMesh.bonesWeights);
      }

      size_t mergedSubMeshesStartIndex = mergedRawMesh.subMeshesDescriptions.size();
      ContainersUtils::append(mergedRawMesh.subMeshesDescriptions, rawMesh.subMeshesDescriptions);

      for (size_t subMeshIndex = mergedSubMeshesStartIndex; subMeshIndex < mergedRawMesh.subMeshesDescriptions.size();
           subMeshIndex++) {
        RawSubMeshDescription& subMesh = mergedRawMesh.subMeshesDescriptions[subMeshIndex];

        for (uint16_t& indexValue: subMesh.indices) {
          indexValue += static_cast<uint16_t>(verticesOffset);
        }
      }

      mergedRawMesh.aabb = GeometryUtils::mergeAABB(mergedRawMesh.aabb, rawMesh.aabb);

      if (rawMeshNode.isInteractive) {
        mergedRawNode.isInteractive = true;
      }

      if (rawMeshNode.collisionsResolutionEnabled) {
        mergedRawNode.collisionsResolutionEnabled = true;
      }

      mergedRawNode.collisionData.header.collisionShapesCount += rawMeshNode.collisionData.header.collisionShapesCount;
      ContainersUtils::append(mergedRawNode.collisionData.collisionShapes, rawMeshNode.collisionData.collisionShapes);

      ContainersUtils::append(mergedRawNode.materials, rawMeshNode.materials);
    }

    rawNodesList.push_back(RawMeshNodeImportData{.rawNode=mergedRawNode, .sceneNodeIndex = -1});
  }

  spdlog::info("Scene conversion to raw format is finished");

  return rawNodesList;
}

std::tuple<const unsigned char*, size_t> SceneImporter::getAttributeBufferStorage(const tinygltf::Model& model,
  const tinygltf::Accessor& accessor)
{
  const tinygltf::BufferView& attributeBufferView = model.bufferViews[accessor.bufferView];
  const tinygltf::Buffer& attributeBuffer = model.buffers[attributeBufferView.buffer];

  const auto* attributeBufferPtr =
    reinterpret_cast<const unsigned char*>(attributeBuffer.data.data() + attributeBufferView.byteOffset
      + accessor.byteOffset);

  return {attributeBufferPtr, accessor.ByteStride(attributeBufferView)};
}

glm::mat4 SceneImporter::getMeshNodeTransform(const tinygltf::Node& meshNode)
{
  glm::dvec3 scale = {1.0, 1.0, 1.0};
  glm::dvec3 translation = {0.0, 0.0, 0.0};
  auto orientation = glm::identity<glm::dquat>();

  if (!meshNode.scale.empty()) {
    scale = glm::dvec3{meshNode.scale[0], meshNode.scale[1], meshNode.scale[2]};
  }

  if (!meshNode.rotation.empty()) {
    orientation = glm::dquat(meshNode.rotation[3], meshNode.rotation[0], meshNode.rotation[1], meshNode.rotation[2]);
  }

  if (!meshNode.translation.empty()) {
    translation = glm::dvec3{meshNode.translation[0], meshNode.translation[1], meshNode.translation[2]};
  }

  return glm::mat4(glm::translate(glm::identity<glm::dmat4x4>(), translation) *
    glm::toMat4(orientation) *
    glm::scale(glm::identity<glm::dmat4x4>(), scale));
}

RawMeshNode SceneImporter::convertMeshNodeToRawData(const tinygltf::Model& model,
  const tinygltf::Scene& scene,
  const glm::mat4& nodeTransform,
  const tinygltf::Node& node)
{
  const tinygltf::Mesh& mesh = model.meshes[node.mesh];

  glm::dvec3 scale;
  glm::dquat orientation;
  glm::dvec3 translation;
  glm::dvec3 skew;
  glm::dvec4 perspective;
  glm::decompose(glm::dmat4(nodeTransform), scale, orientation, translation, skew, perspective);

  if (!MathUtils::isEqual(glm::vec3(skew), {0.0f, 0.0f, 0.0f})) {
    raiseImportError(fmt::format("Mesh node {}: non-zero skew factors are forbidden", node.name));
  }

  if (!MathUtils::isEqual(glm::vec4(perspective), {0.0f, 0.0f, 0.0f, 1.0f})) {
    raiseImportError(fmt::format("Mesh node {}: non-identity perspective factors are not supported", node.name));
  }

//  orientation = glm::conjugate(orientation);

  RawMeshNode rawNode{};
  rawNode.position = {float(translation.x), float(translation.y), float(translation.z)};
  rawNode.scale = {float(scale.x), float(scale.y), float(scale.z)};
  rawNode.orientation = {float(orientation.x), float(orientation.y), float(orientation.z), float(orientation.w)};

  strncpy_s(rawNode.name, node.name.c_str(), sizeof(rawNode.name));

  rawNode.rawMesh.header.formatVersion = MESH_FORMAT_VERSION;
  rawNode.rawMesh.header.subMeshesCount = static_cast<uint16_t>(mesh.primitives.size());
  rawNode.rawMesh.header.verticesCount = 0;
  rawNode.rawMesh.header.storedAttributesMask = 0;

  if (mesh.primitives.size() > 1) {
    spdlog::debug("Mesh with submeshes is converted");
  }

  for (const auto& primitive : mesh.primitives) {
    const tinygltf::Accessor& indexAccessor = model.accessors[primitive.indices];

    size_t verticesCount = model.accessors[primitive.attributes.begin()->second].count;
    size_t rawMeshVerticesOffset = rawNode.rawMesh.positions.size();

    size_t indicesCount = indexAccessor.count;

    RawSubMeshDescription subMeshDescription{.indicesCount = static_cast<uint32_t>(indicesCount),
      .indices = std::vector<uint16_t>(indicesCount, 0)};

    auto[indicesBufferPtr, indicesBufferStride] = getAttributeBufferStorage(model, indexAccessor);

    for (size_t indexNumber = 0; indexNumber < indexAccessor.count; indexNumber++) {
      subMeshDescription.indices[indexNumber] = reinterpret_cast<const uint16_t*>(
        indicesBufferPtr)[0] + static_cast<uint16_t>(rawMeshVerticesOffset);
      indicesBufferPtr += indicesBufferStride;
    }

    for (auto& attribute : primitive.attributes) {
      const tinygltf::Accessor& accessor = model.accessors[attribute.second];

      auto[attributeBufferPtr, attributeBufferStride] = getAttributeBufferStorage(model, accessor);

      if (attribute.first == "POSITION") {
        rawNode.rawMesh.positions.resize(rawMeshVerticesOffset + verticesCount);

        for (size_t vertexIndex = 0; vertexIndex < verticesCount; vertexIndex++) {
          rawNode.rawMesh.positions[rawMeshVerticesOffset + vertexIndex].x =
            reinterpret_cast<const float*>(attributeBufferPtr)[0];
          rawNode.rawMesh.positions[rawMeshVerticesOffset + vertexIndex].y =
            reinterpret_cast<const float*>(attributeBufferPtr)[1];
          rawNode.rawMesh.positions[rawMeshVerticesOffset + vertexIndex].z =
            reinterpret_cast<const float*>(attributeBufferPtr)[2];
          attributeBufferPtr += attributeBufferStride;
        }

        rawNode.rawMesh.header.storedAttributesMask |= static_cast<size_t>(RawMeshAttributes::Positions);
      }
      else if (attribute.first == "NORMAL") {
        rawNode.rawMesh.normals.resize(rawMeshVerticesOffset + verticesCount);

        for (size_t vertexIndex = 0; vertexIndex < verticesCount; vertexIndex++) {
          rawNode.rawMesh.normals[rawMeshVerticesOffset + vertexIndex].x =
            reinterpret_cast<const float*>(attributeBufferPtr)[0];
          rawNode.rawMesh.normals[rawMeshVerticesOffset + vertexIndex].y =
            reinterpret_cast<const float*>(attributeBufferPtr)[1];
          rawNode.rawMesh.normals[rawMeshVerticesOffset + vertexIndex].z =
            reinterpret_cast<const float*>(attributeBufferPtr)[2];
          attributeBufferPtr += attributeBufferStride;
        }

        rawNode.rawMesh.header.storedAttributesMask |= static_cast<size_t>(RawMeshAttributes::Normals);
      }
      else if (attribute.first == "JOINTS_0") {
        const tinygltf::Skin& skin = model.skins[node.skin];

        rawNode.rawMesh.bonesIds.resize(rawMeshVerticesOffset + verticesCount);

        for (size_t vertexIndex = 0; vertexIndex < verticesCount; vertexIndex++) {
          const auto* bonesIdsPtr = reinterpret_cast<const unsigned short*>(attributeBufferPtr);

          // TODO: check that JOINTS_0 attribute bones ids always corresponds to m_modelNodesSkeletonsRawIndices mapping
          // and simplify this conversion in that case.
          SW_ASSERT(bonesIdsPtr[0] == m_modelNodesSkeletonsRawIndices[node.skin][skin.joints[bonesIdsPtr[0]]] &&
            bonesIdsPtr[1] == m_modelNodesSkeletonsRawIndices[node.skin][skin.joints[bonesIdsPtr[1]]] &&
            bonesIdsPtr[2] == m_modelNodesSkeletonsRawIndices[node.skin][skin.joints[bonesIdsPtr[2]]] &&
            bonesIdsPtr[3] == m_modelNodesSkeletonsRawIndices[node.skin][skin.joints[bonesIdsPtr[3]]]);

          rawNode.rawMesh.bonesIds[rawMeshVerticesOffset + vertexIndex].x =
            static_cast<uint8_t>(m_modelNodesSkeletonsRawIndices[node.skin][skin.joints[bonesIdsPtr[0]]]);
          rawNode.rawMesh.bonesIds[rawMeshVerticesOffset + vertexIndex].y =
            static_cast<uint8_t>(m_modelNodesSkeletonsRawIndices[node.skin][skin.joints[bonesIdsPtr[1]]]);
          rawNode.rawMesh.bonesIds[rawMeshVerticesOffset + vertexIndex].z =
            static_cast<uint8_t>(m_modelNodesSkeletonsRawIndices[node.skin][skin.joints[bonesIdsPtr[2]]]);
          rawNode.rawMesh.bonesIds[rawMeshVerticesOffset + vertexIndex].w =
            static_cast<uint8_t>(m_modelNodesSkeletonsRawIndices[node.skin][skin.joints[bonesIdsPtr[3]]]);
          attributeBufferPtr += attributeBufferStride;
        }

        rawNode.rawMesh.header.storedAttributesMask |= static_cast<size_t>(RawMeshAttributes::BonesIDs);
      }
      else if (attribute.first == "WEIGHTS_0") {
        rawNode.rawMesh.bonesWeights.resize(rawMeshVerticesOffset + verticesCount);

        const auto* bonesWeightsPtr = reinterpret_cast<const float*>(attributeBufferPtr);

        for (size_t vertexIndex = 0; vertexIndex < verticesCount; vertexIndex++) {
          RawU8Vector4& vertexBonesWeights = rawNode.rawMesh.bonesWeights[rawMeshVerticesOffset + vertexIndex];

          vertexBonesWeights.x =
            static_cast<uint8_t>(std::round(bonesWeightsPtr[0] * 255));
          vertexBonesWeights.y =
            static_cast<uint8_t>(std::round(bonesWeightsPtr[1] * 255));
          vertexBonesWeights.z =
            static_cast<uint8_t>(std::round(bonesWeightsPtr[2] * 255));
          vertexBonesWeights.w =
            static_cast<uint8_t>(std::round(bonesWeightsPtr[3] * 255));

          int16_t
            weightsSum = vertexBonesWeights.x + vertexBonesWeights.y + vertexBonesWeights.z + vertexBonesWeights.w;
          int16_t weightsSumDiff = 255 - weightsSum;

          SW_ASSERT(abs(weightsSumDiff) <= 4);

          size_t minComponentIndex = 0;

          for (size_t currentComponentIndex = 0; currentComponentIndex < 4; currentComponentIndex++) {
            if (vertexBonesWeights.data[currentComponentIndex] > 0 &&
              vertexBonesWeights.data[currentComponentIndex] < vertexBonesWeights.data[minComponentIndex]) {
              minComponentIndex = currentComponentIndex;
            }
          }

          int16_t normalizedWeightComponent = vertexBonesWeights.data[minComponentIndex] + weightsSumDiff;
          SW_ASSERT(normalizedWeightComponent >= 0 && normalizedWeightComponent <= 255);
          vertexBonesWeights.data[minComponentIndex] = static_cast<uint8_t>(normalizedWeightComponent);

          if ((vertexBonesWeights.x + vertexBonesWeights.y + vertexBonesWeights.z + vertexBonesWeights.w) != 255) {
            raiseImportError("Vertices data contains not-normalized bones weights vector");
          }

          attributeBufferPtr += attributeBufferStride;
        }

        rawNode.rawMesh.header.storedAttributesMask |= static_cast<size_t>(RawMeshAttributes::BonesWeights);
      }
      else if (attribute.first == "TANGENT") {
        rawNode.rawMesh.tangents.resize(rawMeshVerticesOffset + verticesCount);

        for (size_t vertexIndex = 0; vertexIndex < verticesCount; vertexIndex++) {
          rawNode.rawMesh.tangents[rawMeshVerticesOffset + vertexIndex].x =
            reinterpret_cast<const float*>(attributeBufferPtr)[0];
          rawNode.rawMesh.tangents[rawMeshVerticesOffset + vertexIndex].y =
            reinterpret_cast<const float*>(attributeBufferPtr)[1];
          rawNode.rawMesh.tangents[rawMeshVerticesOffset + vertexIndex].z =
            reinterpret_cast<const float*>(attributeBufferPtr)[2];

          // TODO: store also w component

          attributeBufferPtr += attributeBufferStride;
        }

        rawNode.rawMesh.header.storedAttributesMask |= static_cast<size_t>(RawMeshAttributes::Tangents);
      }
      else if (attribute.first == "TEXCOORD_0") {
        rawNode.rawMesh.uv.resize(rawMeshVerticesOffset + verticesCount);

        for (size_t vertexIndex = 0; vertexIndex < verticesCount; vertexIndex++) {
          rawNode.rawMesh.uv[rawMeshVerticesOffset + vertexIndex].x = reinterpret_cast<const float*>(
            attributeBufferPtr)[0];
          rawNode.rawMesh.uv[rawMeshVerticesOffset + vertexIndex].y = reinterpret_cast<const float*>(
            attributeBufferPtr)[1];
          attributeBufferPtr += attributeBufferStride;
        }

        rawNode.rawMesh.header.storedAttributesMask |= static_cast<size_t>(RawMeshAttributes::UV);
      }
      else if (attribute.first == "COLOR_0") {
        spdlog::warn("Color vertices attribute for mesh {} is ignored", node.name);
      }
      else {
        SW_ASSERT(false);
      }
    }

    if (primitive.material != -1) {
      const tinygltf::Material& material = model.materials[primitive.material];
      const tinygltf::PbrMetallicRoughness& pbrParameters = material.pbrMetallicRoughness;

      RawMaterial rawMaterial{};

      strncpy_s(rawMaterial.name, material.name.c_str(), sizeof(rawNode.name));

      rawMaterial.baseColorFactor = {static_cast<float>(pbrParameters.baseColorFactor[0]),
        static_cast<float>(pbrParameters.baseColorFactor[1]),
        static_cast<float>(pbrParameters.baseColorFactor[2]),
        static_cast<float>(pbrParameters.baseColorFactor[3])};

      if (pbrParameters.baseColorTexture.index != -1) {
        rawMaterial.baseColorTextureInfo = exportTextureToTempLocation(model, pbrParameters.baseColorTexture);
      }

      rawNode.materials.emplace_back(rawMaterial);
    }
    else {
      rawNode.materials.emplace_back();
    }

    rawNode.rawMesh.subMeshesDescriptions.push_back(subMeshDescription);
  }

  glm::vec3 aabbMin(std::numeric_limits<float>::max());
  glm::vec3 aabbMax(std::numeric_limits<float>::min());

  for (const auto& position : rawNode.rawMesh.positions) {
    aabbMin = glm::min(aabbMin, {position.x, position.y, position.z});
    aabbMax = glm::max(aabbMax, {position.x, position.y, position.z});
  }

  rawNode.rawMesh.aabb = AABB(aabbMin, aabbMax);

  bool collisionsResolutionDisabled = false;

  for (const auto& colliderMeshNode : model.nodes) {
    if (colliderMeshNode.mesh == -1) {
      continue;
    }

    std::string colliderMeshNamePrefix = node.name + "_" + "collision" + "_";

    if (!colliderMeshNode.name.starts_with(colliderMeshNamePrefix)) {
      continue;
    }

    if (colliderMeshNode.name.starts_with(colliderMeshNamePrefix + "no_collision")) {
      collisionsResolutionDisabled = true;
      break;
    }

    std::string colliderType = StringUtils::split(colliderMeshNode.name.substr(
      colliderMeshNamePrefix.length()), '_')[0];

    if (colliderType.starts_with("aabb")) {
      AABB aabbShape = GeometryUtils::restoreAABBByVerticesList(
        convertMeshToVerticesList(model,
          scene,
          glm::inverse(nodeTransform) * getMeshNodeTransform(colliderMeshNode),
          colliderMeshNode));

      glm::vec3 minVertex = aabbShape.getMin();
      glm::vec3 maxVertex = aabbShape.getMax();

      RawMeshCollisionShape shape{};
      shape.type = RawMeshCollisionShapeType::AABB;
      shape.aabb = RawMeshCollisionShapeAABB{
        .min = {minVertex.x, minVertex.y, minVertex.z},
        .max = {maxVertex.x, maxVertex.y, maxVertex.z}};

      rawNode.collisionData.collisionShapes.push_back(shape);

      spdlog::info("Load AABB collider {}, min={}, max={}", colliderMeshNode.name,
        glm::to_string(minVertex), glm::to_string(maxVertex));
    }
    else if (colliderType.starts_with("sphere")) {
      Sphere sphereShape = GeometryUtils::restoreSphereByVerticesList(
        convertMeshToVerticesList(model,
          scene,
          glm::inverse(nodeTransform) * getMeshNodeTransform(colliderMeshNode),
          colliderMeshNode));

      float radius = sphereShape.getRadius();
      glm::vec3 origin = sphereShape.getOrigin();

      RawMeshCollisionShape shape{};
      shape.type = RawMeshCollisionShapeType::Sphere;
      shape.sphere.radius = radius;
      shape.sphere.origin = {origin.x, origin.y, origin.z};

      rawNode.collisionData.collisionShapes.push_back(shape);

      spdlog::info("Load sphere collider {}, origin={}, radius={}", colliderMeshNode.name,
        glm::to_string(origin), radius);
    }
    else if (colliderType.starts_with("triangle_mesh")) {
      std::vector<glm::vec3> collisionMeshVertices =
        convertMeshToVerticesList(model,
          scene,
          glm::inverse(nodeTransform) * getMeshNodeTransform(colliderMeshNode),
          colliderMeshNode);

      RawMeshCollisionShape shape{};
      shape.type = RawMeshCollisionShapeType::TriangleMesh;
      shape.triangleMesh.header.verticesCount = static_cast<uint16_t>(collisionMeshVertices.size());

      for (const auto& vertex : collisionMeshVertices) {
        shape.triangleMesh.vertices.push_back({vertex.x, vertex.y, vertex.z});
      }

      rawNode.collisionData.collisionShapes.push_back(shape);

      spdlog::info("Load triangle mesh collider {}, vertices_count={}", colliderMeshNode.name,
        collisionMeshVertices.size());
    }
    else {
      raiseImportError(fmt::format("Collision mesh type {} is not supported", colliderMeshNode.name));
    }
  }

  if (collisionsResolutionDisabled) {
    rawNode.collisionData.collisionShapes.clear();
  }
  else {
    rawNode.collisionData.header.formatVersion = MESH_COLLISION_DATA_FORMAT_VERSION;
    rawNode.collisionData.header.collisionShapesCount = static_cast<uint16_t>(
      rawNode.collisionData.collisionShapes.size());
  }

  rawNode.collisionsResolutionEnabled = !collisionsResolutionDisabled;

  rawNode.rawMesh.header.verticesCount = static_cast<uint16_t>(rawNode.rawMesh.positions.size());

  size_t rawMeshVerticesCount = rawNode.rawMesh.positions.size();

  if (!rawNode.rawMesh.normals.empty() && rawNode.rawMesh.normals.size() != rawMeshVerticesCount ||
    !rawNode.rawMesh.tangents.empty() && rawNode.rawMesh.tangents.size() != rawMeshVerticesCount ||
    !rawNode.rawMesh.uv.empty() && rawNode.rawMesh.uv.size() != rawMeshVerticesCount ||
    !rawNode.rawMesh.bonesIds.empty() && rawNode.rawMesh.bonesIds.size() != rawMeshVerticesCount ||
    !rawNode.rawMesh.bonesWeights.empty() && rawNode.rawMesh.bonesWeights.size() != rawMeshVerticesCount) {
    raiseImportError(fmt::format("Mesh {} is in inconsistent state because of different attributes count", node.name));
  }

  return rawNode;
}

void SceneImporter::raiseImportError(const std::string& error)
{
  spdlog::critical(error);
  THROW_EXCEPTION(EngineRuntimeException, error);
}

std::vector<glm::vec3> SceneImporter::convertMeshToVerticesList(const tinygltf::Model& model,
  const tinygltf::Scene& scene,
  const glm::mat4& nodeTransform,
  const tinygltf::Node& node)
{
  glm::mat4 meshTransform = nodeTransform;

  RawMeshNode rawMeshNode = convertMeshNodeToRawData(model, scene, meshTransform, node);

  std::vector<glm::vec3> meshVertices;

  for (const auto& vertex : rawMeshNode.rawMesh.positions) {
    meshVertices.emplace_back(vertex.x, vertex.y, vertex.z);
  }

  for (auto& vertex : meshVertices) {
    vertex = glm::vec3(meshTransform * glm::vec4(vertex, 1.0f));
  }

  return meshVertices;
}

void SceneImporter::traverseSceneInternal(const tinygltf::Model& model,
  const tinygltf::Scene& scene,
  const glm::mat4& parentNodeTransform,
  const tinygltf::Node& node,
  const std::function<void(const tinygltf::Model&,
    const tinygltf::Scene&,
    const glm::mat4&,
    const tinygltf::Node&)>& visitor,
  bool withMeshesOnly)
{
  glm::mat4 nodeTransform = parentNodeTransform * getMeshNodeTransform(node);

  if (withMeshesOnly) {
    if (node.mesh != -1) {
      visitor(model, scene, nodeTransform, node);
    }
  }
  else {
    visitor(model, scene, nodeTransform, node);
  }

  for (int childNodeIndex : node.children) {
    traverseSceneInternal(model, scene, nodeTransform, model.nodes[childNodeIndex], visitor, withMeshesOnly);
  }
}

void SceneImporter::traverseScene(const tinygltf::Model& model,
  const tinygltf::Scene& scene,
  const std::function<void(const tinygltf::Model&,
    const tinygltf::Scene&,
    const glm::mat4&,
    const tinygltf::Node&)>& visitor,
  bool withMeshesOnly)
{
  auto rootTransform = glm::identity<glm::mat4>();

  for (int sceneNodeIndex : scene.nodes) {
    traverseSceneInternal(model, scene, rootTransform, model.nodes[sceneNodeIndex], visitor, withMeshesOnly);
  }
}

std::filesystem::path SceneImporter::getTextureTmpExportPath(const tinygltf::Model& model,
  const tinygltf::Texture& texture, size_t index)
{
  std::string extension;

  const tinygltf::Image& textureImage = model.images[texture.source];

  if (textureImage.mimeType == "image/jpeg") {
    extension = "jpg";
  }
  else if (textureImage.mimeType == "image/png") {
    extension = "png";
  }
  else if (textureImage.mimeType == "image/bmp") {
    extension = "bmp";
  }
  else {
    SW_ASSERT(false);
  }

  return std::filesystem::path("mesh_tool_tmp") /
    StringUtils::replace(fmt::format("{}_{}_{}.{}",
      texture.name, textureImage.name, index, extension), " ", "_");
}

RawTextureInfo SceneImporter::exportTextureToTempLocation(const tinygltf::Model& model,
  const tinygltf::TextureInfo& textureInfo)
{
  const tinygltf::Texture& texture = model.textures[textureInfo.index];

  const tinygltf::Image& textureImage = model.images[texture.source];
  const tinygltf::BufferView& textureBufferView = model.bufferViews[textureImage.bufferView];
  const tinygltf::Buffer& textureBuffer = model.buffers[textureBufferView.buffer];

  const auto* textureBufferPtr =
    reinterpret_cast<const unsigned char*>(textureBuffer.data.data() + textureBufferView.byteOffset);

  size_t textureExportIndex = 0;

  auto exportPath = getTextureTmpExportPath(model, texture, textureExportIndex);

  // NOTE: assume here that each texture/image pair has an unique name

//  while (std::filesystem::exists(exportPath)) {
//    textureExportIndex++;
//    exportPath = getTextureTmpExportPath(model, texture, textureExportIndex).string();
//  }

  std::ofstream textureFile;
  textureFile.open(exportPath, std::ofstream::binary);
  textureFile.write(reinterpret_cast<const char*>(textureBufferPtr), textureBufferView.byteLength);
  textureFile.close();

  RawTextureInfo rawTextureInfo{
    .textureTmpPath = exportPath.string(),
    .textureBaseName = exportPath.stem().string()
  };

  if (textureInfo.extensions.contains("KHR_texture_transform")) {
    rawTextureInfo.textureTransform = RawTextureTransformInfo{};

    const tinygltf::Value& textureTransform = textureInfo.extensions.at("KHR_texture_transform");

    if (textureTransform.Has("offset")) {
      const tinygltf::Value& textureTransformOffset = textureTransform.Get("offset");

      rawTextureInfo.textureTransform->offset = {
        static_cast<float>(textureTransformOffset.Get(0).GetNumberAsDouble()),
        static_cast<float>(textureTransformOffset.Get(1).GetNumberAsDouble())
      };
    }

    if (textureTransform.Has("scale")) {
      const tinygltf::Value& textureTransformScale = textureTransform.Get("scale");

      rawTextureInfo.textureTransform->scale = {
        static_cast<float>(textureTransformScale.Get(0).GetNumberAsDouble()),
        static_cast<float>(textureTransformScale.Get(1).GetNumberAsDouble())
      };
    }

    if (textureTransform.Has("rotation")) {
      const tinygltf::Value& textureTransformRotation = textureTransform.Get("rotation");
      rawTextureInfo.textureTransform->rotation = static_cast<float>(textureTransformRotation.GetNumberAsDouble());
    }
  }

  return rawTextureInfo;
}

std::vector<RawSkeleton> SceneImporter::convertSceneSkeletonsToRawData(const tinygltf::Model& model,
  const tinygltf::Scene& scene)
{
  std::vector<RawSkeleton> rawSkeletonsList;

  for (size_t skinId = 0; skinId < model.skins.size(); skinId++) {
    const tinygltf::Skin& skin = model.skins[skinId];

    int16_t rootBoneNodeIndex = -1;

    std::set<size_t> skinBonesNodesSet(skin.joints.begin(), skin.joints.end());

    traverseScene(model,
      scene,
      [this, &rootBoneNodeIndex, &skinBonesNodesSet](const tinygltf::Model& model,
        const tinygltf::Scene& scene,
        const glm::mat4& nodeTransform,
        const tinygltf::Node& node) {
        ARG_UNUSED(nodeTransform);

        int16_t nodeIndex = getNodeIndex(model, scene, node);

        if (rootBoneNodeIndex == -1) {
          if (skinBonesNodesSet.contains(nodeIndex)) {
            rootBoneNodeIndex = nodeIndex;
          }
        }
      },
      false);

    if (rootBoneNodeIndex == -1) {
      raiseImportError("Impossible to find skeleton root bone");
    }

    const tinygltf::Node& rootBoneNode = model.nodes[rootBoneNodeIndex];

    std::vector<size_t> bonesNodes;

    traverseSceneInternal(model,
      scene,
      glm::identity<glm::mat4>(),
      rootBoneNode,
      [this, &bonesNodes, skinId](const tinygltf::Model& model,
        const tinygltf::Scene& scene,
        const glm::mat4& nodeTransform,
        const tinygltf::Node& node) {
        ARG_UNUSED(nodeTransform);
        bonesNodes.push_back(getNodeIndex(model, scene, node));
      },
      false);

    if (bonesNodes.size() != skin.joints.size()) {
      raiseImportError("Partial skeleton importing is not supported");
    }

    std::set<size_t> bonesNodesSet(bonesNodes.begin(), bonesNodes.end());

    for (size_t skeletonBoneNodeId : skin.joints) {
      if (!bonesNodesSet.contains(skeletonBoneNodeId)) {
        raiseImportError("Skeleton root bone node should contain all another bones");
      }
    }

    spdlog::debug("Skeleton root node global transform {}\nLocal transform: {}",
      glm::to_string(m_modelNodesGlobalTransforms.at(rootBoneNodeIndex)),
      glm::to_string(getMeshNodeTransform(rootBoneNode)));

    glm::mat4 skeletonGlobalTransform =
      m_modelNodesGlobalTransforms.at(rootBoneNodeIndex) * glm::inverse(getMeshNodeTransform(rootBoneNode));

    spdlog::debug("Skeleton root node parent transform {}", glm::to_string(skeletonGlobalTransform));

    /*if (!MathUtils::isMatrixIdentity(skeletonGlobalTransform)) {
      raiseImportError(fmt::format("Skeleton root node should have identity parent transform, current transform is {}",
        glm::to_string(skeletonGlobalTransform)));
    }*/

    RawSkeleton rawSkeleton{};
    rawSkeleton.header.formatVersion = SKELETON_FORMAT_VERSION;
    rawSkeleton.header.bonesCount = static_cast<uint8_t>(bonesNodes.size());

    SW_ASSERT(!rootBoneNode.name.empty() && "TODO: implement bones names auto generation");

    // TODO: check for filenames collisions
    std::string skeletonName = StringUtils::filterFilename(rootBoneNode.name);

    strncpy_s(rawSkeleton.header.name, skeletonName.c_str(), sizeof(rawSkeleton.header.name));

    // Nodes indices to raw bones indices map
    std::unordered_map<size_t, size_t> rawBonesIndicesMap;

    std::vector<glm::mat4> inverseBindPoseMatrices = getSkinInverseBindPoseMatrices(model, scene, skin);
    std::unordered_map<size_t, glm::mat4> inverseBindPoseMatricesMap;

    for (size_t childBoneIndex = 0; childBoneIndex < skin.joints.size(); childBoneIndex++) {
      inverseBindPoseMatricesMap[skin.joints[childBoneIndex]] =
        inverseBindPoseMatrices[childBoneIndex];
    }

    RawBone rootRawBone{};

    strncpy_s(rootRawBone.name, rootBoneNode.name.c_str(), sizeof(rootRawBone.name));
    rootRawBone.parentId = RawSkeleton::ROOT_BONE_PARENT_ID;
    rootRawBone.inverseBindPoseMatrix = glmMatrix4ToRawMatrix4(inverseBindPoseMatricesMap[skin.joints[0]]);

    rawSkeleton.bones.push_back(rootRawBone);
    rawBonesIndicesMap.insert({rootBoneNodeIndex, 0});

    // Assume that skeleton root node always has identity parent transform

    for (size_t childNodeId : bonesNodes) {
      if (childNodeId == rootBoneNodeIndex) {
        continue;
      }

      if (m_modelNodesParents[childNodeId] == -1) {
        raiseImportError("Skeleton nodes hierarchy is parsed wrong");
      }

      const tinygltf::Node& boneNode = model.nodes[childNodeId];
      RawBone rawBoneNode{};

      strncpy_s(rawBoneNode.name, boneNode.name.c_str(), sizeof(rawBoneNode.name));
      rawBoneNode.parentId = static_cast<uint8_t>(rawBonesIndicesMap[m_modelNodesParents[childNodeId]]);
      rawBoneNode.inverseBindPoseMatrix = glmMatrix4ToRawMatrix4(inverseBindPoseMatricesMap[childNodeId]);

      rawSkeleton.bones.push_back(rawBoneNode);
      rawBonesIndicesMap.insert({childNodeId, rawSkeleton.bones.size() - 1});
    }

    m_modelNodesSkeletonsRawIndices[skinId] = rawBonesIndicesMap;

    rawSkeletonsList.push_back(rawSkeleton);
  }

  spdlog::info("Skeletons conversion to raw format is finished");

  return rawSkeletonsList;
}

// TODO: get rid of scene and node passing
int16_t SceneImporter::getNodeIndex(const tinygltf::Model& model,
  const tinygltf::Scene& scene,
  const tinygltf::Node& node)
{
  ARG_UNUSED(model);
  ARG_UNUSED(scene);

  return m_nodesIds[&node];
}

std::vector<glm::mat4> SceneImporter::getSkinInverseBindPoseMatrices(const tinygltf::Model& model,
  const tinygltf::Scene& scene,
  const tinygltf::Skin& skin)
{
  ARG_UNUSED(scene);

  std::vector<glm::mat4> matrices;

  const tinygltf::Accessor& matricesAccessor =
    model.accessors[skin.inverseBindMatrices];

  SW_ASSERT(matricesAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT &&
    matricesAccessor.type == TINYGLTF_TYPE_MAT4);

  auto[matricesBufferPtr, matricesBufferStride] = getAttributeBufferStorage(model, matricesAccessor);

  SW_ASSERT(matricesAccessor.count == skin.joints.size());

  for (size_t matrixIndex = 0; matrixIndex < matricesAccessor.count; matrixIndex++) {
    glm::mat4 matrix = glm::make_mat4(reinterpret_cast<const float*>(matricesBufferPtr));
    matrices.push_back(matrix);

    matricesBufferPtr += matricesBufferStride;
  }

  return matrices;
}

std::vector<RawSkeletalAnimationClip> SceneImporter::convertSceneAnimationsToRawData(const tinygltf::Model& model,
  const tinygltf::Scene& scene)
{
  ARG_UNUSED(scene);

  std::vector<RawSkeletalAnimationClip> rawClips;

  for (size_t clipIndex = 0; clipIndex < model.animations.size(); clipIndex++) {
    const tinygltf::Animation& animation = model.animations[clipIndex];

    size_t clipSkeletonIndex = m_modelNodesSkeletons.at(animation.channels.at(0).target_node);

    for (const tinygltf::AnimationChannel& animationChannel : animation.channels) {
      size_t clipNodeSkeleton = m_modelNodesSkeletons.at(animationChannel.target_node);

      if (clipNodeSkeleton != clipSkeletonIndex) {
        raiseImportError("All animation clip nodes should refer to the same existing skeleton");
      }

      if (animationChannel.target_path != "translation" && animationChannel.target_path != "rotation" &&
        animationChannel.target_path != "scale") {
        raiseImportError(fmt::format("Animation channel component {} is not supported", animationChannel.target_path));
      }
    }

    RawSkeletalAnimationClip rawClip{};

    // TODO: check for filenames collisions
    std::string animationClipName = StringUtils::filterFilename(animation.name);
    strncpy_s(rawClip.header.name, animationClipName.c_str(), sizeof(rawClip.header.name));

    rawClip.header.formatVersion = ANIMATION_FORMAT_VERSION;
    rawClip.header.duration = 0.0f;
    rawClip.header.rate = 1.0f;
    rawClip.header.skeletonBonesCount =
      static_cast<uint8_t>(m_modelNodesSkeletonsRawIndices.at(clipSkeletonIndex).size());

    rawClip.bonesAnimationChannels.resize(rawClip.header.skeletonBonesCount, RawBoneAnimationChannel{});

    float maxChannelDuration = 0.0f;

    for (const tinygltf::AnimationChannel& animationChannel : animation.channels) {
      size_t rawBoneChannelId = m_modelNodesSkeletonsRawIndices[clipSkeletonIndex].at(animationChannel.target_node);
      RawBoneAnimationChannel& rawBoneChannel = rawClip.bonesAnimationChannels[rawBoneChannelId];

      const tinygltf::AnimationSampler& channelSampler = animation.samplers[animationChannel.sampler];

      // Load keyframes time
      const tinygltf::Accessor& keyframesTimeAccessor =
        model.accessors[channelSampler.input];

      SW_ASSERT(keyframesTimeAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT &&
        keyframesTimeAccessor.type == TINYGLTF_TYPE_SCALAR);

      auto[keyframesTimeBufferPtr, keyframesTimeBufferStride] = getAttributeBufferStorage(model, keyframesTimeAccessor);

      std::vector<float> keyframesTime;

      for (size_t keyframeTimeIndex = 0; keyframeTimeIndex < keyframesTimeAccessor.count; keyframeTimeIndex++) {
        float keyframeTime = *reinterpret_cast<const float*>(keyframesTimeBufferPtr);
        keyframesTime.push_back(keyframeTime);

        keyframesTimeBufferPtr += keyframesTimeBufferStride;
      }

      // Load animation transformations

      const tinygltf::Accessor& keyframesValuesAccessor =
        model.accessors[channelSampler.output];

      SW_ASSERT(keyframesValuesAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT &&
        (keyframesValuesAccessor.type == TINYGLTF_TYPE_VEC3 || keyframesValuesAccessor.type == TINYGLTF_TYPE_VEC4));

      SW_ASSERT(keyframesValuesAccessor.count == keyframesTime.size());

      auto[keyframesValuesBufferPtr, keyframesValuesBufferStride] = getAttributeBufferStorage(model,
        keyframesValuesAccessor);

      if (animationChannel.target_path == "translation") {
        SW_ASSERT(keyframesValuesAccessor.type == TINYGLTF_TYPE_VEC3);
        SW_ASSERT(rawBoneChannel.positionFrames.empty());

        rawBoneChannel.positionFrames.resize(keyframesValuesAccessor.count);

        for (size_t keyframeIndex = 0; keyframeIndex < keyframesValuesAccessor.count; keyframeIndex++) {
          glm::vec3 positionKey = glm::make_vec3(reinterpret_cast<const float*>(keyframesValuesBufferPtr));
          rawBoneChannel.positionFrames[keyframeIndex].time = keyframesTime[keyframeIndex];
          rawBoneChannel.positionFrames[keyframeIndex].position =
            {.x = positionKey.x, .y = positionKey.y, .z = positionKey.z};

          keyframesValuesBufferPtr += keyframesValuesBufferStride;
        }

        rawBoneChannel.header.positionFramesCount = static_cast<uint16_t>(rawBoneChannel.positionFrames.size());
      }
      else if (animationChannel.target_path == "rotation") {
        SW_ASSERT(keyframesValuesAccessor.type == TINYGLTF_TYPE_VEC4);
        SW_ASSERT(rawBoneChannel.orientationFrames.empty());

        rawBoneChannel.orientationFrames.resize(keyframesValuesAccessor.count);

        for (size_t keyframeIndex = 0; keyframeIndex < keyframesValuesAccessor.count; keyframeIndex++) {
          glm::vec4 orientationKey = glm::make_vec4(reinterpret_cast<const float*>(keyframesValuesBufferPtr));
          rawBoneChannel.orientationFrames[keyframeIndex].time = keyframesTime[keyframeIndex];
          rawBoneChannel.orientationFrames[keyframeIndex].orientation = {.x = orientationKey.x,
            .y = orientationKey.y, .z = orientationKey.z, .w = orientationKey.w};

          keyframesValuesBufferPtr += keyframesValuesBufferStride;
        }

        rawBoneChannel.header.orientationFramesCount = static_cast<uint16_t>(rawBoneChannel.orientationFrames.size());
      }
      else if (animationChannel.target_path == "scale") {
        SW_ASSERT(keyframesValuesAccessor.type == TINYGLTF_TYPE_VEC3);

        for (size_t keyframeIndex = 0; keyframeIndex < keyframesValuesAccessor.count; keyframeIndex++) {
          glm::vec3 scaleKey = glm::make_vec3(reinterpret_cast<const float*>(keyframesValuesBufferPtr));

          if (!MathUtils::isEqual(scaleKey, glm::vec3(1.0f), 1e-4f)) {
            raiseImportError("Scale keys for skeletal animation are not supported");
          }

          keyframesValuesBufferPtr += keyframesValuesBufferStride;
        }

        rawBoneChannel.header.positionFramesCount = static_cast<uint16_t>(rawBoneChannel.positionFrames.size());
      }
      else {
        raiseImportError(fmt::format("Impossible to handle unknown target path {}", animationChannel.target_path));
      }

      maxChannelDuration = std::max(maxChannelDuration, *keyframesTime.rbegin());
    }

    rawClip.header.duration = maxChannelDuration;

    rawClips.push_back(rawClip);
  }

  return rawClips;
}


