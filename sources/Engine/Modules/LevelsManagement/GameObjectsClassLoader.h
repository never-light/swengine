#pragma once

#include "Modules/ECS/ECS.h"
#include "Utility/xml.h"

class GameObjectsClassLoader {
 public:
  GameObjectsClassLoader() = default;
  virtual ~GameObjectsClassLoader() = default;

  virtual void loadGameObject(GameObject& gameObject, const pugi::xml_node& objectNode) = 0;
  virtual void loadComponent(GameObject& gameObject, const pugi::xml_node& componentNode) = 0;

  virtual void onComponentLoaded(GameObject& gameObject, const std::string& componentName) = 0;
};
