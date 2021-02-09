#include "MeshImporter.h"

#include <spdlog/spdlog.h>

#include <string>
#include <bitset>
#include <Engine/Exceptions/exceptions.h>
#include <Engine/swdebug.h>

#include "utils.h"
#include "AssimpMeshLoader.h"
#include "SkeletonImporter.h"

MeshImporter::MeshImporter()
{

}

std::unique_ptr<RawMesh> MeshImporter::importFromFile(const std::string& path, const MeshImportOptions& options)
{
  spdlog::info("Load source mesh: {}", path);

  AssimpMeshLoadOptions assimpOptions;
  assimpOptions.flipUV = options.flipUV;
  assimpOptions.glueByMaterials = options.glueByMaterials;
  assimpOptions.calculateTangents = options.calculateTangents;
  assimpOptions.joinIdenticalVertices = options.joinIdenticalVertices;
  assimpOptions.maxBonexPerVertex = options.maxBonesPerVertex;
  assimpOptions.loadRiggingData = options.loadSkin;

  std::unique_ptr<AssimpScene> scene = AssimpMeshLoader::loadScene(path, assimpOptions);

  spdlog::info("Source mesh is loaded");
  spdlog::info("Start mesh parsing");

  std::unique_ptr<RawSkeleton> skeleton = nullptr;

  if (options.loadSkin) {
    spdlog::info("Start to load mesh skeleton...");

    skeleton = getSkeleton(path, options);

    spdlog::info("Mesh skeleton is loaded");
  }

  std::unique_ptr<RawMesh> mesh = convertSceneToMesh(scene->getScene(), skeleton.get(), options);

  size_t totalIndicesCount = std::accumulate(mesh->subMeshesDescriptions.begin(),
    mesh->subMeshesDescriptions.end(), 0,
    [](uint32_t sum, RawSubMeshDescription& subMeshDescription) {
      return sum + subMeshDescription.indicesCount;
    });

  spdlog::info("Mesh is parsed ({} vertices, {} indices, {} submeshes)",
    mesh->header.verticesCount, totalIndicesCount,
    mesh->header.subMeshesCount);

  return mesh;
}

std::unique_ptr<RawMesh> MeshImporter::convertSceneToMesh(const aiScene& scene,
  const RawSkeleton* skeleton,
  const MeshImportOptions& options)
{
  SW_ASSERT(!options.loadSkin || (options.loadSkin && skeleton != nullptr));

  std::unordered_map<std::string, int> bonesMap;

  if (options.loadSkin) {
    bonesMap = getBonesMap(*skeleton);
  }

  std::vector<RawVector3> positions;
  std::vector<RawVector3> normals;
  std::vector<RawVector3> tangents;
  std::vector<RawVector2> uv;

  std::vector<uint8_t> bonesFreeDataPosition;
  std::vector<RawU8Vector4> bonesIDs;

  std::vector<glm::vec4> bonesWeights;
  std::vector<RawU8Vector4> convertedBonesWeights;

  std::vector<std::vector<std::uint16_t>> subMeshesIndices;

  glm::vec3 aabbMin(std::numeric_limits<float>::max());
  glm::vec3 aabbMax(std::numeric_limits<float>::min());

  std::unordered_map<std::string, ImportMeshData> meshesList;

  aiMatrix4x4 rootTransform;
  aiIdentityMatrix4(&rootTransform);

  collectMeshes(scene, *scene.mRootNode, meshesList, rootTransform);

  if (meshesList.empty()) {
    THROW_EXCEPTION(EngineRuntimeException, "Failed to import mesh, geometry is not found");
  }

  // TODO: this loop is so long and complex, decompose and simplify it
  for (auto[subMeshName, subMeshPtr] : meshesList) {
    const aiMesh& subMesh = *subMeshPtr.mesh;
    size_t subMeshIndex = subMeshesIndices.size() - 1;

    if (subMeshName.starts_with("collider")) {
      spdlog::info("Skip collider {}", subMeshName);

      continue;
    }

    spdlog::info("Import submesh {}", subMeshName);

    bool requiredAttributesFound = subMesh.HasPositions() && subMesh.HasNormals()
      && subMesh.HasTextureCoords(0) && subMesh.HasFaces() && subMesh.HasTangentsAndBitangents();

    if (!requiredAttributesFound) {
      spdlog::info("Submesh #{} ({}) is incomplete and was skipped", subMeshIndex, subMeshName);
      continue;
    }

    // Vertices
    size_t verticesAddIndex = positions.size();

    aiIdentityMatrix4(&subMeshPtr.sceneTransfromationMatrix);

    for (size_t vertexIndex = 0; vertexIndex < subMesh.mNumVertices; vertexIndex++) {
      RawVector3 position = getTransformedRawVector(subMesh.mVertices[vertexIndex],
        subMeshPtr.sceneTransfromationMatrix, true);

      positions.push_back(position);

      aabbMin = glm::min(aabbMin, {position.x, position.y, position.z});
      aabbMax = glm::max(aabbMax, {position.x, position.y, position.z});

      normals.push_back(getTransformedRawVector(subMesh.mNormals[vertexIndex],
        subMeshPtr.sceneTransfromationMatrix, false, true));

      tangents.push_back(getTransformedRawVector(subMesh.mTangents[vertexIndex],
        subMeshPtr.sceneTransfromationMatrix, false, true));

      uv.push_back({subMesh.mTextureCoords[0][vertexIndex].x,
        subMesh.mTextureCoords[0][vertexIndex].y});

      bonesIDs.push_back({0, 0, 0, 0});
      bonesWeights.push_back({0.0f, 0.0f, 0.0f, 0.0f});
      convertedBonesWeights.push_back({0, 0, 0, 0});
      bonesFreeDataPosition.push_back(0);
    }

    // Indices
    bool nonTrianglePolygonFound = false;

    size_t indicesOffset = verticesAddIndex;

    std::vector<uint16_t> indices;

    for (size_t faceIndex = 0; faceIndex < subMesh.mNumFaces; faceIndex++) {
      const aiFace& face = subMesh.mFaces[faceIndex];

      if (face.mNumIndices != 3) {
        nonTrianglePolygonFound = true;
        break;
      }

      for (size_t indexNumber = 0; indexNumber < 3; indexNumber++) {
        indices.push_back(static_cast<uint16_t>(face.mIndices[indexNumber] + indicesOffset));
      }
    }

    if (nonTrianglePolygonFound) {
      spdlog::info("Submesh #{} ({}) has non-triangle polygon and was skipped", subMeshIndex, subMeshName);
      continue;
    }

    if (options.loadSkin) {
      if (!subMesh.HasBones()) {
        spdlog::warn("Submesh #{} ({}) has not any attached bones", subMeshIndex, subMeshName);
      }

      for (size_t boneIndex = 0; boneIndex < subMesh.mNumBones; boneIndex++) {
        const aiBone& bone = *subMesh.mBones[boneIndex];

        std::string boneName = bone.mName.C_Str();

        auto rawBoneIt = bonesMap.find(boneName);
        int skeletonBoneIndex = rawBoneIt->second;

        if (rawBoneIt == bonesMap.end()) {
          THROW_EXCEPTION(EngineRuntimeException,
            "Bone " + boneName + " that is attached to the submesh is not found in the skeleton");
        }

        for (size_t weightIndex = 0; weightIndex < bone.mNumWeights; weightIndex++) {
          const aiVertexWeight& vertexWeight = bone.mWeights[weightIndex];

          size_t affectedVertexId = verticesAddIndex + vertexWeight.mVertexId;
          float weight = vertexWeight.mWeight;

          uint8_t boneDataPosition = bonesFreeDataPosition[affectedVertexId];

          SW_ASSERT(boneDataPosition < options.maxBonesPerVertex);

          bonesIDs[affectedVertexId].data[boneDataPosition] = static_cast<uint8_t>(skeletonBoneIndex);
          bonesWeights[affectedVertexId][boneDataPosition] = weight;

          bonesFreeDataPosition[affectedVertexId]++;
        }
      }
    }

    // Correct bones influence weights
    int unskinnedVerticesCount = 0;

    for (size_t weightIndex = 0; weightIndex < bonesWeights.size(); weightIndex++) {
      const glm::vec4& weight = bonesWeights[weightIndex];

      std::uint8_t remainingSum = 255;

      for (size_t weightComponentIndex = 0; weightComponentIndex < 4; weightComponentIndex++) {
        float sourceWeight = weight[static_cast<int>(weightComponentIndex)];
        auto convertedWeight = static_cast<std::uint8_t>(std::round(sourceWeight * 255));

        if (convertedWeight > remainingSum || weightComponentIndex == 3) {
          convertedBonesWeights[weightIndex].data[weightComponentIndex] = remainingSum;

          break;
        }
        else {
          remainingSum -= convertedWeight;
          convertedBonesWeights[weightIndex].data[weightComponentIndex] = convertedWeight;
        }
      }

      SW_ASSERT(convertedBonesWeights[weightIndex].x + convertedBonesWeights[weightIndex].y +
        convertedBonesWeights[weightIndex].z + convertedBonesWeights[weightIndex].w == 255);
    }

    if (options.loadSkin && unskinnedVerticesCount > 0) {
      spdlog::warn("There is {} unskinned vertices", unskinnedVerticesCount);
    }

    subMeshesIndices.push_back(std::move(indices));
  }

  // Mesh formation
  SW_ASSERT(positions.size() == normals.size() && positions.size() == uv.size());

  std::unique_ptr<RawMesh> mesh = std::make_unique<RawMesh>();

  mesh->positions = positions;
  mesh->normals = normals;
  mesh->tangents = tangents;
  mesh->uv = uv;
  mesh->bonesIds = bonesIDs;
  mesh->bonesWeights = convertedBonesWeights;

  for (const auto& subMeshIndices : subMeshesIndices) {
    mesh->subMeshesDescriptions.push_back(RawSubMeshDescription{
      .indicesCount = static_cast<uint16_t>(subMeshIndices.size()),
      .indices = subMeshIndices
    });
  }

  mesh->aabb = AABB(aabbMin, aabbMax);

  mesh->header.formatVersion = MESH_FORMAT_VERSION;
  mesh->header.verticesCount = static_cast<uint16_t>(mesh->positions.size());
  mesh->header.subMeshesCount = static_cast<uint16_t>(subMeshesIndices.size());

  RawMeshAttributes storedAttributesMask = RawMeshAttributes::Positions | RawMeshAttributes::Normals |
    RawMeshAttributes::UV | RawMeshAttributes::Tangents;

  if (options.loadSkin) {
    storedAttributesMask = storedAttributesMask | RawMeshAttributes::BonesIDs | RawMeshAttributes::BonesWeights;
  }

  mesh->header.storedAttributesMask = static_cast<bitmask64>(storedAttributesMask);

  return mesh;
}

void MeshImporter::collectMeshes(const aiScene& scene,
  const aiNode& sceneNode,
  std::unordered_map<std::string, ImportMeshData>& meshesList,
  const aiMatrix4x4& parentNodeTransform)
{
  std::string currentNodeName = sceneNode.mName.C_Str();
  aiMatrix4x4 currentNodeTransform = parentNodeTransform * sceneNode.mTransformation;

  for (size_t meshIndex = 0; meshIndex < sceneNode.mNumMeshes; meshIndex++) {
    const aiMesh* mesh = scene.mMeshes[sceneNode.mMeshes[meshIndex]];

    std::string meshName = mesh->mName.C_Str();

    auto meshIt = meshesList.find(meshName);

    if (meshIt != meshesList.end()) {
      // The mesh is already collected
      // Nodes structure and ability to use one mesh in two different nodes is ignored

      spdlog::warn("The same mesh is attached to multiple nodes ({}), attachment is skipped (node {})",
        meshName, currentNodeName);
      continue;
    }

    if (!currentNodeTransform.IsIdentity()) {
      spdlog::warn("The mesh {} node {} has non-identity transform, "
                   "vertices will be converted to scene space", meshName, currentNodeName);
    }

    meshesList.insert({meshName, {mesh, currentNodeTransform}});
  }

  for (size_t childIndex = 0; childIndex < sceneNode.mNumChildren; childIndex++) {
    const aiNode* childNode = sceneNode.mChildren[childIndex];

    collectMeshes(scene, *childNode, meshesList, currentNodeTransform);
  }
}

std::unique_ptr<RawSkeleton> MeshImporter::getSkeleton(const std::string& path,
  const MeshImportOptions& options)
{
  SkeletonImporter importer;
  SkeletonImportOptions importOptions;
  importOptions.maxBonexPerVertex = options.maxBonesPerVertex;

  return importer.importFromFile(path, importOptions);
}

std::unordered_map<std::string, int> MeshImporter::getBonesMap(const RawSkeleton& skeleton)
{
  std::unordered_map<std::string, int> bonesMap;

  for (size_t boneIndex = 0; boneIndex < skeleton.bones.size(); boneIndex++) {
    const RawBone& bone = skeleton.bones[boneIndex];
    bonesMap[std::string(bone.name)] = static_cast<int>(boneIndex);
  }

  return bonesMap;
}
