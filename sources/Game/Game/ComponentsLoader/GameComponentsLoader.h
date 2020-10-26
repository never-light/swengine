#pragma once

#include <Engine/Modules/ECS/ECS.h>
#include <Engine/Modules/ResourceManagement/ResourceManager.h>

#include <Engine/Utility/xml.h>

class GameComponentsLoader {
 public:
  explicit GameComponentsLoader(
    std::shared_ptr<GameWorld> gameWorld,
    std::shared_ptr<ResourcesManager> resourceManager);

  void loadPlayerData(GameObject& gameObject, const pugi::xml_node& data);
  void loadInventoryItemData(GameObject& gameObject, const pugi::xml_node& data);
  void loadInventoryData(GameObject& gameObject, const pugi::xml_node& data);
  void loadInteractiveData(GameObject& gameObject, const pugi::xml_node& data);
  void loadActorData(GameObject& gameObject, const pugi::xml_node& data);

 private:
  std::shared_ptr<GameWorld> m_gameWorld;
  std::shared_ptr<ResourcesManager> m_resourceManager;
};
