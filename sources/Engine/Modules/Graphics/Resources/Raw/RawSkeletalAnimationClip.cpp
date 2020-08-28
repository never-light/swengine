#include "precompiled.h"

#pragma hdrstop

#include "RawSkeletalAnimationClip.h"
#include "Exceptions/exceptions.h"

#include "Utility/files.h"

RawSkeletalAnimationClip RawSkeletalAnimationClip::readFromFile(const std::string& path)
{
  if (!FileUtils::isFileExists(path)) {
    THROW_EXCEPTION(EngineRuntimeException, "Trying to read not existing animation clip file " + path);
  }

  std::ifstream clipFile(path, std::ios::binary);

  RawSkeletalAnimationClip rawClip;

  clipFile.read(reinterpret_cast<char*>(&rawClip.header), sizeof(rawClip.header));

  if (rawClip.header.formatVersion != ANIMATION_FORMAT_VERSION) {
    THROW_EXCEPTION(EngineRuntimeException, "Trying to load animation clip with incompatible format version: " +
      path);
  }

  if (rawClip.header.skeletonBonesCount == 0) {
    THROW_EXCEPTION(EngineRuntimeException, "Trying to load animation clip with zero bones count: " +
      path);
  }

  const uint16_t skeletonBonesCount = rawClip.header.skeletonBonesCount;
  rawClip.bonesAnimationChannels.resize(skeletonBonesCount);

  for (size_t channelIndex = 0; channelIndex < skeletonBonesCount; channelIndex++) {
    RawBoneAnimationChannel& channel = rawClip.bonesAnimationChannels[channelIndex];

    clipFile.read(reinterpret_cast<char*>(&channel.header), sizeof(channel.header));

    if (channel.header.positionFramesCount > 0) {
      channel.positionFrames.resize(channel.header.positionFramesCount);

      clipFile.read(reinterpret_cast<char*>(channel.positionFrames.data()),
        sizeof(*channel.positionFrames.begin()) * channel.header.positionFramesCount);
    }

    if (channel.header.orientationFramesCount > 0) {
      channel.orientationFrames.resize(channel.header.orientationFramesCount);

      clipFile.read(reinterpret_cast<char*>(channel.orientationFrames.data()),
        sizeof(*channel.orientationFrames.begin()) * channel.header.orientationFramesCount);
    }
  }

  clipFile.close();

  return rawClip;
}

void RawSkeletalAnimationClip::writeToFile(const std::string& path, const RawSkeletalAnimationClip& rawClip)
{
  SW_ASSERT(rawClip.header.formatVersion == ANIMATION_FORMAT_VERSION);

  SW_ASSERT(rawClip.header.skeletonBonesCount > 0 && rawClip.header.skeletonBonesCount ==
    rawClip.bonesAnimationChannels.size());

  std::ofstream out(path, std::ios::binary);

  const uint16_t affectedbonesCount = rawClip.header.skeletonBonesCount;

  out.write(reinterpret_cast<const char*>(&rawClip.header), sizeof(rawClip.header));

  for (size_t channelIndex = 0; channelIndex < affectedbonesCount; channelIndex++) {
    const RawBoneAnimationChannel& channel = rawClip.bonesAnimationChannels[channelIndex];

    out.write(reinterpret_cast<const char*>(&channel.header), sizeof(channel.header));

    if (channel.positionFrames.size() > 0) {
      out.write(reinterpret_cast<const char*>(channel.positionFrames.data()),
        sizeof(*channel.positionFrames.begin()) * channel.header.positionFramesCount);
    }

    if (channel.orientationFrames.size() > 0) {
      out.write(reinterpret_cast<const char*>(channel.orientationFrames.data()),
        sizeof(*channel.orientationFrames.begin()) * channel.header.orientationFramesCount);
    }
  }

  out.close();
}
