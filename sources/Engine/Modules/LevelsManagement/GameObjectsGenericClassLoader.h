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

  std::unordered_map<std::string, std::unique_ptr<BaseGameObjectsComponentBinder>>
  loadGameObject(const pugi::xml_node& objectNode) override;

  std::unique_ptr<BaseGameObjectsComponentBinder> loadComponent(const pugi::xml_node& componentNode) override;

  void onComponentLoaded(const std::string& componentName, BaseGameObjectsComponentBinder& componentBinder) override;

  std::unique_ptr<BaseGameObjectsComponentBinder> loadComponent(
    const pugi::xml_node& objectNode,
    const std::string& componentName) override;

 private:
  std::weak_ptr<LevelsManager> m_levelsManager;
};
