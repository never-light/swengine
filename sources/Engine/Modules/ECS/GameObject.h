#pragma once

#include <unordered_map>
#include <typeindex>

#include "ComponentInstance.h"
#include "ComponentHandle.h"

using GameObjectId = size_t;

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
	ComponentHandle<T> addComponent(Args&&... args);

	template<class T>
	void removeComponent();

	/*!
	 * \brief Gets the specified component
	 * 
	 * \return the specified component handle
	 */
	template<class T>
	ComponentHandle<T> getComponent() const;

	/*!
	* \brief Checks existing of the component
	* 
	* \return
	*/
	template<class T>
	bool hasComponent() const;

	/*!
	 * \brief Checks existing of several components
	 * 
	 * \return 
	 */
	template<class T, class V, class... Types>
	bool hasComponent() const;

	/*!
	 * \brief Gets the game object id
	 * 
	 * \return id of the game object
	 */
	GameObjectId getId() const;

	/*!
	 * \brief Check whether the game object was destroyed
	 * 
	 * \return 
	 */
	bool isDestroyed() const;
protected:
	GameObjectId m_id;
    std::unordered_map<std::type_index, BaseComponentInstance*> m_components;

	bool m_isDestroyed;

	GameWorld* m_gameWorld;

protected:
	friend class GameWorld;
};
