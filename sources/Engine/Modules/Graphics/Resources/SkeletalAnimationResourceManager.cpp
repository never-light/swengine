#include "precompiled.h"

#pragma hdrstop

#include <fstream>
#include <streambuf>
#include <bitset>
#include <algorithm>

#include "SkeletalAnimationResourceManager.h"
#include "Exceptions/exceptions.h"
#include "Utility/memory.h"

#include "Modules/Graphics/Resources/Raw/RawSkeletalAnimationClip.h"

SkeletalAnimationResourceManager::SkeletalAnimationResourceManager(ResourcesManager* resourcesManager)
  : ResourceManager<AnimationClip, SkeletalAnimationResourceConfig>(resourcesManager)
{

}

SkeletalAnimationResourceManager::~SkeletalAnimationResourceManager() = default;

void SkeletalAnimationResourceManager::load(size_t resourceIndex)
{
  SkeletalAnimationResourceConfig* config = getResourceConfig(resourceIndex);

  // Read raw mesh
  RawSkeletalAnimationClip rawClip = RawSkeletalAnimationClip::readFromFile(config->resourcePath);

  // Convert raw animation clip to internal animation clip object
  const uint16_t skeletonBonesCount = rawClip.header.skeletonBonesCount;

  std::vector<BoneAnimationChannel> animationChannels;
  animationChannels.reserve(skeletonBonesCount);

  for (size_t channelIndex = 0; channelIndex < skeletonBonesCount; channelIndex++) {
    RawBoneAnimationChannel& channel = rawClip.bonesAnimationChannels[channelIndex];

    std::vector<BoneAnimationPositionFrame> positionFrames =
      MemoryUtils::createBinaryCompatibleVector<RawBonePositionFrame, BoneAnimationPositionFrame>(channel
        .positionFrames);

    std::vector<BoneAnimationOrientationFrame> orientationFrames =
      MemoryUtils::createBinaryCompatibleVector<RawBoneOrientationFrame, BoneAnimationOrientationFrame>(channel
        .orientationFrames);

    animationChannels.emplace_back(positionFrames, orientationFrames);
  }

  std::string animationName = rawClip.header.name;
  float animationDuration = rawClip.header.duration;
  float animationRate = rawClip.header.rate;

  allocateResource<AnimationClip>(resourceIndex, animationName,
    animationDuration,
    animationRate,
    animationChannels);
}

void SkeletalAnimationResourceManager::parseConfig(size_t resourceIndex, pugi::xml_node configNode)
{
  SkeletalAnimationResourceConfig* resourceConfig = createResourceConfig(resourceIndex);
  resourceConfig->resourcePath = configNode.attribute("source").as_string();

  if (!FileUtils::isFileExists(resourceConfig->resourcePath)) {
    THROW_EXCEPTION(EngineRuntimeException,
      fmt::format("Animation clip resource refer to not existing file", resourceConfig->resourcePath));
  }
}
