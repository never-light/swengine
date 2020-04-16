#include "precompiled.h"

#pragma hdrstop

#include "GameWorld.h"
#include <algorithm>

GameWorld::GameWorld() {
}

GameWorld::~GameWorld() {
  m_gameSystemsGroup->unconfigure(this);

  for (GameObject* object : m_gameObjects) {
    for (auto& it : object->m_components) {
      delete it.second;
    }

    object->m_isDestroyed = true;

    delete object;
  }
}

void GameWorld::update(float delta) {
  m_gameSystemsGroup->update(this, delta);

  removeDestroyedObjects();
}

void GameWorld::render() {
  m_gameSystemsGroup->render(this);
}

void GameWorld::beforeRender() {
  m_gameSystemsGroup->beforeRender(this);
}

void GameWorld::afterRender() {
  m_gameSystemsGroup->afterRender(this);
}

void GameWorld::setGameSystemsGroup(std::unique_ptr<GameSystemsGroup> group) {
  m_gameSystemsGroup = std::move(group);
  m_gameSystemsGroup->configure(this);
}

GameSystemsGroup* GameWorld::getGameSystemsGroup() const {
  return m_gameSystemsGroup.get();
}

GameObject* GameWorld::createGameObject() {
  m_lastGameObjectId++;
  GameObject* gameObject = new GameObject(m_lastGameObjectId, this);

  m_gameObjects.push_back(gameObject);

  return gameObject;
}

GameObject* GameWorld::findGameObject(const std::function<bool(GameObject*)> predicate) const {
  for (GameObject* object : m_gameObjects) {
    if (!object->isDestroyed() && predicate(object)) {
      return object;
    }
  }

  return nullptr;
}

GameObject* GameWorld::getGameObjectByIndex(size_t index) const {
  return m_gameObjects[index];
}

bool GameWorld::hasGameObjectWithIndex(size_t index) const {
  return index < m_gameObjects.size();
}

size_t GameWorld::getGameObjectsCount() const {
  return m_gameObjects.size();
}

void GameWorld::removeGameObject(GameObject* gameObject) {
  gameObject->m_isDestroyed = true;
}

void GameWorld::forEach(std::function<void(GameObject*)> action) {
  for (GameObject* gameObject : this->all()) {
    action(gameObject);
  }
}

GameObjectsSequentialView GameWorld::all() {
  GameObjectsSequentialIterator begin(this, 0, false);
  GameObjectsSequentialIterator end(this, m_gameObjects.size(), true);

  return GameObjectsSequentialView(begin, end);
}

void GameWorld::cancelEventsListening(BaseEventsListener* listener) {
  for (auto& it : m_eventsListeners) {
    std::vector<BaseEventsListener*>& listeners = it.second;
    listeners.erase(std::remove(listeners.begin(), listeners.end(), listener), listeners.end());
  }
}

void GameWorld::removeDestroyedObjects() {
  std::for_each(m_gameObjects.begin(), m_gameObjects.end(), [](GameObject*& obj) {
    if (obj->isDestroyed()) {
      for (auto& it : obj->m_components) {
        delete it.second;
      }

      delete obj;
      obj = nullptr;
    }
  });

  m_gameObjects.erase(std::remove(m_gameObjects.begin(), m_gameObjects.end(), nullptr), m_gameObjects.end());
}

GameObject* GameWorld::findGameObject(GameObjectId id) const {
  for (GameObject* object : m_gameObjects) {
    if (!object->isDestroyed() && object->getId() == id) {
      return object;
    }
  }

  return nullptr;
}
