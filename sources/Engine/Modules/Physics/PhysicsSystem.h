#pragma once

#include "Modules/ECS/ECS.h"

class PhysicsSystem : public GameSystem {
 public:
  PhysicsSystem();
  ~PhysicsSystem() override;

  void configure(GameWorld* gameWorld) override;
  void unconfigure(GameWorld* gameWorld) override;

  void update(GameWorld* gameWorld, float delta) override;
};
