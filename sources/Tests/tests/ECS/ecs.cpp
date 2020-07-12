#include <catch2/catch.hpp>

#include <unordered_set>
#include <Engine/Modules/ECS/ECS.h>

struct TestEvent {
  int messageCode = 0;
};

struct TestHealthComponent {
  int health = 0;
};

struct TestSpeedComponent {
  int speed = 0;
};

struct TestMeshComponent {
  bool isDrawn = false;
};

class TestGameSystem : public GameSystem {
 public:
  TestGameSystem() = default;
  ~TestGameSystem() override = default;

  void update(GameWorld* gameWorld, float delta) override
  {
    ARG_UNUSED(delta);

    for (auto object : gameWorld->allWith<TestSpeedComponent>()) {
      object->getComponent<TestSpeedComponent>()->speed += 5;
    }

    for (auto object : gameWorld->allWith<TestMeshComponent>()) {
      object->getComponent<TestMeshComponent>()->isDrawn = false;
    }
  }

  void render(GameWorld* gameWorld) override
  {
    for (auto object : gameWorld->allWith<TestMeshComponent>()) {
      object->getComponent<TestMeshComponent>()->isDrawn = true;
    }
  }
};

class TestEventsListener : public EventsListener<TestEvent> {
 public:
  TestEventsListener() = default;
  ~TestEventsListener() override = default;

  EventProcessStatus receiveEvent(GameWorld* gameWorld, const TestEvent& event) override
  {
    ARG_UNUSED(gameWorld);

    m_lastMessageCode = event.messageCode;
    return EventProcessStatus::Processed;
  }

  [[nodiscard]] int getLastMessageCode() const
  {
    return m_lastMessageCode;
  }

 private:
  int m_lastMessageCode = 0;
};

TEST_CASE("game_objects_creation", "[ecs]")
{
  std::shared_ptr<GameWorld> gameWorld = GameWorld::createInstance();

  GameObject* firstObject = gameWorld->createGameObject();

  firstObject->addComponent<TestHealthComponent>();
  firstObject->addComponent<TestSpeedComponent>(TestSpeedComponent{10});

  REQUIRE(firstObject->hasComponent<TestHealthComponent>());
  REQUIRE(firstObject->getComponent<TestHealthComponent>()->health == 0);

  REQUIRE(firstObject->hasComponent<TestSpeedComponent>());
  REQUIRE(firstObject->getComponent<TestSpeedComponent>()->speed == 10);

  GameObject* secondObject = gameWorld->createGameObject();
  secondObject->addComponent<TestSpeedComponent>(TestSpeedComponent{20});

  REQUIRE(secondObject->hasComponent<TestSpeedComponent>());
  REQUIRE(secondObject->getComponent<TestSpeedComponent>()->speed == 20);

  REQUIRE(gameWorld->findGameObject(firstObject->getId()) == firstObject);
  REQUIRE(gameWorld->findGameObject(secondObject->getId()) == secondObject);
}

TEST_CASE("game_objects_components_management", "[ecs]")
{
  std::shared_ptr<GameWorld> gameWorld = GameWorld::createInstance();
  GameObject* object = gameWorld->createGameObject();

  object->addComponent<TestHealthComponent>(TestHealthComponent{10});
  object->addComponent<TestSpeedComponent>(TestSpeedComponent{15});

  REQUIRE(object->hasComponent<TestHealthComponent>());
  REQUIRE(object->getComponent<TestHealthComponent>()->health == 10);

  REQUIRE(object->hasComponent<TestSpeedComponent>());
  REQUIRE(object->getComponent<TestSpeedComponent>()->speed == 15);

  object->removeComponent<TestHealthComponent>();

  REQUIRE_FALSE(object->hasComponent<TestHealthComponent>());
  REQUIRE(object->hasComponent<TestSpeedComponent>());

  object->addComponent<TestHealthComponent>(TestHealthComponent{50});

  REQUIRE(object->hasComponent<TestHealthComponent>());
  REQUIRE(object->getComponent<TestHealthComponent>()->health == 50);
}

TEST_CASE("game_objects_management", "[ecs]")
{
  std::shared_ptr<GameWorld> gameWorld = GameWorld::createInstance();

  GameObject* firstObject = gameWorld->createGameObject();
  GameObjectId firstObjectId = firstObject->getId();

  firstObject->addComponent<TestHealthComponent>();

  GameObject* secondObject = gameWorld->createGameObject();
  GameObjectId secondObjectId = secondObject->getId();

  secondObject->addComponent<TestHealthComponent>();

  gameWorld->removeGameObject(secondObject);

  GameObject* thirdObject = gameWorld->createGameObject();
  GameObjectId thirdObjectId = thirdObject->getId();

  thirdObject->addComponent<TestHealthComponent>();

  REQUIRE(gameWorld->findGameObject(secondObjectId) == nullptr);
  REQUIRE(gameWorld->findGameObject([secondObjectId](GameObject* obj) {
    return obj->getId() == secondObjectId;
  }) == nullptr);

  std::unordered_set<GameObjectId> objectsFoundByForEach;

  gameWorld->forEach([&objectsFoundByForEach](GameObject* obj) {
    objectsFoundByForEach.insert(obj->getId());
  });

  REQUIRE(objectsFoundByForEach.contains(firstObjectId));
  REQUIRE_FALSE(objectsFoundByForEach.contains(secondObjectId));
  REQUIRE(objectsFoundByForEach.contains(thirdObjectId));

  std::unordered_set<GameObjectId> objectsFoundByForAll;

  for (auto obj : gameWorld->all()) {
    objectsFoundByForAll.insert(obj->getId());
  }

  REQUIRE(objectsFoundByForAll.contains(firstObjectId));
  REQUIRE_FALSE(objectsFoundByForAll.contains(secondObjectId));
  REQUIRE(objectsFoundByForAll.contains(thirdObjectId));

  std::unordered_set<GameObjectId> objectsFoundByForAllWith;

  for (auto obj : gameWorld->allWith<TestHealthComponent>()) {
    objectsFoundByForAllWith.insert(obj->getId());
  }

  REQUIRE(objectsFoundByForAllWith.contains(firstObjectId));
  REQUIRE_FALSE(objectsFoundByForAllWith.contains(secondObjectId));
  REQUIRE(objectsFoundByForAllWith.contains(thirdObjectId));
}

TEST_CASE("game_systems_usage", "[ecs]")
{
  std::shared_ptr<GameWorld> gameWorld = GameWorld::createInstance();
  auto defaultGameSystemsGroup = gameWorld->getGameSystemsGroup();

  std::shared_ptr<TestGameSystem> testSystem = std::make_shared<TestGameSystem>();
  defaultGameSystemsGroup->addGameSystem(testSystem);

  REQUIRE(defaultGameSystemsGroup->getGameSystem<TestGameSystem>() == testSystem);

  GameObject* gameObject = gameWorld->createGameObject();
  gameObject->addComponent<TestSpeedComponent>(TestSpeedComponent{10});
  gameObject->addComponent<TestMeshComponent>(TestMeshComponent{false});

  gameWorld->update(1.0f);

  REQUIRE(gameObject->getComponent<TestSpeedComponent>()->speed == 15);

  gameWorld->render();

  REQUIRE(gameObject->getComponent<TestSpeedComponent>()->speed == 15);
  REQUIRE(gameObject->getComponent<TestMeshComponent>()->isDrawn);

  gameWorld->update(1.0f);

  REQUIRE(gameObject->getComponent<TestSpeedComponent>()->speed == 20);
  REQUIRE_FALSE(gameObject->getComponent<TestMeshComponent>()->isDrawn);

  defaultGameSystemsGroup->removeGameSystem(testSystem);

  // TODO: forbid to get nonexistent game system, add hasGameSystem method
  REQUIRE(defaultGameSystemsGroup->getGameSystem<TestGameSystem>() == nullptr);

  gameWorld->update(1.0f);
  gameWorld->render();

  REQUIRE(gameObject->getComponent<TestSpeedComponent>()->speed == 20);
}

TEST_CASE("game_events_handling", "[ecs]")
{
  std::shared_ptr<GameWorld> gameWorld = GameWorld::createInstance();
  std::shared_ptr<TestEventsListener> listener = std::make_shared<TestEventsListener>();

  gameWorld->subscribeEventsListener(listener.get());

  gameWorld->emitEvent(TestEvent{ 10 });
  REQUIRE(listener->getLastMessageCode() == 10);

  gameWorld->unsubscribeEventsListener(listener.get());

  gameWorld->emitEvent(TestEvent{ 20 });
  REQUIRE(listener->getLastMessageCode() == 10);
}
