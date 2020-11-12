#pragma once

#include <Engine/Modules/Graphics/Resources/Raw/RawMesh.h>
#include <Engine/Modules/Graphics/Resources/Raw/RawSkeleton.h>
#include <Engine/Modules/Physics/Resources/Raw/RawMeshCollisionData.h>

#include <Engine/Modules/Math/geometry.h>

struct RawMeshNode {
  char name[128]{};
  RawMesh rawMesh;
  RawVector3 position{};
  RawVector3 scale{};
  RawQuaternion orientation{};

  bool collisionsResolutionEnabled{};
  RawMeshCollisionData collisionData{};

  bool isInteractive{};
};

struct RawScene {
  std::vector<RawMeshNode> meshesNodes;
};