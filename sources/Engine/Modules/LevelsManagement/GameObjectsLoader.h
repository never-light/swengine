#pragma once

#include <utility>
#include <optional>

#include "Modules/ECS/ECS.h"
#include "Modules/ResourceManagement/ResourcesManagement.h"

#include "Utility/xml.h"

#include "GameObjectsClassLoader.h"
#include "GameObjectsComponentBinder.h"

struct GameObjectSpawnComponent {
 public:
  GameObjectSpawnComponent() = default;

  [[nodiscard]] const std::string& getSpawnName() const
  {
    return m_spawnName;
  }

  void setSpawnName(const std::string& spawnName)
  {
    m_spawnName = spawnName;
  }

 private:
  std::string m_spawnName;
};

struct GameObjectFactoryData {
  GameObjectFactoryData(std::optional<std::string> gameObjectId,
    std::unordered_map<std::string, std::unique_ptr<GameObjectsComponentBinder>> componentsFactories)
    : gameObjectId(std::move(gameObjectId)),
      componentsFactories(std::move(componentsFactories))
  {

  }

  GameObjectFactoryData(GameObjectFactoryData& other) = delete;

  GameObjectFactoryData(GameObjectFactoryData&& other) noexcept
    : gameObjectId(std::move(other.gameObjectId)),
      componentsFactories(std::move(other.componentsFactories))
  {

  }

  std::optional<std::string> gameObjectId;
  std::unordered_map<std::string, std::unique_ptr<GameObjectsComponentBinder>> componentsFactories;
};

class GameObjectsLoader {
 public:
  using ComponentLoaderCallback = std::function<std::unique_ptr<GameObjectsComponentBinder>(const pugi::xml_node&)>;

 public:
  GameObjectsLoader(std::shared_ptr<GameWorld> gameWorld,
    std::shared_ptr<ResourcesManager> resourceManager);
  ~GameObjectsLoader();

  template<class T>
  void registerComponentName(const std::string& name)
  {
    size_t typeId = ComponentsTypeInfo::getTypeIndex<T>();

    SW_ASSERT(m_componentsNames[typeId].empty() && "Component name is already registered");

    m_componentsNames[typeId] = name;
  }

  template<class T>
  [[nodiscard]] inline const std::string& getComponentName() const
  {
    size_t typeId = ComponentsTypeInfo::getTypeIndex<T>();

    SW_ASSERT(!m_componentsNames[typeId].empty() && "Component name is not registered");

    return m_componentsNames[typeId];
  }

  template<class T>
  inline void registerGenericComponentLoader(const ComponentLoaderCallback& loader)
  {
    registerGenericComponentLoader(getComponentName<T>(), loader);
  }

  void registerClassLoader(const std::string& className, std::unique_ptr<GameObjectsClassLoader> loader);
  void registerGenericComponentLoader(const std::string& componentName, const ComponentLoaderCallback& loader);

  [[nodiscard]] ComponentLoaderCallback getComponentLoader(const std::string& componentName) const;
  ResourcesManager& getResourceManager();

  std::string loadGameObject(const pugi::xml_node& objectNode);

  template<class T>
  void buildGameObjectComponent(GameObject object)
  {
    std::string componentName = getComponentName<T>();
    std::string spawnName = object.getComponent<GameObjectSpawnComponent>()->getSpawnName();

    SW_ASSERT(m_gameObjectsComponentsFactories.at(spawnName).componentsFactories.contains(componentName));

    auto& gameObjectFactoryData = m_gameObjectsComponentsFactories.at(spawnName);

    for (auto&[componentBuilderName, componentBuilder] : gameObjectFactoryData.componentsFactories) {
      if (componentBuilderName == componentName) {
        componentBuilder->bindToObject(object);
      }
    }
  }

  GameObject buildGameObject(const std::string& spawnName, const std::optional<std::string>& objectName = {});

 public:
  std::unique_ptr<GameObjectsComponentBinder> loadTransformData(const pugi::xml_node& data);
  std::unique_ptr<GameObjectsComponentBinder> loadVisualData(const pugi::xml_node& data);
  std::unique_ptr<GameObjectsComponentBinder> loadRigidBodyData(const pugi::xml_node& data);
  std::unique_ptr<GameObjectsComponentBinder> loadEnvironmentData(const pugi::xml_node& data);
  std::unique_ptr<GameObjectsComponentBinder> loadAudioSourceData(const pugi::xml_node& data);
  std::unique_ptr<GameObjectsComponentBinder> loadCameraData(const pugi::xml_node& data);
  std::unique_ptr<GameObjectsComponentBinder> loadAnimationData(const pugi::xml_node& data);
  std::unique_ptr<GameObjectsComponentBinder> loadKinematicCharacterData(const pugi::xml_node& data);

 private:
  std::shared_ptr<GameWorld> m_gameWorld;
  std::shared_ptr<ResourcesManager> m_resourceManager;

  std::unordered_map<std::string, std::unique_ptr<GameObjectsClassLoader>> m_classesLoaders;
  std::unordered_map<std::string, ComponentLoaderCallback> m_componentsLoaders;
  std::vector<std::string> m_registeredComponentsList;

  std::unordered_map<std::string, GameObjectFactoryData> m_gameObjectsComponentsFactories;

  std::vector<std::string> m_componentsNames = std::vector<std::string>(GameObjectData::MAX_COMPONENTS_COUNT);

  size_t m_freeSpawnNameIndex = 0;
};
