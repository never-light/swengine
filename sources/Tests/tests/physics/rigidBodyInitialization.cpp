#include <catch2/catch.hpp>

#include <unordered_set>
#include <Engine/Modules/Physics/PhysicsSystem.h>

#include <Engine/Modules/Math/MathUtils.h>
#include <Engine/Modules/Graphics/GraphicsSystem/TransformComponent.h>

std::shared_ptr<GameWorld> createPhysicsGameWorld() {
  auto gameWorld = GameWorld::createInstance();
  auto physicsSystem = std::make_shared<PhysicsSystem>();

  gameWorld->getGameSystemsGroup()->addGameSystem(physicsSystem);
  physicsSystem->setGravity({ 0.0f, -10.0f, 0.0f });

  return gameWorld;
}

TEST_CASE("rigid_body_creation", "[physics]")
{
  std::shared_ptr<GameWorld> gameWorld = createPhysicsGameWorld();
  std::shared_ptr<GameObject> rigidBody = gameWorld->createGameObject();

  auto& transformComponent = rigidBody->addComponent<TransformComponent>();
  transformComponent.getTransform().setPosition(0.0f, 10.0f, 0.0f);

  rigidBody->addComponent<RigidBodyComponent>(RigidBodyComponent(1.0f,
    CollisionShapesFactory::createSphere(10.0f),
    transformComponent.getTransformPtr()));

  REQUIRE(MathUtils::isEqual(rigidBody->getComponent<RigidBodyComponent>().getMass(), 1.0f));
}

TEST_CASE("rigid_body_gravity_affection", "[physics]")
{
  std::shared_ptr<GameWorld> gameWorld = createPhysicsGameWorld();
  std::shared_ptr<GameObject> rigidBody = gameWorld->createGameObject();

  auto& transform = rigidBody->addComponent<TransformComponent>().getTransform();
  transform.setPosition(0.0f, 10.0f, 0.0f);

  auto& rigidBodyComponent = rigidBody->addComponent<RigidBodyComponent>(RigidBodyComponent(1.0f,
    CollisionShapesFactory::createSphere(1.0f),
    rigidBody->getComponent<TransformComponent>().getTransformPtr()));

  gameWorld->update(0.5f);
  gameWorld->update(0.5f);
  gameWorld->update(0.4142f);

  glm::vec3 vel = rigidBodyComponent.getLinearVelocity();

  // TODO: probably, this precision is low. Check its reason and try to fix
  REQUIRE(MathUtils::isEqual(transform.getPosition(), { 0.0f, 0.0f, 0.0f }, 0.25f));
  REQUIRE(MathUtils::isEqual(rigidBodyComponent.getLinearVelocity(), { 0.0f, -14.1421f, 0.0f }, 0.25f));
}
