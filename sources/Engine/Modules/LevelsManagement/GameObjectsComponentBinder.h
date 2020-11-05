#pragma once

#include "Modules/ECS/ECS.h"

class GameObjectsComponentBinder {
 public:
  GameObjectsComponentBinder() = default;
  virtual ~GameObjectsComponentBinder() = default;

  virtual void bindToObject(GameObject& gameObject) = 0;
};
