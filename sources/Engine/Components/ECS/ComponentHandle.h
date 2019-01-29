#pragma once

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

	bool isValid() const {
		return m_componentPtr != nullptr;
	}

private:
	T* m_componentPtr;
};
