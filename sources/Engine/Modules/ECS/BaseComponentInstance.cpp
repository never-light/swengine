#include "precompiled.h"

#pragma hdrstop

#include "BaseComponentInstance.h"

BaseComponentInstance::BaseComponentInstance(GameObject* gameObject)
    : m_gameObject(gameObject) {

}

BaseComponentInstance::~BaseComponentInstance() = default;

GameObject* BaseComponentInstance::getGameObject() const {
  return m_gameObject;
}
