#include "SceneImporter.h"

#include <fstream>
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

  traceSceneDebugInformation(model, scene);
  validateScene(model, scene);

  auto rawScene = std::make_unique<RawScene>();
  rawScene->meshesNodes = convertSceneToRawData(model, scene);

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
  if (model.buffers.size() != 1) {
    raiseImportError("Models with multiple buffers are not supported yet");
  }

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

  if (node.skin != -1 || !node.weights.empty()) {
    raiseImportError("Nodes skinning are not supported yet\"");
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
      else {
        raiseImportError(fmt::format("Attribute {} is not supported yet", attribute.first));
      }
    }
  }
}

std::vector<RawMeshNode> SceneImporter::convertSceneToRawData(const tinygltf::Model& model,
  const tinygltf::Scene& scene)
{
  ARG_UNUSED(model);
  ARG_UNUSED(scene);

  std::vector<RawMeshNode> rawNodesList;

  traverseScene(model, scene, [this, &rawNodesList](const tinygltf::Model& model,
    const tinygltf::Scene& scene,
    const glm::mat4& nodeTransform,
    const tinygltf::Node& node) {
    if (node.name.find("collision") != std::string::npos) {
      return;
    }

    rawNodesList.push_back(convertMeshNodeToRawData(model, scene, nodeTransform, node));
  });

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
  rawNode.rawMesh.header.subMeshesIndicesOffsetsCount = 0;
  rawNode.rawMesh.header.indicesCount = 0;
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

    size_t rawMeshIndicesOffset = rawNode.rawMesh.indices.size();
    rawNode.rawMesh.indices.resize(rawMeshIndicesOffset + indicesCount);

    auto[indicesBufferPtr, indicesBufferStride] = getAttributeBufferStorage(model, indexAccessor);

    for (size_t indexNumber = 0; indexNumber < indexAccessor.count; indexNumber++) {
      rawNode.rawMesh.indices[rawMeshIndicesOffset + indexNumber] = reinterpret_cast<const uint16_t*>(
        indicesBufferPtr)[0] + static_cast<uint16_t>(rawMeshVerticesOffset);
      indicesBufferPtr += indicesBufferStride;
    }

    rawNode.rawMesh.subMeshesIndicesOffsets.push_back(uint16_t(rawMeshIndicesOffset));

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
      else {
        SW_ASSERT(false);
      }
    }
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

    const tinygltf::Mesh& colliderMesh = model.meshes[colliderMeshNode.mesh];

    std::string colliderMeshNamePrefix = mesh.name + "_" + "collision" + "_";

    if (!colliderMesh.name.starts_with(colliderMeshNamePrefix)) {
      continue;
    }

    if (colliderMesh.name.starts_with(colliderMeshNamePrefix + "no_collision")) {
      collisionsResolutionDisabled = true;
      break;
    }

    std::string colliderType = StringUtils::split(colliderMesh.name.substr(
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

      spdlog::info("Load AABB collider {}, min={}, max={}", colliderMesh.name,
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

      spdlog::info("Load sphere collider {}, origin={}, radius={}", glm::to_string(origin), radius);
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

      spdlog::info("Load triangle mesh collider {}, vertices_count=", collisionMeshVertices.size());
    }
    else {
      raiseImportError(fmt::format("Collision mesh type {} is not supported", colliderMesh.name));
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

  rawNode.rawMesh.header.subMeshesIndicesOffsetsCount =
    static_cast<uint16_t>(rawNode.rawMesh.subMeshesIndicesOffsets.size());
  rawNode.rawMesh.header.indicesCount = static_cast<uint16_t>(rawNode.rawMesh.indices.size());
  rawNode.rawMesh.header.verticesCount = static_cast<uint16_t>(rawNode.rawMesh.positions.size());

//  if (rawNode.rawMesh.header.subMeshesIndicesOffsetsCount > 1) {
//    spdlog::debug("Mesh with submeshes is converted");
//  }

  size_t rawMeshVerticesCount = rawNode.rawMesh.positions.size();

  if (!rawNode.rawMesh.normals.empty() && rawNode.rawMesh.normals.size() != rawMeshVerticesCount ||
    !rawNode.rawMesh.tangents.empty() && rawNode.rawMesh.tangents.size() != rawMeshVerticesCount ||
    !rawNode.rawMesh.uv.empty() && rawNode.rawMesh.uv.size() != rawMeshVerticesCount ||
    !rawNode.rawMesh.bonesIds.empty() && rawNode.rawMesh.bonesIds.size() != rawMeshVerticesCount ||
    !rawNode.rawMesh.bonesWeights.empty() && rawNode.rawMesh.bonesWeights.size() != rawMeshVerticesCount) {
    raiseImportError(fmt::format("Mesh {} is in inconsistent state because of different attributes count", mesh.name));
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
  glm::mat4 nodeTransform = getMeshNodeTransform(node) * parentNodeTransform;

  if (withMeshesOnly) {
    if (node.mesh != -1) {
      visitor(model, scene, nodeTransform, node);
    }
  }
  else {
    visitor(model, scene, nodeTransform, node);
  }

  for (int childNodeIndex : node.children) {
    traverseSceneInternal(model, scene, nodeTransform, model.nodes[childNodeIndex], visitor);
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

