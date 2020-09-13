#pragma once

class GameObject;

struct CollisionInfo {
  GameObject selfGameObject;
  GameObject gameObject;
};

enum class RigidBodyCollisionProcessingStatus {
  Processed,
  ObservedOnly,
  Skipped
};

using CollisionCallback = std::function<RigidBodyCollisionProcessingStatus(CollisionInfo&)>;
