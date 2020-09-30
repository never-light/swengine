#include <catch2/catch.hpp>

#include <set>
#include <Engine/Modules/ECS/ECS.h>
#include <Engine/Modules/Graphics/GraphicsSystem/TransformComponent.h>

bool operator<(const GameObject& lhs, const GameObject& rhs)
{
  if (!lhs.isAlive()) {
    return false;
  }
  else if (!rhs.isAlive()) {
    return true;
  }
  else {
    return lhs.getId() < rhs.getId();
  }
}

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

  void update(float delta) override
  {
    ARG_UNUSED(delta);

    for (auto object : getGameWorld()->allWith<TestSpeedComponent>()) {
      object.getComponent<TestSpeedComponent>()->speed += 5;
    }

    for (auto object : getGameWorld()->allWith<TestMeshComponent>()) {
      object.getComponent<TestMeshComponent>()->isDrawn = false;
    }
  }

  void render() override
  {
    for (auto object : getGameWorld()->allWith<TestMeshComponent>()) {
      object.getComponent<TestMeshComponent>()->isDrawn = true;
    }
  }
};

class TestEventsListener : public EventsListener<TestEvent> {
 public:
  TestEventsListener() = default;
  ~TestEventsListener() override = default;

  EventProcessStatus receiveEvent(const TestEvent& event) override
  {
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

  GameObject firstObject = gameWorld->createGameObject();

  firstObject.addComponent<TestHealthComponent>();
  firstObject.addComponent<TestSpeedComponent>(TestSpeedComponent{10});

  REQUIRE(firstObject.hasComponent<TestHealthComponent>());
  REQUIRE(firstObject.getComponent<TestHealthComponent>()->health == 0);

  REQUIRE(firstObject.hasComponent<TestSpeedComponent>());
  REQUIRE(firstObject.getComponent<TestSpeedComponent>()->speed == 10);

  // Reference to the first object could become invalid here
  GameObject secondObject = gameWorld->createGameObject();
  secondObject.addComponent<TestSpeedComponent>(TestSpeedComponent{20});

  REQUIRE(secondObject.hasComponent<TestSpeedComponent>());
  REQUIRE(secondObject.getComponent<TestSpeedComponent>()->speed == 20);
}

TEST_CASE("game_objects_components_management", "[ecs]")
{
  std::shared_ptr<GameWorld> gameWorld = GameWorld::createInstance();
  GameObject object = gameWorld->createGameObject();

  object.addComponent<TestHealthComponent>(TestHealthComponent{10});
  object.addComponent<TestSpeedComponent>(TestSpeedComponent{15});

  REQUIRE(object.hasComponent<TestHealthComponent>());
  REQUIRE(object.getComponent<TestHealthComponent>()->health == 10);

  REQUIRE(object.hasComponent<TestSpeedComponent>());
  REQUIRE(object.getComponent<TestSpeedComponent>()->speed == 15);

  object.removeComponent<TestHealthComponent>();

  REQUIRE_FALSE(object.hasComponent<TestHealthComponent>());
  REQUIRE(object.hasComponent<TestSpeedComponent>());

  object.addComponent<TestHealthComponent>(TestHealthComponent{50});

  REQUIRE(object.hasComponent<TestHealthComponent>());
  REQUIRE(object.getComponent<TestHealthComponent>()->health == 50);
}

TEST_CASE("game_objects_management", "[ecs]")
{
  std::shared_ptr<GameWorld> gameWorld = GameWorld::createInstance();

  GameObject firstObject = gameWorld->createGameObject();
  firstObject.addComponent<TestHealthComponent>();

  GameObject secondObject = gameWorld->createGameObject();
  secondObject.addComponent<TestHealthComponent>();

  gameWorld->removeGameObject(secondObject);

  GameObject thirdObject = gameWorld->createGameObject();
  thirdObject.addComponent<TestHealthComponent>();

  REQUIRE_FALSE(secondObject.isAlive());

  std::set<GameObject> objectsFoundByForEach;

  gameWorld->forEach([&objectsFoundByForEach](GameObject& obj) {
    objectsFoundByForEach.insert(obj);
  });

  REQUIRE(objectsFoundByForEach.contains(firstObject));
  REQUIRE_FALSE(objectsFoundByForEach.contains(secondObject));
  REQUIRE(objectsFoundByForEach.contains(thirdObject));

  std::set<GameObject> objectsFoundByForAll;

  for (auto obj : gameWorld->all()) {
    objectsFoundByForAll.insert(obj);
  }

  REQUIRE(objectsFoundByForAll.contains(firstObject));
  REQUIRE_FALSE(objectsFoundByForAll.contains(secondObject));
  REQUIRE(objectsFoundByForAll.contains(thirdObject));

  std::set<GameObject> objectsFoundByForAllWith;

  for (auto obj : gameWorld->allWith<TestHealthComponent>()) {
    objectsFoundByForAllWith.insert(obj);
  }

  REQUIRE(objectsFoundByForAllWith.contains(firstObject));
  REQUIRE_FALSE(objectsFoundByForAllWith.contains(secondObject));
  REQUIRE(objectsFoundByForAllWith.contains(thirdObject));
}

TEST_CASE("game_systems_usage", "[ecs]")
{
  std::shared_ptr<GameWorld> gameWorld = GameWorld::createInstance();
  auto defaultGameSystemsGroup = gameWorld->getGameSystemsGroup();

  std::shared_ptr<TestGameSystem> testSystem = std::make_shared<TestGameSystem>();
  defaultGameSystemsGroup->addGameSystem(testSystem);

  REQUIRE(defaultGameSystemsGroup->getGameSystem<TestGameSystem>() == testSystem);

  GameObject gameObject = gameWorld->createGameObject();
  gameObject.addComponent<TestSpeedComponent>(TestSpeedComponent{10});
  gameObject.addComponent<TestMeshComponent>(TestMeshComponent{false});

  gameWorld->update(1.0f);

  REQUIRE(gameObject.getComponent<TestSpeedComponent>()->speed == 15);

  gameWorld->render();

  REQUIRE(gameObject.getComponent<TestSpeedComponent>()->speed == 15);
  REQUIRE(gameObject.getComponent<TestMeshComponent>()->isDrawn);

  gameWorld->update(1.0f);

  REQUIRE(gameObject.getComponent<TestSpeedComponent>()->speed == 20);
  REQUIRE_FALSE(gameObject.getComponent<TestMeshComponent>()->isDrawn);

  defaultGameSystemsGroup->removeGameSystem(testSystem);

  // TODO: forbid to get nonexistent game system, add hasGameSystem method
  REQUIRE(defaultGameSystemsGroup->getGameSystem<TestGameSystem>() == nullptr);

  gameWorld->update(1.0f);
  gameWorld->render();

  REQUIRE(gameObject.getComponent<TestSpeedComponent>()->speed == 20);
}

TEST_CASE("game_events_handling", "[ecs]")
{
  std::shared_ptr<GameWorld> gameWorld = GameWorld::createInstance();
  std::shared_ptr<TestEventsListener> listener = std::make_shared<TestEventsListener>();

  gameWorld->subscribeEventsListener(listener.get());

  gameWorld->emitEvent(TestEvent{10});
  REQUIRE(listener->getLastMessageCode() == 10);

  gameWorld->unsubscribeEventsListener(listener.get());

  gameWorld->emitEvent(TestEvent{20});
  REQUIRE(listener->getLastMessageCode() == 10);
}

TEST_CASE("game_objects_iteration", "[ecs]")
{
    std::shared_ptr<GameWorld> gameWorld = GameWorld::createInstance();

    std::vector<GameObject> gameObjects;

    for(size_t i = 0; i < 100; i++) {
        GameObject object = gameWorld->createGameObject();
        object.addComponent<TransformComponent>();
        gameObjects.push_back(object);
    }

    for(size_t i = 50; i < 60; i++) {
        gameWorld->removeGameObject(gameObjects[i]);
    }

    for(size_t i = 0; i < 100; i++) {
        GameObject object = gameWorld->createGameObject();
        object.addComponent<TransformComponent>();
        gameObjects.push_back(object);
    }

    size_t iterations = 190;
    gameWorld->forEach([&iterations](const GameObject& object) {
        REQUIRE(object.isAlive());
        iterations--;
    });
    REQUIRE(!iterations);

    iterations = 190;
    GameObject foundObj = gameWorld->findGameObject([&iterations](const GameObject& object) {
        REQUIRE(object.isAlive());
        iterations--;
        return false;
    });
    REQUIRE(iterations == 0);

    iterations = 190;
    auto gameObjectsSequentialView = gameWorld->all();
    auto gameObjectsSequentialIterator = gameObjectsSequentialView.begin();

    while(!gameObjectsSequentialIterator.isEnd()) {
        REQUIRE(gameObjectsSequentialIterator.getGameObject().isAlive());
        ++gameObjectsSequentialIterator;
        iterations--;
    }

    REQUIRE(!iterations);

    iterations = 190;
    auto gameObjectsComponentsView = gameWorld->allWith<TransformComponent>();
    auto gameObjectsComponentsIterator = gameObjectsComponentsView.begin();

    while(!gameObjectsComponentsIterator.isEnd()) {
        REQUIRE((*gameObjectsComponentsIterator).isAlive());
        ++gameObjectsComponentsIterator;
        iterations--;
    }

    REQUIRE(!iterations);
}


TEST_CASE("game_objects_iteration_with_delete", "[ecs]")
{
    std::shared_ptr<GameWorld> gameWorld = GameWorld::createInstance();

    std::vector<GameObject> gameObjects;

    for(size_t i = 0; i < 100; i++) {
        GameObject object = gameWorld->createGameObject();
        object.addComponent<TransformComponent>();
        gameObjects.push_back(object);
    }

    size_t iterationIndex = 0;
    size_t iterations = 90;
    gameWorld->forEach([&iterationIndex, &gameWorld, &gameObjects](const GameObject& object) {
        if(iterationIndex == 5) {
            for(size_t i = iterationIndex + 1; i < iterationIndex + 11; i++) {
                gameWorld->removeGameObject(gameObjects[i]);
            }
        }
        REQUIRE(object.isAlive());
        iterationIndex++;
    });
    REQUIRE(iterationIndex == iterations);

    iterationIndex = 0;
    iterations-=10;
    GameObject foundObj = gameWorld->findGameObject([&iterationIndex, &gameWorld, &gameObjects]
            (const GameObject& object) {
        if(iterationIndex == 5) {
            for(size_t i = iterationIndex + 11; i < iterationIndex + 21; i++) {
                gameWorld->removeGameObject(gameObjects[i]);
            }
        }
        REQUIRE(object.isAlive());
        iterationIndex++;
        return false;
    });
    REQUIRE(iterationIndex == iterations);

    iterationIndex = 0;
    iterations -=10;
    auto gameObjectsSequentialView = gameWorld->all();

    for(auto it = gameObjectsSequentialView.begin(); it != gameObjectsSequentialView.end(); ++it, iterationIndex++) {
        if(iterationIndex == 5) {
            auto nextIt = ++it;
            for (size_t removeCounter = 0; removeCounter < 10; removeCounter++) {
                GameObject objectToRemove = *nextIt;
                gameWorld->removeGameObject(objectToRemove);
                ++nextIt;
            }
        }
        REQUIRE(it.getGameObject().isAlive());

    }

    REQUIRE(iterationIndex == iterations);

    iterationIndex = 0;
    iterations -=10;
    auto gameObjectsComponentsView = gameWorld->allWith<TransformComponent>();

    for(auto it = gameObjectsComponentsView.begin(); it != gameObjectsComponentsView.end(); ++it, iterationIndex++) {
        if(iterationIndex == 5) {
            auto nextIt = ++it;
            for (size_t removeCounter = 0; removeCounter < 10; removeCounter++) {
                GameObject objectToRemove = *nextIt;
                gameWorld->removeGameObject(objectToRemove);
                ++nextIt;
            }
        }
        REQUIRE((*it).isAlive());

    }

    REQUIRE(iterationIndex == iterations);
}