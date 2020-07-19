#pragma once

#include <any>
#include <unordered_map>
#include <typeindex>

using GameObjectId = int;

constexpr GameObjectId GameObjectNone = -1;

class GameWorld;

/*!
 * \brief Class for a game object representation
 *
 * This class is used for representing a game object. 
 * It allows to assign and find components.
 */
class GameObject {
 public:
  GameObject(GameObjectId id, GameWorld* gameWorld);
  virtual ~GameObject();

  template<class T, class... Args>
  T& addComponent(Args&& ... args);

  template<class T>
  void removeComponent();

  /*!
   * \brief Gets the specified component
   *
   * \return the specified component handle
   */
  template<class T>
  T& getComponent();

  /*!
 * \brief Gets the specified component
 *
 * \return the specified component handle
 */
  template<class T>
  const T& getComponent() const;

  /*!
  * \brief Checks existing of the component
  *
  * \return
  */
  template<class T>
  [[nodiscard]] bool hasComponent() const;

  /*!
   * \brief Checks existing of several components
   *
   * \return
   */
  template<class T, class V, class... Types>
  [[nodiscard]] bool hasComponent() const;

  /*!
   * \brief Gets the game object id
   *
   * \return id of the game object
   */
  [[nodiscard]] GameObjectId getId() const;

  /*!
   * \brief Check whether the game object was destroyed
   *
   * \return
   */
  [[nodiscard]] bool isAlive() const;

 protected:
  GameObjectId m_id;
  std::unordered_map<std::type_index, std::any> m_components;

  bool m_isDestroyed;

  GameWorld* m_gameWorld;

 protected:
  friend class GameWorld;
};
