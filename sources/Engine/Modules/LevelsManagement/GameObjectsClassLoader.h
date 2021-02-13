#pragma once

#include "Modules/ECS/ECS.h"
#include "Modules/ECS/GameObjectsFactory.h"
#include "Utility/xml.h"

class GameObjectsClassLoader {
 public:
  GameObjectsClassLoader() = default;
  virtual ~GameObjectsClassLoader() = default;

  virtual std::unordered_map<std::string, std::unique_ptr<BaseGameObjectsComponentBinder>>
  loadGameObject(const pugi::xml_node& objectNode) = 0;
  virtual std::unique_ptr<BaseGameObjectsComponentBinder> loadComponent(const pugi::xml_node& componentNode) = 0;

  virtual void onComponentLoaded(const std::string& componentName, BaseGameObjectsComponentBinder& componentBinder) = 0;

  virtual std::unique_ptr<BaseGameObjectsComponentBinder> loadComponent(
    const pugi::xml_node& objectNode,
    const std::string& componentName) = 0;
};
