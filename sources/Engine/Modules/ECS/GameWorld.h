#pragma once

#include <vector>
#include <unordered_map>
#include <functional>
#include <typeindex>
#include <memory>
#include <utility>

#include "GameSystemsGroup.h"
#include "GameObject.h"
#include "ComponentInstance.h"

#include "GameObjectsSequentialView.h"
#include "GameObjectsComponentsView.h"

#include "EventsListener.h"

// TODO: declare and use type alias like GameObjectRef instead of std::shared_ptr<GameObject>

// TODO: check whether the game object alive before any actions with it

// TODO: remove proxy object for game objects' components, forbid ability to store owning references to components

// TODO: replace raw EventListener pointers with shared pointers to avoid memory leaks or usage of invalid pointers

// TODO: add ability to subscribe to events not only objects but also callbacks, probably replace
// polymorphic objects with std::function objects, but foresee deleting of them (for example, by unique identifies)

/*!
 * \brief Class for representing the game world
 * 
 * This class allows to store game object, their components
 * and systems. Also the class provide functions to find these things
 * and iterate over them.
 */
class GameWorld : public std::enable_shared_from_this<GameWorld> {
 public:
  ~GameWorld();

  /*!
   * \brief Performs the game world update
   *
   * The function performs update of the game world and
   * calls systems update methods
   *
   * \param delta delta time
   */
  void update(float delta);

  /*!
   * \brief Renders the game world
   */
  void render();

  /*!
   * \brief It is called before rendering of the game world
   */
  void beforeRender();

  /*!
   * \brief It is called after rendering of the game world
   */
  void afterRender();

  /*!
   * \brief getGameSystemsGroup Returns main game systems group
   * \return the main game systems group
   */
  GameSystemsGroup* getGameSystemsGroup() const;

  /*!
   * \brief Creates and registers a new game object
   *
   * \return the object pointer
   */
  std::shared_ptr<GameObject> createGameObject();

  /*!
   * \brief Finds the game object by ID
   *
   * \param id Identifier of the game object
   * \return the object pointer
   */
  std::shared_ptr<GameObject> findGameObject(GameObjectId id) const;

  /*!
   * \brief Finds the game object by predicate
   *
   * \param predicate predicate for the object determination
   * \return the object pointer
   */
  std::shared_ptr<GameObject> findGameObject(std::function<bool(const GameObject&)> predicate) const;

  /*!
   * \brief Removes the game objects
   *
   * \param gameObject removed game object
   */
  void removeGameObject(std::shared_ptr<GameObject> gameObject);

  /*!
   * \brief Performs specified action for each existing game object
   *
   * \param action action to perform
   */
  void forEach(std::function<void(GameObject&)> action);

  /*!
   * \brief Returns view for iterate over all game objects
   *
   * \return view for iterate over all game objects
   */
  GameObjectsSequentialView all();

  /*!
   * \brief Returns view of game objects with specified components
   *
   * \return view of game objects with specified components
   */
  template<class... ComponentsTypes>
  GameObjectsComponentsView<ComponentsTypes...> allWith();

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
  void cancelEventsListening(BaseEventsListener* listener);

  /*!
   * \brief Sends the event data to all appropriate listeners
   *
   * \param event event data
   */
  template<class T>
  EventProcessStatus emitEvent(const T& event);

 public:
  static std::shared_ptr<GameWorld> createInstance();

 private:
  GameWorld();
  GameWorld(const GameWorld& gameWorld) = delete;

  void removeDestroyedObjects();

  /*!
 * \brief setGameSystemsGroup Sets main game systems group
 * \param group group to set
 */
  void setGameSystemsGroup(std::unique_ptr<GameSystemsGroup> group);

 private:
  /*!
 * \brief Adds the new component to the game object
 *
 * \param gameObject game object to add the component
 * \param ...args args for the component constructor
 * \return ComponentHandle object
 */
  template<class T, class... Args>
  ComponentHandle<T> assignComponent(GameObject& gameObject, Args&& ... args);

  /*!
   * \brief Removes the component from the game object
   *
   * \param gameObject game object to remove the component
   */
  template<class T>
  void removeComponent(GameObject& gameObject);

  /*!
   * \brief Gets the game object by it's index
   *
   * \param index index of the game object
   * \return the object pointer
   */
  GameObject& getGameObjectByIndex(size_t index) const;

  /*!
   * \brief Checks whether the game object exists by it's index
   *
   * \param index index of the game object
   * \return the existing status
   */
  bool hasGameObjectWithIndex(size_t index) const;

  /*!
   * \brief Gets the number of game objects
   *
   * \return the number of game objects
   */
  size_t getGameObjectsCount() const;

 private:
  GameObjectId m_lastGameObjectId = -1;

 private:
  std::unique_ptr<GameSystemsGroup> m_gameSystemsGroup;
  std::vector<std::shared_ptr<GameObject>> m_gameObjects;

  std::unordered_map<std::type_index, std::vector<BaseEventsListener*>> m_eventsListeners;

 private:
  friend class GameObject;

  template<class... ComponentsTypes>
  friend class GameObjectsComponentsIterator;

  friend class GameObjectsSequentialIterator;
};

template<class T, class ...Args>
inline ComponentHandle<T> GameWorld::assignComponent(GameObject& gameObject, Args&& ...args)
{
  std::type_index typeId = std::type_index(typeid(T));

  T component(std::forward<Args>(args)...);

  ComponentInstance<T>* instance = new ComponentInstance<T>(&gameObject, std::move(component));

  gameObject.m_components[typeId] = instance;

  return ComponentHandle<T>(instance->getDataPtr());
}

template<class T>
inline void GameWorld::removeComponent(GameObject& gameObject)
{
  std::type_index typeId = std::type_index(typeid(T));

  auto componentIterator = gameObject.m_components.find(typeId);

  if (componentIterator == gameObject.m_components.end()) {
    return;
  }

  BaseComponentInstance* instance = componentIterator->second;

  gameObject.m_components.erase(componentIterator);

  delete instance;
}

template<class ...ComponentsTypes>
inline GameObjectsComponentsView<ComponentsTypes...> GameWorld::allWith()
{
  GameObjectsComponentsIterator<ComponentsTypes...> begin(this, 0, false);
  GameObjectsComponentsIterator<ComponentsTypes...> end(this, m_gameObjects.size(), true);

  return GameObjectsComponentsView<ComponentsTypes...>(begin, end);
}

template<class T>
inline void GameWorld::subscribeEventsListener(EventsListener<T>* listener)
{
  std::type_index typeId = std::type_index(typeid(T));

  auto eventListenersListIt = m_eventsListeners.find(typeId);

  if (eventListenersListIt == m_eventsListeners.end()) {
    m_eventsListeners.insert({typeId, std::vector<BaseEventsListener*>{listener}});
  }
  else {
    eventListenersListIt->second.push_back(listener);
  }
}

template<class T>
inline void GameWorld::unsubscribeEventsListener(EventsListener<T>* listener)
{
  std::type_index typeId = std::type_index(typeid(T));

  auto eventListenersListIt = m_eventsListeners.find(typeId);

  if (eventListenersListIt == m_eventsListeners.end()) {
    return;
  }

  eventListenersListIt->second.erase(std::remove(eventListenersListIt->second.begin(),
    eventListenersListIt->second.end(), listener),
    eventListenersListIt->second.end());
}

template<class T>
inline EventProcessStatus GameWorld::emitEvent(const T& event)
{
  bool processed = false;
  std::type_index typeId = std::type_index(typeid(T));

  auto eventListenersListIt = m_eventsListeners.find(typeId);

  if (eventListenersListIt != m_eventsListeners.end()) {
    for (BaseEventsListener* baseListener : eventListenersListIt->second) {
      EventsListener<T>* listener = reinterpret_cast<EventsListener<T>*>(baseListener);
      EventProcessStatus processStatus = listener->receiveEvent(this, event);

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
