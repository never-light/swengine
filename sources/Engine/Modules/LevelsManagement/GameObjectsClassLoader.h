#pragma once

#include "Modules/ECS/ECS.h"
#include "Utility/xml.h"

#include "GameObjectsComponentsBinders.h"

class GameObjectsClassLoader {
 public:
  GameObjectsClassLoader() = default;
  virtual ~GameObjectsClassLoader() = default;

  virtual std::unordered_map<std::string, std::unique_ptr<GameObjectsComponentBinder>> loadGameObject(const pugi::xml_node& objectNode) = 0;
  virtual std::unique_ptr<GameObjectsComponentBinder> loadComponent(const pugi::xml_node& componentNode) = 0;

  virtual void onComponentLoaded(const std::string& componentName, GameObjectsComponentBinder& componentBinder) = 0;

  virtual std::unique_ptr<GameObjectsComponentBinder> loadComponent(
    const pugi::xml_node& objectNode,
    const std::string& componentName) = 0;
};
