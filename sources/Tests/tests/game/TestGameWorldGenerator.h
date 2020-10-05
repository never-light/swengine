#pragma once

#include <Engine/Modules/ECS/ECS.h>

class TestGameWorldGenerator {
 public:
  static std::shared_ptr<GameWorld> buildTestGameWorld();
};
