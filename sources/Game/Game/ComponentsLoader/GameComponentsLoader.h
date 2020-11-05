#pragma once

#include <Engine/Modules/ECS/ECS.h>
#include <Engine/Modules/ResourceManagement/ResourcesManagement.h>

#include <Engine/Utility/xml.h>

#include "GameComponentsBinders.h"

class GameComponentsLoader {
 public:
  explicit GameComponentsLoader(
    std::shared_ptr<GameWorld> gameWorld,
    std::shared_ptr<ResourcesManager> resourceManager);

  std::unique_ptr<GameObjectsComponentBinder> loadPlayerData(const pugi::xml_node& data);
  std::unique_ptr<GameObjectsComponentBinder> loadInventoryItemData(const pugi::xml_node& data);
  std::unique_ptr<GameObjectsComponentBinder> loadInventoryData(const pugi::xml_node& data);
  std::unique_ptr<GameObjectsComponentBinder> loadInteractiveData(const pugi::xml_node& data);
  std::unique_ptr<GameObjectsComponentBinder> loadActorData(const pugi::xml_node& data);

 private:
  std::shared_ptr<GameWorld> m_gameWorld;
  std::shared_ptr<ResourcesManager> m_resourceManager;
};
