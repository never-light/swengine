#include "precompiled.h"
#pragma hdrstop

#include "GameObjectsGenericClassLoader.h"

#include <utility>

#include "Modules/Graphics/GraphicsSystem/TransformComponent.h"
#include "LevelsManager.h"

GameObjectsGenericClassLoader::GameObjectsGenericClassLoader(std::weak_ptr<LevelsManager> levelsManager)
  : m_levelsManager(std::move(levelsManager))
{

}

void GameObjectsGenericClassLoader::loadGameObject(GameObject& gameObject, const pugi::xml_node& objectNode)
{
  for (const pugi::xml_node& componentNode : objectNode.children()) {
    auto componentName = componentNode.name();

    loadComponent(gameObject, componentNode);
    onComponentLoaded(gameObject, componentName);
  }
}

void GameObjectsGenericClassLoader::loadComponent(GameObject& gameObject, const pugi::xml_node& componentNode)
{
  auto componentName = componentNode.name();
  auto componentLoader = m_levelsManager.lock()->getObjectsLoader().getComponentLoader(componentName);

  componentLoader(gameObject, componentNode);
}

void GameObjectsGenericClassLoader::onComponentLoaded(GameObject& gameObject, const std::string& componentName)
{
  ARG_UNUSED(gameObject);
  ARG_UNUSED(componentName);
}

void GameObjectsGenericClassLoader::loadComponent(GameObject& gameObject,
  const pugi::xml_node& objectNode,
  const std::string& componentName)
{
  loadComponent(gameObject, objectNode.child(componentName.c_str()));
}
