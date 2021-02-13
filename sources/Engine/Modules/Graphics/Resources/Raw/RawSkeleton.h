#pragma once

#include <vector>
#include <string>
#include "Modules/ResourceManagement/RawDataStructures.h"

constexpr uint16_t SKELETON_FORMAT_VERSION = 112;
constexpr size_t MAX_SKELETON_NAME_LENGTH = 64;
constexpr size_t MAX_BONE_NAME_LENGTH = 64;

struct RawSkeletonHeader {
  uint16_t formatVersion;

  char name[MAX_SKELETON_NAME_LENGTH];
  uint8_t bonesCount;
};

struct RawBone {
  char name[MAX_BONE_NAME_LENGTH];

  uint8_t parentId;
  RawMatrix4 inverseBindPoseMatrix;
};

struct RawSkeleton {
  RawSkeletonHeader header;

  std::vector<RawBone> bones;

  static RawSkeleton readFromFile(const std::string& path);
  static void writeToFile(const std::string& path, const RawSkeleton& rawSkeleton);

  static constexpr uint8_t ROOT_BONE_PARENT_ID = 255;
};
