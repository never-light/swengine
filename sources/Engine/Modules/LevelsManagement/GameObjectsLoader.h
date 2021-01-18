#pragma once

#include <utility>
#include <optional>

#include "Modules/ECS/ECS.h"
#include "Modules/ResourceManagement/ResourcesManagement.h"

#include "Utility/xml.h"

#include "GameObjectsClassLoader.h"

#include "Modules/Graphics/GraphicsSystem/TransformComponent.h"
#include "Modules/Graphics/GraphicsSystem/MeshRendererComponent.h"
#include "Modules/Graphics/GraphicsSystem/CameraComponent.h"
#include "Modules/Graphics/GraphicsSystem/EnvironmentRenderingSystem.h"
#include "Modules/Graphics/GraphicsSystem/Animation/AnimationStatesMachine.h"
#include "Modules/Graphics/GraphicsSystem/Animation/SkeletalAnimationComponent.h"
#include "Modules/Physics/RigidBodyComponent.h"
#include "Modules/Physics/KinematicCharacterComponent.h"
#include "Modules/Audio/AudioSourceComponent.h"

class GameObjectSpawnComponentBindingParameters {
 public:
  std::string spawnName;

  template<class Archive>
  void serialize(Archive& archive)
  {
    archive(
      cereal::make_nvp("spawn_name", spawnName));
  };

};

struct GameObjectSpawnComponent {
 public:
  static constexpr bool s_isSerializable = true;
  using BindingParameters = GameObjectSpawnComponentBindingParameters;

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

  [[nodiscard]] BindingParameters getBindingParameters() const
  {
    return GameObjectSpawnComponentBindingParameters{.spawnName=m_spawnName};
  }

 private:
  std::string m_spawnName;
};

class GameObjectSpawnComponentBinder : public GameObjectsComponentBinder<GameObjectSpawnComponent> {
 public:
  explicit GameObjectSpawnComponentBinder(const ComponentBindingParameters& componentParameters)
    : m_bindingParameters(componentParameters)
  {

  }

  void bindToObject(GameObject& gameObject) override
  {
    auto spawnComponent = gameObject.addComponent<GameObjectSpawnComponent>();
    spawnComponent->setSpawnName(m_bindingParameters.spawnName);
  }

 private:
  ComponentBindingParameters m_bindingParameters;
};

struct GameObjectFactoryData {
  GameObjectFactoryData(std::optional<std::string> gameObjectId,
    std::unordered_map<std::string, std::unique_ptr<BaseGameObjectsComponentBinder>> componentsFactories)
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
  std::unordered_map<std::string, std::unique_ptr<BaseGameObjectsComponentBinder>> componentsFactories;
};

class GameObjectsLoader {
 public:
  using ComponentLoaderCallback = std::function<std::unique_ptr<BaseGameObjectsComponentBinder>(const pugi::xml_node&)>;

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
  std::unique_ptr<BaseGameObjectsComponentBinder> loadTransformData(const pugi::xml_node& data);
  std::unique_ptr<BaseGameObjectsComponentBinder> loadVisualData(const pugi::xml_node& data);
  std::unique_ptr<BaseGameObjectsComponentBinder> loadRigidBodyData(const pugi::xml_node& data);
  std::unique_ptr<BaseGameObjectsComponentBinder> loadEnvironmentData(const pugi::xml_node& data);
  std::unique_ptr<BaseGameObjectsComponentBinder> loadAudioSourceData(const pugi::xml_node& data);
  std::unique_ptr<BaseGameObjectsComponentBinder> loadCameraData(const pugi::xml_node& data);
  std::unique_ptr<BaseGameObjectsComponentBinder> loadAnimationData(const pugi::xml_node& data);
  std::unique_ptr<BaseGameObjectsComponentBinder> loadKinematicCharacterData(const pugi::xml_node& data);

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
