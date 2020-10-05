#pragma once

#include <utility>
#include <vector>
#include <algorithm>

#include "GameObject.h"
#include "GameObjectsStorage.h"
#include "GameWorld.h"

#include "swdebug.h"

inline void GameObjectsStorage::remove(GameObject& gameObject)
{
  SW_ASSERT(gameObject.isValid());

  auto& objectData = m_gameObjects[gameObject.m_id];

  for (size_t componentIndex = 0; componentIndex < GameObjectData::MAX_COMPONENTS_COUNT; componentIndex++) {
    if (objectData.componentsMask.test(componentIndex)) {
      m_componentsUtilities[componentIndex]->emitRemoveEvent(gameObject);
      m_componentsDataPools[componentIndex]->freeObject(gameObject.m_id);

      objectData.componentsMask.reset(componentIndex);
    }
  }

  m_gameWorld->emitEvent(GameObjectRemoveEvent{gameObject});

  if (!objectData.name.empty()) {
    m_gameObjectsNamesLookupTable.erase(objectData.name);
  }

  m_freeGameObjectsIds.push_back(objectData.id);
  objectData.id = GameObjectNone;
}

template<class T, class... Args>
inline GameObjectComponentHandle<T> GameObjectsStorage::assignComponent(GameObject& gameObject, Args&& ... args)
{
  size_t typeId = ComponentsTypeInfo::getTypeIndex<T>();

  if (m_componentsDataPools[typeId] == nullptr) {
    m_componentsDataPools[typeId] = new ComponentsPool<T>(8192);
    m_componentsUtilities[typeId] = new GameObjectGenericComponentsUtility<T>(m_gameWorld, this);
  }

  auto& gameObjectData = m_gameObjects[gameObject.m_id];

  SW_ASSERT(!gameObjectData.componentsMask.test(typeId));

  ComponentsPool<T>* componentStorage = getComponentDataStorage<T>();

  if (gameObject.m_id >= componentStorage->getSize()) {
    componentStorage->fit(gameObject.m_id + 1);
  }

  T* componentPtr = ::new(componentStorage->getObject(gameObject.m_id)) T(std::forward<Args>(args)...);
  LOCAL_VALUE_UNUSED(componentPtr);

  gameObjectData.componentsMask.set(typeId);

  GameObjectComponentHandle<T> componentReference(gameObject.m_id, this);

  m_gameWorld->emitEvent(GameObjectAddComponentEvent<T>{gameObject, componentReference});
  return componentReference;
}

template<class T>
inline void GameObjectsStorage::removeComponent(GameObject& gameObject)
{
  size_t typeId = ComponentsTypeInfo::getTypeIndex<T>();

  auto& gameObjectData = m_gameObjects[gameObject.m_id];

  SW_ASSERT(gameObjectData.componentsMask.test(typeId));

  ComponentsPool<T>* componentStorage = getComponentDataStorage<T>();

  m_componentsUtilities[typeId]->emitRemoveEvent(gameObject);

  componentStorage->freeObject(gameObject.m_id);
  gameObjectData.componentsMask.reset(typeId);
}

template<class T>
inline T* GameObjectComponentHandle<T>::operator->()
{
  return static_cast<T*>(m_gameObjectsStorage->getComponentDataStorage<T>()->getObject(m_objectId));
}

template<class T>
inline const T* GameObjectComponentHandle<T>::operator->() const
{
  return static_cast<T*>(m_gameObjectsStorage->getComponentDataStorage<T>()->getObject(m_objectId));
}

template<class T>
inline T* GameObjectComponentHandle<T>::get()
{
  return static_cast<T*>(m_gameObjectsStorage->getComponentDataStorage<T>()->getObject(m_objectId));
}

template<class T>
inline const T* GameObjectComponentHandle<T>::get() const
{
  return static_cast<T*>(m_gameObjectsStorage->getComponentDataStorage<T>()->getObject(m_objectId));
}

template<class T>
inline void GameObjectGenericComponentsUtility<T>::emitRemoveEvent(const GameObject& gameObject)
{
  GameObjectComponentHandle<T> componentReference(gameObject.getId(), m_gameObjectsStorage);
  m_gameWorld->emitEvent(GameObjectRemoveComponentEvent<T>{gameObject, componentReference});
}
