#include "precompiled.h"

#pragma hdrstop

#include "GameWorld.h"
#include <algorithm>

GameWorld::GameWorld()
{
}

GameWorld::~GameWorld()
{
  m_gameSystemsGroup->unconfigure(this);

  for (auto object : m_gameObjects) {
    removeGameObject(object);
  }

  removeDestroyedObjects();

  SW_ASSERT(m_gameObjects.empty());
}

void GameWorld::update(float delta)
{
  m_gameSystemsGroup->update(this, delta);

  removeDestroyedObjects();
}

void GameWorld::render()
{
  m_gameSystemsGroup->render(this);
}

void GameWorld::beforeRender()
{
  m_gameSystemsGroup->beforeRender(this);
}

void GameWorld::afterRender()
{
  m_gameSystemsGroup->afterRender(this);
}

void GameWorld::setGameSystemsGroup(std::unique_ptr<GameSystemsGroup> group)
{
  m_gameSystemsGroup = std::move(group);
  m_gameSystemsGroup->configure(this);
}

GameSystemsGroup* GameWorld::getGameSystemsGroup() const
{
  return m_gameSystemsGroup.get();
}

std::shared_ptr<GameObject> GameWorld::createGameObject()
{
  m_lastGameObjectId++;
  auto gameObject = std::make_shared<GameObject>(m_lastGameObjectId, this);

  m_gameObjects.push_back(gameObject);

  return gameObject;
}

std::shared_ptr<GameObject> GameWorld::findGameObject(const std::function<bool(const GameObject&)> predicate) const
{
  for (auto object : m_gameObjects) {
    if (object->isAlive() && predicate(*object)) {
      return object;
    }
  }

  return nullptr;
}

GameObject& GameWorld::getGameObjectByIndex(size_t index) const
{
  return *m_gameObjects[index];
}

bool GameWorld::hasGameObjectWithIndex(size_t index) const
{
  return index < m_gameObjects.size();
}

size_t GameWorld::getGameObjectsCount() const
{
  return m_gameObjects.size();
}

void GameWorld::removeGameObject(std::shared_ptr<GameObject> gameObject)
{
  gameObject->m_isDestroyed = true;
}

void GameWorld::forEach(std::function<void(GameObject&)> action)
{
  for (GameObject* gameObject : this->all()) {
    action(*gameObject);
  }
}

GameObjectsSequentialView GameWorld::all()
{
  GameObjectsSequentialIterator begin(this, 0, false);
  GameObjectsSequentialIterator end(this, m_gameObjects.size(), true);

  return GameObjectsSequentialView(begin, end);
}

void GameWorld::cancelEventsListening(BaseEventsListener* listener)
{
  for (auto& it : m_eventsListeners) {
    std::vector<BaseEventsListener*>& listeners = it.second;
    listeners.erase(std::remove(listeners.begin(), listeners.end(), listener), listeners.end());
  }
}

void GameWorld::removeDestroyedObjects()
{
  std::for_each(m_gameObjects.begin(), m_gameObjects.end(), [](std::shared_ptr<GameObject>& obj) {
    if (!obj->isAlive()) {
      obj = nullptr;
    }
  });

  m_gameObjects.erase(std::remove(m_gameObjects.begin(), m_gameObjects.end(), nullptr),
    m_gameObjects.end());
}

std::shared_ptr<GameObject> GameWorld::findGameObject(GameObjectId id) const
{
  for (auto object : m_gameObjects) {
    if (object->isAlive() && object->getId() == id) {
      return object;
    }
  }

  return nullptr;
}

std::shared_ptr<GameWorld> GameWorld::createInstance()
{
  std::shared_ptr<GameWorld> gameWorld(new GameWorld());
  gameWorld->setGameSystemsGroup(std::make_unique<GameSystemsGroup>(gameWorld));

  return gameWorld;
}
