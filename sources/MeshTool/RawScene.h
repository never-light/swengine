#pragma once

#include <Engine/Modules/Graphics/Resources/Raw/RawMesh.h>
#include <Engine/Modules/Graphics/Resources/Raw/RawSkeleton.h>

#include <Engine/Modules/Math/geometry.h>

struct RawMeshNode {
  RawMesh rawMesh;
  RawVector3 position{};
  RawQuaternion orientation{};
  char name[128]{};
};

struct RawScene {
  std::vector<RawMeshNode> meshesNodes;
};