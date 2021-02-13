#pragma once

#include <Engine/Modules/ECS/ECS.h>
#include <Engine/Modules/ResourceManagement/ResourcesManagement.h>

#include <Engine/Utility/xml.h>

class GameComponentsLoader {
 public:
  explicit GameComponentsLoader(
    std::shared_ptr<GameWorld> gameWorld,
    std::shared_ptr<ResourcesManager> resourceManager);

  std::unique_ptr<BaseGameObjectsComponentBinder> loadPlayerData(const pugi::xml_node& data);
  std::unique_ptr<BaseGameObjectsComponentBinder> loadInventoryItemData(const pugi::xml_node& data);
  std::unique_ptr<BaseGameObjectsComponentBinder> loadInventoryData(const pugi::xml_node& data);
  std::unique_ptr<BaseGameObjectsComponentBinder> loadInteractiveData(const pugi::xml_node& data);
  std::unique_ptr<BaseGameObjectsComponentBinder> loadActorData(const pugi::xml_node& data);

 private:
  std::shared_ptr<GameWorld> m_gameWorld;
  std::shared_ptr<ResourcesManager> m_resourceManager;
};
