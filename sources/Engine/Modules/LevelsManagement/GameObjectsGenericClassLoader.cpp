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

std::unordered_map<std::string, std::unique_ptr<GameObjectsComponentBinder>> GameObjectsGenericClassLoader::loadGameObject(
  const pugi::xml_node& objectNode)
{
  std::unordered_map<std::string, std::unique_ptr<GameObjectsComponentBinder>> componentsBinders;

  for (const pugi::xml_node& componentNode : objectNode.children()) {
    auto componentName = componentNode.name();

    componentsBinders.insert({componentName, std::move(loadComponent(componentNode))});
    onComponentLoaded(componentName, *componentsBinders.at(componentName).get());
  }

  return std::move(componentsBinders);
}

std::unique_ptr<GameObjectsComponentBinder> GameObjectsGenericClassLoader::loadComponent(
  const pugi::xml_node& componentNode)
{
  auto componentName = componentNode.name();
  auto componentLoader = m_levelsManager.lock()->getObjectsLoader().getComponentLoader(componentName);

  return componentLoader(componentNode);
}

void GameObjectsGenericClassLoader::onComponentLoaded(const std::string& componentName,
  GameObjectsComponentBinder& componentBinder)
{
  ARG_UNUSED(componentName);
  ARG_UNUSED(componentBinder);
}

std::unique_ptr<GameObjectsComponentBinder> GameObjectsGenericClassLoader::loadComponent(
  const pugi::xml_node& objectNode,
  const std::string& componentName)
{
  return loadComponent(objectNode.child(componentName.c_str()));
}
