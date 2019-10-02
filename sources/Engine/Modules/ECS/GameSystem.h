#pragma once

class GameWorld;

/*!
 * \brief Class for representing a game system
 *
 * This class allows to represent a game system with user-specified functionality
 */
class GameSystem {
public:
	GameSystem();
	virtual ~GameSystem();

	/*!
	 * \brief Performs the game system update
	 *
	 * \param gameWorld the game world pointer
	 * \param delta delta time
	 */
    virtual void update(GameWorld* gameWorld, float delta);

	/*!
	 * \brief Renders the game system data
	 */
	virtual void render(GameWorld* gameWorld);

	/*!
	 * \brief Calls at the time of the game system registration
	 * 
	 * \param gameWorld the game world pointer
	 */
	virtual void configure(GameWorld* gameWorld);

	/*!
	 * \brief Calls at the time of the game system removal
	 *
	 * \param gameWorld the game world pointer
	 */
	virtual void unconfigure(GameWorld* gameWorld);
};
