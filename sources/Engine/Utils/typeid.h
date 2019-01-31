#pragma once

#include <typeinfo>
#include <typeindex>

using TypeId = std::type_index;

template<class T>
TypeId getTypeId() {
	return std::type_index(typeid(T));
}