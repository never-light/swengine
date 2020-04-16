#pragma once

#include <vector>
#include <string>

#include "Modules/ResourceManagement/RawDataStructures.h"

constexpr uint16_t ANIMATION_FORMAT_VERSION = 112;
constexpr size_t MAX_ANIMATION_NAME_LENGTH = 64;

struct RawSkeletalAnimationHeader {
  uint16_t formatVersion;

  char name[MAX_ANIMATION_NAME_LENGTH];
  uint8_t skeletonBonesCount;

  float duration;
  float rate;
};

struct RawBonePositionFrame {
  float time;
  RawVector3 position;
};

struct RawBoneOrientationFrame {
  float time;
  RawQuaternion orientation;
};

struct RawBoneAnimationChannelHeader {
  uint16_t positionFramesCount;
  uint16_t orientationFramesCount;
};

struct RawBoneAnimationChannel {
  RawBoneAnimationChannelHeader header;

  std::vector<RawBonePositionFrame> positionFrames;
  std::vector<RawBoneOrientationFrame> orientationFrames;
};

struct RawSkeletalAnimationClip {
  RawSkeletalAnimationHeader header;
  std::vector<RawBoneAnimationChannel> bonesAnimationChannels;

  static RawSkeletalAnimationClip readFromFile(const std::string& path);
  static void writeToFile(const std::string& path, const RawSkeletalAnimationClip& rawClip);
};
