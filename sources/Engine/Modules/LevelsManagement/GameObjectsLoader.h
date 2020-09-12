#pragma once

#include <utility>

#include "Modules/ECS/ECS.h"
#include "Modules/ResourceManagement/ResourceManager.h"

#include "Utility/xml.h"

#include "GameObjectsClassLoader.h"

class GameObjectsLoader {
 public:
  using ComponentLoaderCallback = std::function<void(GameObject&, const pugi::xml_node&)>;

 public:
  GameObjectsLoader(std::shared_ptr<GameWorld> gameWorld,
    std::shared_ptr<ResourceManager> resourceManager);
  ~GameObjectsLoader();

  void registerClassLoader(const std::string& className, std::unique_ptr<GameObjectsClassLoader> loader);
  void registerGenericComponentLoader(const std::string& componentName, const ComponentLoaderCallback& loader);

  [[nodiscard]] ComponentLoaderCallback getComponentLoader(const std::string& componentName) const;
  ResourceManager& getResourceManager();

  GameObject& loadGameObject(const pugi::xml_node& objectNode);

 public:
  void loadTransformData(GameObject& gameObject, const pugi::xml_node& data);
  void loadVisualData(GameObject& gameObject, const pugi::xml_node& data);
  void loadRigidBodyData(GameObject& gameObject, const pugi::xml_node& data);
  void loadEnvironmentData(GameObject& gameObject, const pugi::xml_node& data);
  void loadAudioSourceData(GameObject& gameObject, const pugi::xml_node& data);

 private:
  std::shared_ptr<GameWorld> m_gameWorld;
  std::shared_ptr<ResourceManager> m_resourceManager;

  std::unordered_map<std::string, std::unique_ptr<GameObjectsClassLoader>> m_classesLoaders;
  std::unordered_map<std::string, ComponentLoaderCallback> m_componentsLoaders;
};
