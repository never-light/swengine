#pragma once

/*!
 * \brief Lightweight pointer to the game component data
 */
template<class T>
class ComponentHandle {
public:
	ComponentHandle()
		: m_componentPtr(nullptr)
	{ }

	ComponentHandle(T* component)
		: m_componentPtr(component)
	{ }

	T* operator->() const {
		return m_componentPtr;
	}

	T& get() {
		return *m_componentPtr;
	}

	/*!
	 * \brief Checks validity of the component data
	 * 
	 * \return 
	 */
	bool isValid() const {
		return m_componentPtr != nullptr;
	}

private:
	T* m_componentPtr;
};
