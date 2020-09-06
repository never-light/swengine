#include <catch2/catch.hpp>

#include <unordered_set>
#include <Engine/Modules/Physics/PhysicsSystem.h>

#include <Engine/Modules/Math/MathUtils.h>
#include <Engine/Modules/Graphics/GraphicsSystem/TransformComponent.h>

std::shared_ptr<GameWorld> createCollisionsPhysicsGameWorld()
{
  auto gameWorld = GameWorld::createInstance();
  auto physicsSystem = std::make_shared<PhysicsSystem>();

  gameWorld->getGameSystemsGroup()->addGameSystem(physicsSystem);
  physicsSystem->setGravity({0.0f, -10.0f, 0.0f});

  return gameWorld;
}

std::shared_ptr<GameWorld> createCollisionsFloorSimulation()
{
  auto gameWorld = GameWorld::createInstance();
  auto physicsSystem = std::make_shared<PhysicsSystem>();

  gameWorld->getGameSystemsGroup()->addGameSystem(physicsSystem);
  physicsSystem->setGravity({0.0f, -10.0f, 0.0f});

  std::shared_ptr<GameObject> fallingBody = gameWorld->createGameObject();

  auto& fallingTransformComponent = fallingBody->addComponent<TransformComponent>();
  fallingTransformComponent.getTransform().setPosition(0.0f, 10.0f, 0.0f);

  fallingBody->addComponent<RigidBodyComponent>(RigidBodyComponent(1.0f,
    CollisionShapesFactory::createSphere(1.0f)));

  std::shared_ptr<GameObject> floorBody = gameWorld->createGameObject();

  auto& floorTransformComponent = floorBody->addComponent<TransformComponent>();
  floorTransformComponent.getTransform().setPosition(0.0f, 0.0f, 0.0f);

  floorBody->addComponent<RigidBodyComponent>(RigidBodyComponent(0.0f,
    CollisionShapesFactory::createBox(glm::vec3(0.5f, 0.5f, 0.5f))));

  return gameWorld;
}

TEST_CASE("rigid_bodies_collision", "[physics]")
{
  std::shared_ptr<GameWorld> gameWorld = createCollisionsFloorSimulation();

  const auto& fallingTransformComponent = gameWorld->findGameObject(1)->getComponent<TransformComponent>();

  for (int timeStepIndex = 0; timeStepIndex < 60; timeStepIndex++) {
    gameWorld->update(5.0f / 60.0f);
  }

  REQUIRE(MathUtils::isEqual(fallingTransformComponent.getTransform().getPosition(),
    {0.0f, 1.5f, 0.0f}, 0.25f));
}

TEST_CASE("rigid_bodies_collision_callback", "[physics]")
{
  std::shared_ptr<GameWorld> gameWorld = createCollisionsFloorSimulation();

  bool collisionDetected1 = false;
  bool collisionVerified1 = true;

  bool collisionDetected2 = false;
  bool collisionVerified2 = true;

  gameWorld->findGameObject(1)->getComponent<RigidBodyComponent>()
    .setCollisionCallback([&collisionDetected1, &collisionVerified1, gameWorld](CollisionInfo& collision) {
      collisionDetected1 = true;

      collisionVerified1 = collisionVerified1 &&
        collision.selfGameObject.getId() == 1 && collision.gameObject.getId() == 2;

      return RigidBodyCollisionProcessingStatus::ObservedOnly;
    });

  gameWorld->findGameObject(2)->getComponent<RigidBodyComponent>()
    .setCollisionCallback([&collisionDetected2, &collisionVerified2, gameWorld](CollisionInfo& collision) {
      collisionDetected2 = true;

      collisionVerified2 = collisionVerified2 &&
        collision.selfGameObject.getId() == 2 && collision.gameObject.getId() == 1;

      return RigidBodyCollisionProcessingStatus::ObservedOnly;
    });

  for (int timeStepIndex = 0; timeStepIndex < 60; timeStepIndex++) {
    gameWorld->update(5.0f / 60.0f);
  }

  REQUIRE(collisionDetected1);
  REQUIRE(collisionVerified1);

  REQUIRE(collisionDetected2);
  REQUIRE(collisionVerified2);
}
