#include "precompiled.h"

#pragma hdrstop

#include <fstream>
#include <streambuf>
#include <bitset>
#include <algorithm>

#include "SkeletonResourceManager.h"
#include "Exceptions/exceptions.h"

#include "Modules/Graphics/Resources/Raw/RawSkeleton.h"

SkeletonResourceManager::SkeletonResourceManager(ResourcesManager* resourcesManager)
  : ResourceManager<Skeleton, SkeletonResourceConfig>(resourcesManager)
{

}

SkeletonResourceManager::~SkeletonResourceManager() = default;

void SkeletonResourceManager::load(size_t resourceIndex)
{
  SkeletonResourceConfig* config = getResourceConfig(resourceIndex);

  // Read raw mesh
  RawSkeleton rawSkeleton = RawSkeleton::readFromFile(config->resourcePath);

  // Convert raw skeleton to internal skeleton object
  std::vector<Bone> bones(rawSkeleton.bones.size());

  for (size_t boneIndex = 0; boneIndex < rawSkeleton.bones.size(); boneIndex++) {
    const RawBone& rawBone = rawSkeleton.bones[boneIndex];

    bones[boneIndex].setParentId(rawBone.parentId);
    bones[boneIndex].setName(std::string(rawBone.name));
    bones[boneIndex].setInverseBindPoseMatrix(rawMatrix4ToGLMMatrix4(rawBone.inverseBindPoseMatrix));
  }

  allocateResource<Skeleton>(resourceIndex, bones);
}

void SkeletonResourceManager::parseConfig(size_t resourceIndex, pugi::xml_node configNode)
{
  SkeletonResourceConfig* resourceConfig = createResourceConfig(resourceIndex);
  resourceConfig->resourcePath = configNode.attribute("source").as_string();

  if (!FileUtils::isFileExists(resourceConfig->resourcePath)) {
    THROW_EXCEPTION(EngineRuntimeException,
      fmt::format("Skeleton resource refer to not existing file", resourceConfig->resourcePath));
  }
}
