#pragma once

#include <any>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <bitset>

using GameObjectId = uint32_t;
constexpr GameObjectId GameObjectNone = 0;

class GameObjectsStorage;

struct GameObjectData {
 public:
  static constexpr size_t MAX_COMPONENTS_COUNT = 64;

 public:
  GameObjectId id = GameObjectNone;
  std::string name;

  size_t revision{};
  std::bitset<GameObjectData::MAX_COMPONENTS_COUNT> componentsMask;

  // TODO: remove ghost game objects conception from engine code
  bool isGhost{};

  bool isSerializable = false;
};

template<class T>
class GameObjectComponentHandle;

/*!
 * \brief Class for a game object representation
 *
 * This class is used for representing a game object. 
 * It allows to assign and find components.
 */
class GameObject {
 public:
  GameObject()
    : m_id(GameObjectNone),
      m_revision(0),
      m_objectsStorage(nullptr)
  {

  }

  GameObject(GameObjectId id, size_t revision, GameObjectsStorage* objectsStorage)
    : m_id(id),
      m_revision(revision),
      m_objectsStorage(objectsStorage)
  {

  }

  ~GameObject() = default;

  inline bool operator==(const GameObject& other) const
  {
    return m_id == other.m_id && m_revision == other.m_revision;
  }

  template<class T, class... Args>
  GameObjectComponentHandle<T> addComponent(Args&& ... args);

  template<class T>
  void removeComponent();

  template<class T>
  inline GameObjectComponentHandle<T> getComponent();

  template<class T>
  [[nodiscard]] inline bool hasComponent() const;

  [[nodiscard]] inline GameObjectId getId() const;

  /*!
   * @brief Check whether the game object is alive
   *
   * Aliveness status should be used to check whether the object
   * is ready for processing by game systems
   *
   * @return
   */
  [[nodiscard]] inline bool isAlive() const;

  /*!
   * \brief Returns the game object's name
   *
   * \return game object's name
   */
  [[nodiscard]] inline const std::string& getName() const;

  /*!
 * \brief Returns the game object's revision
 *
 * \return game object's revision
 */
  [[nodiscard]] inline size_t getRevision() const;

  /*!
   * @brief Checks that the game object is formed (has ID)
   * @return Object formation status
   */
  [[nodiscard]] inline bool isFormed() const;

  /*!
   * @brief Checks whether the game object is valid
   *
   * Object is valid if it is formed and was not deleted
   *
   * @return Object validity status
   */
  [[nodiscard]] inline bool isValid() const;

  [[nodiscard]] inline std::bitset<GameObjectData::MAX_COMPONENTS_COUNT> getComponentsMask() const;

  [[nodiscard]] inline bool isSerializable() const;
  
  inline void markAsSerializable(bool isSerializable);

 private:
  GameObjectId m_id;
  size_t m_revision;

  GameObjectsStorage* m_objectsStorage;

 private:
  friend class GameWorld;

  friend class GameObjectsStorage;
};

struct GameObjectAddRemoveEvent {
  mutable GameObject gameObject;
};

struct GameObjectAddEvent : public GameObjectAddRemoveEvent {
};

struct GameObjectRemoveEvent : public GameObjectAddRemoveEvent {
};
