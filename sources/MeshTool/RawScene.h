#pragma once

#include <optional>

#include <Engine/Modules/Graphics/Resources/Raw/RawMesh.h>
#include <Engine/Modules/Graphics/Resources/Raw/RawSkeleton.h>
#include <Engine/Modules/Graphics/Resources/Raw/RawSkeletalAnimationClip.h>
#include <Engine/Modules/Physics/Resources/Raw/RawMeshCollisionData.h>

#include <Engine/Modules/Math/geometry.h>

struct RawTextureTransformInfo {
  RawVector2 scale = { 1.0f, 1.0f };
  RawVector2 offset = { 0.0f, 0.0f };
  float rotation{};
};

struct RawTextureInfo {
  std::string textureTmpPath;
  std::string textureBaseName;

  std::optional<RawTextureTransformInfo> textureTransform;
};

struct RawMaterial {
  char name[128]{};

  RawVector4 baseColorFactor{ 1.0f, 1.0f, 1.0f, 1.0f };
  std::optional<RawTextureInfo> baseColorTextureInfo;
};

struct RawMeshNode {
  char name[128]{};
  RawMesh rawMesh;
  RawVector3 position{};
  RawVector3 scale{};
  RawQuaternion orientation{};

  bool collisionsResolutionEnabled{};
  RawMeshCollisionData collisionData{};

  std::vector<std::optional<RawMaterial>> materials;

  bool isInteractive{};

  size_t skeletonIndex = NONE_SKELETON;

  static constexpr size_t NONE_SKELETON = std::numeric_limits<size_t>::max();
};

struct RawSkeletonDescription {
  RawSkeleton skeleton;
  std::vector<size_t> animationClips;
};

struct RawScene {
  std::vector<RawMeshNode> meshesNodes;
  std::vector<RawSkeletonDescription> skeletons;
  std::vector<RawSkeletalAnimationClip> animationClips;
};
