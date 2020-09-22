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
  ResourceManager& getResourceManager();

  GameObject loadGameObject(const pugi::xml_node& objectNode);

  void loadGameObjectComponent(const pugi::xml_node& objectNode, const std::string& componentName, GameObject object);

  template<class T>
  void loadGameObjectComponent(const pugi::xml_node& objectNode, GameObject object)
  {
    loadGameObjectComponent(objectNode, getComponentName<T>(), object);
  }

 public:
  void loadTransformData(GameObject& gameObject, const pugi::xml_node& data);
  void loadVisualData(GameObject& gameObject, const pugi::xml_node& data);
  void loadRigidBodyData(GameObject& gameObject, const pugi::xml_node& data);
  void loadEnvironmentData(GameObject& gameObject, const pugi::xml_node& data);
  void loadAudioSourceData(GameObject& gameObject, const pugi::xml_node& data);
  void loadCameraData(GameObject& gameObject, const pugi::xml_node& data);
  void loadAnimationData(GameObject& gameObject, const pugi::xml_node& data);
  void loadKinematicCharacterData(GameObject& gameObject, const pugi::xml_node& data);

 private:
  std::shared_ptr<GameWorld> m_gameWorld;
  std::shared_ptr<ResourceManager> m_resourceManager;

  std::unordered_map<std::string, std::unique_ptr<GameObjectsClassLoader>> m_classesLoaders;
  std::unordered_map<std::string, ComponentLoaderCallback> m_componentsLoaders;

  std::vector<std::string> m_componentsNames = std::vector<std::string>(GameObjectData::MAX_COMPONENTS_COUNT);
};
