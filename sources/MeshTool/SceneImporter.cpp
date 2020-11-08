#include "SceneImporter.h"

#include <fstream>
#include <spdlog/spdlog.h>

#include <Engine/swdebug.h>
#include <Engine/Exceptions/exceptions.h>

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

  bool loadingResult = loader.LoadASCIIFromFile(&model, &errors, &warnings, path);

  if (!warnings.empty()) {
    spdlog::warn("Warnings: {}", warnings);
  }

  if (!errors.empty()) {
    spdlog::warn("Errors: {}", warnings);
  }
  if (!loadingResult) {
    spdlog::critical("It is impossible to load glTF scene {}, result {}", path, loadingResult);

    THROW_EXCEPTION(EngineRuntimeException, fmt::format("It is impossible to load glTF scene {}", path));
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

  for (int nodeIndex : scene.nodes) {
    const tinygltf::Node& node = model.nodes[nodeIndex];
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
    spdlog::critical("Models with multiple buffers are not supported yet");
    THROW_EXCEPTION(EngineRuntimeException,
      "Buffer views stride attribute is not supported yet");
  }

  for (int nodeIndex : scene.nodes) {
    const tinygltf::Node& node = model.nodes[nodeIndex];
    const tinygltf::Mesh& mesh = model.meshes[node.mesh];

    if (node.children.size() > 1) {
      spdlog::critical("Nodes hierarchies are not supported yet, so it is needed to flatten the scene");
      THROW_EXCEPTION(EngineRuntimeException, "Nodes hierarchies are not supported");
    }

    if (!node.rotation.empty() || !node.scale.empty() || node.skin != -1 || !node.weights.empty()) {
      spdlog::critical("Nodes rotation, scale and skinning are not supported yet");
      THROW_EXCEPTION(EngineRuntimeException, "Nodes rotation, scale and skinning are not supported yet");
    }

    if (mesh.primitives.size() != 1) {
      spdlog::critical("Mesh should have only one primitives set with triangle primitives");
      THROW_EXCEPTION(EngineRuntimeException, "Mesh should have only one primitives set with triangle primitives");
    }

    auto& primitive = mesh.primitives[0];

    if (primitive.mode != TINYGLTF_MODE_TRIANGLES) {
      spdlog::critical("Non-triangle primitives are not supported yet");
      THROW_EXCEPTION(EngineRuntimeException, "Non-triangle primitives are not supported yet");
    }

    const tinygltf::Accessor& indexAccessor =
      model.accessors[primitive.indices];

    if (indexAccessor.sparse.isSparse) {
      spdlog::critical("Sparse accessors are not supported yet, so it is needed to flatten the index buffers");
      THROW_EXCEPTION(EngineRuntimeException, "Sparse accessors are not supported");
    }

    if (indexAccessor.normalized || indexAccessor.componentType != TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT ||
      indexAccessor.type != TINYGLTF_TYPE_SCALAR) {
      spdlog::critical("Index accessors should not be normalized and should have scalar unsigned short type");
      THROW_EXCEPTION(EngineRuntimeException,
        "Index accessors should not be GL-normalized and should have scalar unsigned short type");
    }

    const tinygltf::BufferView& indexBufferView = model.bufferViews[indexAccessor.bufferView];

    if (indexBufferView.byteStride != 0) {
      spdlog::critical("Buffer views stride attribute are not supported yet");
      THROW_EXCEPTION(EngineRuntimeException,
        "Buffer views stride attribute are not supported yet");
    }

    if (primitive.attributes.empty()) {
      spdlog::critical("Primitive should have at least one vertex attribute");
      THROW_EXCEPTION(EngineRuntimeException, "Primitive should have at least one vertex attribute");
    }

    size_t firstAttributeCount = model.accessors[(*primitive.attributes.begin()).second].count;

    for (auto& attribute : primitive.attributes) {
      const tinygltf::Accessor& accessor = model.accessors[attribute.second];

      if (accessor.count != firstAttributeCount) {
        spdlog::critical("All vertices attributes should have equal number of values");
        THROW_EXCEPTION(EngineRuntimeException, "All vertices attributes should have equal number of values");
      }

      if (accessor.sparse.isSparse) {
        spdlog::critical("Sparse accessors are not supported yet, so it is needed to flatten the vertex buffers");
        THROW_EXCEPTION(EngineRuntimeException, "Sparse accessors are not supported");
      }

      if (attribute.first == "POSITION") {
        if (accessor.normalized || accessor.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT ||
          accessor.type != TINYGLTF_TYPE_VEC3) {
          spdlog::critical("Position accessors should not be normalized and should have vec3 type");
          THROW_EXCEPTION(EngineRuntimeException,
            "Position accessors should not be GL-normalized and should have vec3 type");
        }
      }
      else if (attribute.first == "NORMAL") {
        if (accessor.normalized || accessor.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT ||
          accessor.type != TINYGLTF_TYPE_VEC3) {
          spdlog::critical("Normal accessors should not be normalized and should have vec3 type");
          THROW_EXCEPTION(EngineRuntimeException,
            "Normal accessors should not be GL-normalized and should have vec3 type");
        }
      }
      else if (attribute.first == "TANGENT") {
        if (accessor.normalized || accessor.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT ||
          accessor.type != TINYGLTF_TYPE_VEC4) {
          spdlog::critical("Normal accessors should not be normalized and should have vec4 type");
          THROW_EXCEPTION(EngineRuntimeException,
            "Tangents accessors should not be GL-normalized and should have vec4 type");
        }
      }
      else if (attribute.first == "TEXCOORD_0") {
        if (accessor.normalized || accessor.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT ||
          accessor.type != TINYGLTF_TYPE_VEC2) {
          spdlog::critical("UV0 accessors should not be normalized and should have vec2 type");
          THROW_EXCEPTION(EngineRuntimeException,
            "UV0 accessors should not be GL-normalized and should have vec2 type");
        }
      }
      else {
        spdlog::critical("Attribute {} is not supported yet", attribute.first);
        THROW_EXCEPTION(EngineRuntimeException, fmt::format("Attribute {} is not supported yet", attribute.first));
      }

//      const tinygltf::BufferView& attributeBufferView = model.bufferViews[accessor.bufferView];
//
//      if (attributeBufferView.byteStride != 0) {
//        spdlog::critical("Buffer views stride attribute are not supported yet");
//        THROW_EXCEPTION(EngineRuntimeException,
//          "Buffer views stride attribute are not supported yet");
//      }
    }

  }
}

std::vector<RawMeshNode> SceneImporter::convertSceneToRawData(const tinygltf::Model& model,
  const tinygltf::Scene& scene)
{
  ARG_UNUSED(model);
  ARG_UNUSED(scene);

  std::vector<RawMeshNode> rawNodesList;

  for (int nodeIndex : scene.nodes) {
    const tinygltf::Node& node = model.nodes[nodeIndex];
    const tinygltf::Mesh& mesh = model.meshes[node.mesh];

    RawMeshNode rawNode;
    rawNode.position.x = static_cast<float>(node.translation[0]);
    rawNode.position.y = static_cast<float>(node.translation[1]);
    rawNode.position.z = static_cast<float>(node.translation[2]);

    auto orientation = glm::identity<glm::quat>();
    rawNode.orientation = {orientation.x, orientation.y, orientation.z, orientation.w};

    strncpy_s(rawNode.name, node.name.c_str(), sizeof(rawNode.name));

    auto& primitive = mesh.primitives[0];

    const tinygltf::Accessor& indexAccessor =
      model.accessors[primitive.indices];

    size_t verticesCount = model.accessors[(*primitive.attributes.begin()).second].count;
    size_t indicesCount = indexAccessor.count;

    rawNode.rawMesh.header.formatVersion = MESH_FORMAT_VERSION;
    rawNode.rawMesh.header.subMeshesIndicesOffsetsCount = 1;
    rawNode.rawMesh.header.indicesCount = static_cast<uint16_t>(indicesCount);
    rawNode.rawMesh.header.verticesCount = static_cast<uint16_t>(verticesCount);
    rawNode.rawMesh.header.storedAttributesMask = 0;

    rawNode.rawMesh.indices.resize(indicesCount);

    const tinygltf::BufferView& indexBufferView = model.bufferViews[indexAccessor.bufferView];
    const tinygltf::Buffer& indexBuffer = model.buffers[indexBufferView.buffer];

    const auto
      * indicesBufferPtr = reinterpret_cast<const unsigned char*>(indexBuffer.data.data() + indexBufferView.byteOffset
      + indexAccessor.byteOffset);

    size_t indicesBufferStride = indexAccessor.ByteStride(indexBufferView);
    SW_ASSERT(indicesBufferStride > 0);

    for (size_t indexNumber = 0; indexNumber < indexAccessor.count; indexNumber++) {
      rawNode.rawMesh.indices[indexNumber] = reinterpret_cast<const uint16_t*>(indicesBufferPtr)[0];
      indicesBufferPtr += indicesBufferStride;
    }

    rawNode.rawMesh.subMeshesIndicesOffsets.push_back(0);

    for (auto& attribute : primitive.attributes) {
      const tinygltf::Accessor& accessor = model.accessors[attribute.second];

      const tinygltf::BufferView& attributeBufferView = model.bufferViews[accessor.bufferView];
      const tinygltf::Buffer& attributeBuffer = model.buffers[attributeBufferView.buffer];

      const auto* attributeBufferPtr =
        reinterpret_cast<const unsigned char*>(attributeBuffer.data.data() + attributeBufferView.byteOffset
          + accessor.byteOffset);

      size_t attributeBufferStride = accessor.ByteStride(attributeBufferView);

      if (attribute.first == "POSITION") {
        rawNode.rawMesh.positions.resize(verticesCount);

        for (size_t vertexIndex = 0; vertexIndex < verticesCount; vertexIndex++) {
          rawNode.rawMesh.positions[vertexIndex].x = reinterpret_cast<const float*>(attributeBufferPtr)[0];
          rawNode.rawMesh.positions[vertexIndex].y = reinterpret_cast<const float*>(attributeBufferPtr)[1];
          rawNode.rawMesh.positions[vertexIndex].z = reinterpret_cast<const float*>(attributeBufferPtr)[2];
          attributeBufferPtr += attributeBufferStride;
        }

        rawNode.rawMesh.header.storedAttributesMask |= static_cast<size_t>(RawMeshAttributes::Positions);
      }
      else if (attribute.first == "NORMAL") {
        rawNode.rawMesh.normals.resize(verticesCount);

        for (size_t vertexIndex = 0; vertexIndex < verticesCount; vertexIndex++) {
          rawNode.rawMesh.normals[vertexIndex].x = reinterpret_cast<const float*>(attributeBufferPtr)[0];
          rawNode.rawMesh.normals[vertexIndex].y = reinterpret_cast<const float*>(attributeBufferPtr)[1];
          rawNode.rawMesh.normals[vertexIndex].z = reinterpret_cast<const float*>(attributeBufferPtr)[2];
          attributeBufferPtr += attributeBufferStride;
        }

        rawNode.rawMesh.header.storedAttributesMask |= static_cast<size_t>(RawMeshAttributes::Normals);
      }
      else if (attribute.first == "TANGENT") {
        rawNode.rawMesh.tangents.resize(verticesCount);

        for (size_t vertexIndex = 0; vertexIndex < verticesCount; vertexIndex++) {
          rawNode.rawMesh.tangents[vertexIndex].x = reinterpret_cast<const float*>(attributeBufferPtr)[0];
          rawNode.rawMesh.tangents[vertexIndex].y = reinterpret_cast<const float*>(attributeBufferPtr)[1];
          rawNode.rawMesh.tangents[vertexIndex].z = reinterpret_cast<const float*>(attributeBufferPtr)[2];

          // TODO: store also w component

          attributeBufferPtr += attributeBufferStride;
        }

        rawNode.rawMesh.header.storedAttributesMask |= static_cast<size_t>(RawMeshAttributes::Tangents);
      }
      else if (attribute.first == "TEXCOORD_0") {
        rawNode.rawMesh.uv.resize(verticesCount);

        for (size_t vertexIndex = 0; vertexIndex < verticesCount; vertexIndex++) {
          rawNode.rawMesh.uv[vertexIndex].x = reinterpret_cast<const float*>(attributeBufferPtr)[0];
          rawNode.rawMesh.uv[vertexIndex].y = reinterpret_cast<const float*>(attributeBufferPtr)[1];
          attributeBufferPtr += attributeBufferStride;
        }

        rawNode.rawMesh.header.storedAttributesMask |= static_cast<size_t>(RawMeshAttributes::UV);
      }
      else {
        SW_ASSERT(false);
      }
    }

    glm::vec3 aabbMin(std::numeric_limits<float>::max());
    glm::vec3 aabbMax(std::numeric_limits<float>::min());

    for (const auto& position : rawNode.rawMesh.positions) {
      aabbMin = glm::min(aabbMin, {position.x, position.y, position.z});
      aabbMax = glm::max(aabbMax, {position.x, position.y, position.z});
    }

    rawNode.rawMesh.aabb = AABB(aabbMin, aabbMax);

    rawNodesList.push_back(std::move(rawNode));
  }

  spdlog::info("Scene conversion to raw format is finished");

  return rawNodesList;
}
