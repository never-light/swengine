#include "precompiled.h"

#pragma hdrstop

#include "ECS.h"

size_t ComponentsTypeInfo::s_typeIndex = 0;
ComponentTypeData ComponentsTypeInfo::s_componentsData[GameObjectData::MAX_COMPONENTS_COUNT]{};

size_t EventsTypeInfo::s_typeIndex = 0;