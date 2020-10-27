#pragma once

#include "Modules/ECS/ECS.h"
#include "Modules/ResourceManagement/ResourcesManagement.h"

#include "Utility/xml.h"

#include "GameObjectsClassLoader.h"

class LevelsManager;

class GameObjectsGenericClassLoader : public GameObjectsClassLoader {
 public:
  explicit GameObjectsGenericClassLoader(std::weak_ptr<LevelsManager> levelsManager);
  ~GameObjectsGenericClassLoader() override = default;

  void loadGameObject(GameObject& gameObject, const pugi::xml_node& objectNode) override;
  void loadComponent(GameObject& gameObject, const pugi::xml_node& componentNode) override;

  void onComponentLoaded(GameObject& gameObject, const std::string& componentName) override;

  void loadComponent(
    GameObject& gameObject,
    const pugi::xml_node& objectNode,
    const std::string& componentName) override;

 private:
  std::weak_ptr<LevelsManager> m_levelsManager;
};
