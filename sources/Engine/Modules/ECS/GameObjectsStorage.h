#pragma once

#include <utility>
#include <vector>
#include <algorithm>

#include "Utility/DynamicObjectsPool.h"

#include "GameObject.h"

class GameObjectsStorage;

struct ComponentsTypeInfo {
  template<class T>
  static size_t getTypeIndex()
  {
    static size_t typeIndex = s_typeIndex++;

    return typeIndex;
  }

  static size_t s_typeIndex;
};

template<class T>
class GameObjectComponentHandle {
 public:
  GameObjectComponentHandle(GameObjectId gameObjectId, GameObjectsStorage* gameObjectsStorage)
    : m_objectId(gameObjectId),
      m_gameObjectsStorage(gameObjectsStorage)
  {

  }

  inline T* operator->();
  inline const T* operator->() const;

  inline T* get();
  inline const T* get() const;

 private:
  GameObjectId m_objectId;
  GameObjectsStorage* m_gameObjectsStorage;
};

class GameObjectBaseComponentsUtility {
 public:
  explicit GameObjectBaseComponentsUtility(GameWorld* gameWorld, GameObjectsStorage* gameObjectsStorage)
    : m_gameWorld(gameWorld),
      m_gameObjectsStorage(gameObjectsStorage)
  {

  };

  virtual ~GameObjectBaseComponentsUtility() = default;

  virtual void emitRemoveEvent(const GameObject& gameObject) = 0;

 protected:
  GameWorld* m_gameWorld;
  GameObjectsStorage* m_gameObjectsStorage;
};

template<class T>
class GameObjectGenericComponentsUtility : public GameObjectBaseComponentsUtility {
 public:
  explicit GameObjectGenericComponentsUtility(GameWorld* gameWorld, GameObjectsStorage* gameObjectsStorage)
    : GameObjectBaseComponentsUtility(gameWorld, gameObjectsStorage)
  {

  }

  ~GameObjectGenericComponentsUtility() override = default;

  inline void emitRemoveEvent(const GameObject& gameObject) override;
};

template<class T>
struct GameObjectAddRemoveComponentEvent {
  mutable GameObject gameObject;
  mutable GameObjectComponentHandle<T> component;
};

template<class T>
struct GameObjectAddComponentEvent : public GameObjectAddRemoveComponentEvent<T> {

};

template<class T>
struct GameObjectRemoveComponentEvent : public GameObjectAddRemoveComponentEvent<T> {
};

class GameObjectsStorage {
 public:
  template<class T>
  using ComponentsPool = DynamicObjectsPool<T>;

 public:
  explicit GameObjectsStorage(GameWorld* gameWorld)
    : m_gameWorld(gameWorld)
  {
    m_componentsDataPools.resize(GameObjectData::MAX_COMPONENTS_COUNT, nullptr);
    m_componentsUtilities.resize(GameObjectData::MAX_COMPONENTS_COUNT, nullptr);
  };

  ~GameObjectsStorage()
  {
    for (const auto& object : m_gameObjects) {
      if (object.id != GameObjectNone) {
        GameObject tempObjectHandle = GameObject(object.id, object.revision, this);
        remove(tempObjectHandle);
      }
    }

    for (DynamicDataPool* pool : m_componentsDataPools) {
      delete pool;
    }

    for (GameObjectBaseComponentsUtility* componentUtility : m_componentsUtilities) {
      delete componentUtility;
    }
  }

  [[nodiscard]] inline const std::vector<GameObjectData>& getGameObjects() const
  {
    return m_gameObjects;
  }

  inline std::vector<GameObjectData>& getGameObjects()
  {
    return m_gameObjects;
  }

  inline GameObject create()
  {
    return createEmptyGameObject();
  }

  GameObject createNamed(const std::string& name)
  {
    GameObject gameObject = createEmptyGameObject();
    m_gameObjects[gameObject.m_id].name = name;

    m_gameObjectsNamesLookupTable.insert({name, gameObject.m_id});

    return gameObject;
  }

  inline GameObject createEmptyGameObject()
  {
    GameObjectId gameObjectId = GameObjectNone;

    if (m_freeGameObjectsIds.empty()) {
      gameObjectId = m_lastFreeGameObjectId;
      m_lastFreeGameObjectId++;

      m_gameObjects.push_back({.id = gameObjectId,
        .name = std::string(),
        .revision = 0,
        .componentsMask = 0,
        .isGhost = false});
    }
    else {
      gameObjectId = *m_freeGameObjectsIds.rbegin();
      m_freeGameObjectsIds.pop_back();

      size_t oldRevision = m_gameObjects[gameObjectId].revision;

      m_gameObjects[gameObjectId] = {.id = gameObjectId,
        .name = std::string(),
        .revision = oldRevision + 1,
        .componentsMask = 0,
        .isGhost = false};
    }

    return GameObject(gameObjectId, m_gameObjects[gameObjectId].revision, this);
  }

  inline void remove(GameObject& gameObject);

  inline GameObjectData* getGameObject(size_t index)
  {
    return &m_gameObjects[index];
  }

  [[nodiscard]] inline const GameObjectData* getGameObject(size_t index) const
  {
    return &m_gameObjects[index];
  }

  [[nodiscard]] inline size_t getSize() const
  {
    return m_gameObjects.size();
  }

  GameObject getByName(const std::string& name)
  {
    auto nameIt = m_gameObjectsNamesLookupTable.find(name);

    if (nameIt == m_gameObjectsNamesLookupTable.end()) {
      return GameObject();
    }

    return GameObject(nameIt->second, m_gameObjects[nameIt->second].revision, this);
  }

  inline GameObject getById(GameObjectId id)
  {
    if (id >= m_gameObjects.size()) {
      return GameObject();
    }
    else {
      return GameObject(id, m_gameObjects[id].revision, this);
    }
  }

  inline GameObject getByIndex(size_t index)
  {
    return getById(static_cast<GameObjectId>(index));
  }

  template<class T, class... Args>
  inline GameObjectComponentHandle<T> assignComponent(GameObject& gameObject, Args&& ... args);

  /*!
   * \brief Removes the component from the game object
   *
   * \param gameObject game object to remove the component
   */
  template<class T>
  inline void removeComponent(GameObject& gameObject);

  /*!
   * \brief Gets the component of the game object
   *
   * \param gameObject game object to get the component
   */
  template<class T>
  GameObjectComponentHandle<T> getComponent(const GameObject& gameObject)
  {
    auto& gameObjectData = m_gameObjects[gameObject.m_id];

    size_t typeId = ComponentsTypeInfo::getTypeIndex<T>();
    SW_ASSERT(gameObjectData.componentsMask.test(typeId));

    return GameObjectComponentHandle<T>(gameObject.m_id, this);
  }

  template<class T>
  inline ComponentsPool<T>* getComponentDataStorage() const
  {
    size_t typeId = ComponentsTypeInfo::getTypeIndex<T>();
    return static_cast<ComponentsPool<T>*>(m_componentsDataPools[typeId]);
  }

  template<class T>
  [[nodiscard]] inline bool hasComponent(const GameObject& gameObject) const
  {
    size_t typeId = ComponentsTypeInfo::getTypeIndex<T>();
    return m_gameObjects[gameObject.m_id].componentsMask.test(typeId);
  }

 private:
  GameWorld* m_gameWorld{};

  GameObjectId m_lastFreeGameObjectId = 0;

  std::vector<GameObjectData> m_gameObjects;
  std::vector<GameObjectId> m_freeGameObjectsIds;

  std::vector<DynamicDataPool*> m_componentsDataPools;
  std::vector<GameObjectBaseComponentsUtility*> m_componentsUtilities;

  std::unordered_map<std::string, GameObjectId> m_gameObjectsNamesLookupTable;

 private:
  template<class T>
  friend
  class GameObjectComponentHandle;

  friend class GameObject;
};
