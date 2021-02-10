#include "AssetsDump.h"

#include <bitset>
#include <spdlog/spdlog.h>
#include <Engine/Utility/files.h>
#include <Engine/Utility/strings.h>
#include <Engine/Exceptions/exceptions.h>

void AssetsDump::dumpAssetData(const std::string& assetPath)
{
  std::string assetExtension = FileUtils::getFileExtension(assetPath);

  if (assetExtension == "mesh") {
    dumpMesh(RawMesh::readFromFile(assetPath));
  }
  else if (assetExtension == "skeleton") {
    dumpSkeleton(RawSkeleton::readFromFile(assetPath));
  }
  else if (assetExtension == "anim") {
    dumpAnimationClip(RawSkeletalAnimationClip::readFromFile(assetPath));
  }
  else {
    THROW_EXCEPTION(EngineRuntimeException,
      fmt::format("It is impossible to determine asset type. Path: {}",
        assetPath));
  }
}

void AssetsDump::dumpMesh(const RawMesh& mesh)
{
  static_assert(MESH_FORMAT_VERSION == 116 && "Do not forget to update dump logic");

  std::unordered_map<size_t, std::string> meshAttributesNames = {
    {0, "None"},
    {1, "Positions"},
    {2, "Normals"},
    {3, "UV"},
    {4, "Tangents"},
    {5, "BoneIDs"},
    {6, "BonesWeights"},
  };

  std::vector<std::string> meshAttributesList;
  std::bitset<64> meshAttributesBitset(mesh.header.storedAttributesMask);

  for (const auto&[attributeBitIndex, attributeName] : meshAttributesNames) {
    if (meshAttributesBitset.test(attributeBitIndex)) {
      meshAttributesList.push_back(attributeName);
    }
  }

  fmt::print("Raw mesh asset:\n"
             "  Header:"
             "    formatVersion: {0}\n"
             "    verticesCount: {1}\n"
             "    attributesMask: {2}, transcription {3}\n"
             "    subMeshesCount: {4}\n"
             "  Mesh data:\n"
             "", mesh.header.formatVersion,
    mesh.header.verticesCount,
    mesh.header.storedAttributesMask, StringUtils::join(meshAttributesList), mesh.header.subMeshesCount);

  dumpVectorSection("    vertices:", mesh.positions);
  dumpVectorSection("    normals:", mesh.normals);
  dumpVectorSection("    tangents:", mesh.tangents);
  dumpVectorSection("    uv:", mesh.uv);
  dumpVectorSection("    bonesIds:", mesh.bonesIds);
  dumpVectorSection("    bonesWeights:", mesh.bonesWeights);

  fmt::print("  Submeshes:\n");

  for (size_t subMeshIndex = 0; subMeshIndex < mesh.subMeshesDescriptions.size(); subMeshIndex++) {
    fmt::print("    Submesh #{0}:\n      Indices count: {1}\n      Indices: {2}",
      subMeshIndex, mesh.subMeshesDescriptions[subMeshIndex].indicesCount,
      fmt::join(mesh.subMeshesDescriptions[subMeshIndex].indices, ", "));
  }

  fmt::print("  AABB:\n"
             "    Min: {0}\n"
             "    Max: {1}\n"
             "  inverseSceneTransform: {2}\n",
    glm::to_string(rawVector3ToGLMVector3(mesh.aabb.min)),
    glm::to_string(rawVector3ToGLMVector3(mesh.aabb.max)),
    glm::to_string(rawMatrix4ToGLMMatrix4(mesh.inverseSceneTransform)));
}

void AssetsDump::dumpSkeleton(const RawSkeleton& skeleton)
{
  static_assert(SKELETON_FORMAT_VERSION == 112 && "Do not forget to update dump logic");

  fmt::print("Raw skeleton asset:\n"
             "  Header:"
             "    formatVersion: {0}\n"
             "    name: {1}\n"
             "    bonesCount: {2}\n"
             "", skeleton.header.formatVersion,
    skeleton.header.name,
    skeleton.header.bonesCount);

  fmt::print("  Bones:\n");

  for (size_t boneIndex = 0; boneIndex < skeleton.bones.size(); boneIndex++) {
    const RawBone& bone = skeleton.bones[boneIndex];

    fmt::print("    Bone #{0}:\n      name: {1}\n      parentId: {2}\n      inverseBindPoseMatrix: {3}\n",
      boneIndex, bone.name, bone.parentId, glm::to_string(rawMatrix4ToGLMMatrix4(bone.inverseBindPoseMatrix)));
  }
}

void AssetsDump::dumpAnimationClip(const RawSkeletalAnimationClip& animationClip)
{
  static_assert(ANIMATION_FORMAT_VERSION == 112 && "Do not forget to update dump logic");

  std::vector<RawBoneAnimationChannel> bonesAnimationChannels;

  fmt::print("Raw animation clip asset:\n"
             "  Header:"
             "    formatVersion: {0}\n"
             "    name: {1}\n"
             "    skeletonBonesCount: {2}\n"
             "    duration: {3}\n"
             "    rate: {4}\n"
             "", animationClip.header.formatVersion,
    animationClip.header.name,
    animationClip.header.skeletonBonesCount,
    animationClip.header.duration,
    animationClip.header.rate);

  fmt::print("  Channels:\n");

  for (size_t channelIndex = 0; channelIndex < animationClip.bonesAnimationChannels.size(); channelIndex++) {
    const RawBoneAnimationChannel& channel = animationClip.bonesAnimationChannels[channelIndex];

    fmt::print("    Channel #{0}:\n"
               "      positionsFramesCount: {1}\n"
               "      orientationFramesCount: {2}\n",
      channelIndex, channel.header.positionFramesCount, channel.header.orientationFramesCount);

    dumpVectorSection("        positionFrames:", channel.positionFrames);
    dumpVectorSection("        orientationFrames:", channel.orientationFrames);

  }
}
