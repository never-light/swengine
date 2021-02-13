#pragma once

#include <vector>
#include <unordered_map>
#include <functional>
#include <typeindex>
#include <memory>
#include <utility>

#include "GameSystemsGroup.h"
#include "GameObject.h"
#include "GameObjectsFactory.h"
#include "GameObjectsStorage.h"

#include "GameObjectsSequentialView.h"
#include "GameObjectsComponentsView.h"

#include "EventsListener.h"

struct GameWorldSerializeHeader {
  uint32_t gameObjectsCount = 0;

  template<class Archive>
  void serialize(Archive& archive)
  {
    archive(gameObjectsCount);
  }
};

/*!
 * \brief Class for representing the game world
 * 
 * This class allows to store game object, their components
 * and systems. Also the class provide functions to find these things
 * and iterate over them.
 */
class GameWorld {
 public:
  GameWorld(const GameWorld& gameWorld) = delete;

  ~GameWorld()
  {
    reset();
  }

  void reset()
  {
    if (m_gameSystemsGroup->isConfigured()) {
      m_gameSystemsGroup->unconfigure();

      for (auto& system : m_gameSystemsGroup->getGameSystems()) {
        m_gameSystemsGroup->removeGameSystem(system);
      }

      m_gameObjectsStorage.reset();
    }
  }

  /*!
 * \brief Performs the game world update with fixed internal step
 *
 * The function performs update of the game world and
 * calls systems update methods
 *
 * \param delta delta time
 */
  void fixedUpdate(float delta)
  {
    m_gameSystemsGroup->fixedUpdate(delta);
  }

  /*!
   * \brief Performs the game world update
   *
   * The function performs update of the game world and
   * calls systems update methods
   *
   * \param delta delta time
   */
  void update(float delta)
  {
    m_gameSystemsGroup->update(delta);
  }

  /*!
   * \brief Renders the game world
   */
  void render()
  {
    m_gameSystemsGroup->render();
  }

  /*!
   * \brief It is called before rendering of the game world
   */
  void beforeRender()
  {
    m_gameSystemsGroup->beforeRender();
  }

  /*!
   * \brief It is called after rendering of the game world
   */
  void afterRender()
  {
    m_gameSystemsGroup->afterRender();
  }

  /*!
   * \brief getGameSystemsGroup Returns main game systems group
   * \return the main game systems group
   */
  GameSystemsGroup* getGameSystemsGroup() const
  {
    return m_gameSystemsGroup.get();
  }

  /*!
   * \brief Creates and registers a new game object
   *
   * \return the object pointer
   */
  GameObject createGameObject()
  {
    GameObject gameObject = m_gameObjectsStorage->create();
    emitEvent(GameObjectAddEvent{gameObject});

    return gameObject;
  }

  /*!
 * \brief Creates and registers a new game object with specified unique name
 *
 * \return the object pointer
 */
  GameObject createGameObject(const std::string& name)
  {
    GameObject gameObject = m_gameObjectsStorage->createNamed(name);
    emitEvent(GameObjectAddEvent{gameObject});

    return gameObject;
  }

  /*!
   * \brief Finds the game object by ID
   *
   * \param id Identifier of the game object
   * \return the object pointer
   */
  [[nodiscard]] GameObject findGameObject(GameObjectId id) const
  {
    return m_gameObjectsStorage->getById(id);
  }

  /*!
   * \brief Finds the game object by name
   *
   * \param id Name of the game object
   * \return the object pointer
   */
  [[nodiscard]] GameObject findGameObject(const std::string& name) const
  {
    return m_gameObjectsStorage->getByName(name);
  }

  /*!
   * \brief Finds the game object by predicate
   *
   * \param predicate predicate for the object determination
   * \return the object pointer
   */
  [[nodiscard]] GameObject findGameObject(const std::function<bool(const GameObject&)>& predicate)
  {
    for (const GameObjectData& object : m_gameObjectsStorage->getGameObjects()) {
      GameObject gameObject(object.id, object.revision, m_gameObjectsStorage.get());

      if (gameObject.isAlive() && predicate(gameObject)) {
        return gameObject;
      }
    }

    return GameObject();
  }

  /*!
   * \brief Removes the game objects
   *
   * \param gameObject removed game object
   */
  void removeGameObject(GameObject& gameObject)
  {
    m_gameObjectsStorage->remove(gameObject);
  }

  /*!
   * \brief Performs specified action for each existing game object
   *
   * \param action action to perform
   */
  void forEach(const std::function<void(GameObject&)>& action)
  {
    for (GameObject gameObject : this->all()) {
      action(gameObject);
    }
  }

  /*!
   * \brief Returns view for iterate over all game objects
   *
   * \return view for iterate over all game objects
   */
  [[nodiscard]] GameObjectsSequentialView all() const
  {
    GameObjectsSequentialIterator begin(m_gameObjectsStorage.get(), 0, false);
    GameObjectsSequentialIterator end(m_gameObjectsStorage.get(), m_gameObjectsStorage->getSize(), true);

    return GameObjectsSequentialView(begin, end);
  }

  /*!
   * \brief Returns view of game objects with specified components
   *
   * \return view of game objects with specified components
   */
  template<class ComponentType>
  GameObjectsComponentsView<ComponentType> allWith();

  /*!
   * \brief Subscribes the event listener for the specified event
   *
   * \param listener event listener object
   */
  template<class T>
  void subscribeEventsListener(EventsListener<T>* listener);

  /*!
   * Unsubscribes the event listener from the specified event
   *
   * \param listener event listener object
   */
  template<class T>
  void unsubscribeEventsListener(EventsListener<T>* listener);

  /*!
   * \brief Unsubscribes the event listener from all events
   *
   * \param listener events listener object
   */
  void cancelEventsListening(BaseEventsListener* listener)
  {
    for (auto& eventListeners : m_eventsListeners) {
      std::erase(eventListeners, listener);
    }
  }

  /*!
   * \brief Sends the event data to all appropriate listeners
   *
   * \param event event data
   */
  template<class T>
  EventProcessStatus emitEvent(const T& event);

  template<class Archive>
  void save(Archive& archive) const
  {
    size_t gameObjectsCount = 0;

    for (GameObject gameObject : all()) {
      if (gameObject.isAlive() && gameObject.isSerializable()) {
        gameObjectsCount++;
      }
    }

    GameWorldSerializeHeader serializeHeader{
      .gameObjectsCount = static_cast<uint32_t>(gameObjectsCount)
    };

    archive(serializeHeader);

    for (GameObject gameObject : all()) {
      if (gameObject.isAlive() && gameObject.isSerializable()) {
        std::string objectName = gameObject.getName();
        archive(objectName);

        SW_ASSERT(gameObject.isAlive());

        std::bitset<GameObjectData::MAX_COMPONENTS_COUNT> componentsMask = gameObject.getComponentsMask();

        archive(componentsMask);

        for (size_t componentBitIndex = 0; componentBitIndex < componentsMask.size(); componentBitIndex++) {
          if (componentsMask.test(componentBitIndex)) {
            if (ComponentsTypeInfo::isSerializable(componentBitIndex)) {
              ComponentsTypeInfo::performSave(archive, componentBitIndex,
                m_gameObjectsStorage->getComponentRawData(gameObject, componentBitIndex));
            }
          }
        }

      }
    }
  }

  template<class Archive>
  void load(Archive& archive)
  {
    GameWorldSerializeHeader serializeHeader;
    archive(serializeHeader);

    for (size_t gameObjectIndex = 0; gameObjectIndex < serializeHeader.gameObjectsCount; gameObjectIndex++) {
      std::string objectName;
      archive(objectName);

      GameObject gameObject{};

      if (objectName.empty()) {
        gameObject = createGameObject();
      }
      else {
        gameObject = createGameObject(objectName);
      }

      std::bitset<GameObjectData::MAX_COMPONENTS_COUNT> componentsMask;
      archive(componentsMask);

      for (size_t componentBitIndex = 0; componentBitIndex < componentsMask.size(); componentBitIndex++) {
        if (componentsMask.test(componentBitIndex)) {
          if (ComponentsTypeInfo::isSerializable(componentBitIndex)) {
            ComponentsTypeInfo::performLoad(archive, componentBitIndex,
              gameObject, m_gameObjectsStorage->getComponentBinderFactory(componentBitIndex));
          }
        }
      }
    }
  }

  template<class ComponentType>
  void registerComponentBinderFactory(std::shared_ptr<BaseGameObjectsComponentsBindersFactory> bindersFactory)
  {
    m_gameObjectsStorage->template registerComponentBinderFactory<ComponentType>(std::move(bindersFactory));
  }

 public:
  static std::shared_ptr<GameWorld> createInstance()
  {
    std::shared_ptr<GameWorld> gameWorld(new GameWorld());
    gameWorld->setGameSystemsGroup(std::make_unique<GameSystemsGroup>(gameWorld.get()));
    gameWorld->m_gameObjectsStorage = std::make_unique<GameObjectsStorage>(gameWorld.get());

    // Create internal ill-formed GameObject to mark the zero id as reserved
    RETURN_VALUE_UNUSED(gameWorld->createGameObject());

    return gameWorld;
  }

 private:
  GameWorld() = default;

  /*!
 * \brief setGameSystemsGroup Sets main game systems group
 * \param group group to set
 */
  void setGameSystemsGroup(std::unique_ptr<GameSystemsGroup> group)
  {
    m_gameSystemsGroup = std::move(group);
    m_gameSystemsGroup->configure();
    m_gameSystemsGroup->setActive(true);
  }

 private:
  std::unique_ptr<GameSystemsGroup> m_gameSystemsGroup;
  std::unique_ptr<GameObjectsStorage> m_gameObjectsStorage;

  std::vector<std::vector<BaseEventsListener*>> m_eventsListeners;

 private:
  friend class GameObject;
};

template<class ComponentType>
inline GameObjectsComponentsView<ComponentType> GameWorld::allWith()
{
  GameObjectsComponentsIterator<ComponentType> begin(m_gameObjectsStorage.get(), 0, false);
  GameObjectsComponentsIterator<ComponentType> end(m_gameObjectsStorage.get(), m_gameObjectsStorage->getSize(), true);

  return GameObjectsComponentsView<ComponentType>(begin, end);
}

template<class T>
inline void GameWorld::subscribeEventsListener(EventsListener<T>* listener)
{
  size_t typeId = EventsTypeInfo::getTypeIndex<T>();

  if (typeId >= m_eventsListeners.size()) {
    m_eventsListeners.resize(typeId + 1);
  }

  m_eventsListeners[typeId].push_back(listener);
}

template<class T>
inline void GameWorld::unsubscribeEventsListener(EventsListener<T>* listener)
{
  size_t typeId = EventsTypeInfo::getTypeIndex<T>();
  std::erase(m_eventsListeners[typeId], listener);
}

template<class T>
inline EventProcessStatus GameWorld::emitEvent(const T& event)
{
  bool processed = false;
  size_t typeId = EventsTypeInfo::getTypeIndex<T>();

  if (typeId < m_eventsListeners.size()) {
    for (BaseEventsListener* baseListener : m_eventsListeners[typeId]) {
      auto* listener = dynamic_cast<EventsListener<T>*>(baseListener);
      EventProcessStatus processStatus = listener->receiveEvent(event);

      if (processStatus == EventProcessStatus::Prevented) {
        return EventProcessStatus::Prevented;
      }
      else if (processStatus == EventProcessStatus::Processed) {
        processed = true;
      }
    }
  }

  return (processed) ? EventProcessStatus::Processed : EventProcessStatus::Skipped;
}
