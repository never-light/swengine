#pragma once

class GameWorld;

class GameSystemsGroup;

/*!
 * \brief Class for representing a game system
 *
 * This class allows to represent a game system with user-specified functionality
 */
class GameSystem {
 public:
  GameSystem();
  explicit GameSystem(GameWorld* gameWorld);

  virtual ~GameSystem();

  /*!
* \brief Performs the game system update with fixed internal step
*
* \param gameWorld the game world pointer
* \param delta delta time
*/
  virtual void fixedUpdate(float delta);

  /*!
   * \brief Performs the game system update
   *
   * \param gameWorld the game world pointer
   * \param delta delta time
   */
  virtual void update(float delta);

  /*!
   * \brief Renders the game system data
   */
  virtual void render();

  /*!
   * \brief It is called before rendering of the game world
   */
  virtual void beforeRender();

  /*!
   * \brief It is called after rendering of the game world
   */
  virtual void afterRender();

  /*!
   * \brief Calls at the time of the game system registration
   *
   * \param gameWorld the game world pointer
   */
  virtual void configure();

  /*!
   * \brief Calls at the time of the game system removal
   *
   * \param gameWorld the game world pointer
   */
  virtual void unconfigure();

  /*!
   * \brief Sets the game system active status
   *
   * \param isActive the active status flag
   */
  void setActive(bool isActive);

  /*!
   * @brief Checks whether the game system is active
   *
   * @return active status flag
   */
  [[nodiscard]] bool isActive() const;

  /*!
   * \brief Calls at the time of the game system registration
   */
  virtual void activate();

  /*!
   * \brief Calls at the time of the game system removal
   */
  virtual void deactivate();

  /*!
   * @brief Returns GameWorld reference
   * @return GameWorld reference
   */
  [[nodiscard]] inline GameWorld* getGameWorld() const {
    return m_gameWorld;
  }

 private:
  bool m_isActive = false;

  GameWorld* m_gameWorld{};

 private:
  friend class GameSystemsGroup;
};
