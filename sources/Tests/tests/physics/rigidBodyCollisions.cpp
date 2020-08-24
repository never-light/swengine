#include <catch2/catch.hpp>

#include <unordered_set>
#include <Engine/Modules/Physics/PhysicsSystem.h>

#include <Engine/Modules/Math/MathUtils.h>
#include <Engine/Modules/Graphics/GraphicsSystem/TransformComponent.h>

std::shared_ptr<GameWorld> createCollisionsPhysicsGameWorld() {
  auto gameWorld = GameWorld::createInstance();
  auto physicsSystem = std::make_shared<PhysicsSystem>();

  gameWorld->getGameSystemsGroup()->addGameSystem(physicsSystem);
  physicsSystem->setGravity({ 0.0f, -10.0f, 0.0f });

  return gameWorld;
}

TEST_CASE("rigid_bodies_collision", "[physics]")
{
  std::shared_ptr<GameWorld> gameWorld = createCollisionsPhysicsGameWorld();

  std::shared_ptr<GameObject> fallingBody = gameWorld->createGameObject();

  auto& fallingTransformComponent = fallingBody->addComponent<TransformComponent>();
  fallingTransformComponent.getTransform().setPosition(0.0f, 10.0f, 0.0f);

  fallingBody->addComponent<RigidBodyComponent>(RigidBodyComponent(1.0f,
    CollisionShapesFactory::createSphere(1.0f),
    fallingTransformComponent.getTransformPtr()));

  std::shared_ptr<GameObject> floorBody = gameWorld->createGameObject();

  auto& floorTransformComponent = floorBody->addComponent<TransformComponent>();
  floorTransformComponent.getTransform().setPosition(0.0f, 0.0f, 0.0f);

  floorBody->addComponent<RigidBodyComponent>(RigidBodyComponent(0.0f,
    CollisionShapesFactory::createBox(glm::vec3(0.5f, 0.5f, 0.5f)),
    floorTransformComponent.getTransformPtr()));

  for (int timeStepIndex = 0; timeStepIndex < 60; timeStepIndex++) {
    gameWorld->update(5.0f / 60.0f);
  }

  REQUIRE(MathUtils::isEqual(fallingTransformComponent.getTransform().getPosition(),
    { 0.0f, 1.5f, 0.0f }, 0.25f));
}
