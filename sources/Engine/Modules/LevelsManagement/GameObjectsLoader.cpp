#include "precompiled.h"

#pragma hdrstop

#include "GameObjectsLoader.h"

#include <utility>

#include "Modules/Graphics/Resources/MaterialResourceManager.h"
#include "Modules/Graphics/Resources/SkeletonResourceManager.h"
#include "Modules/Graphics/Resources/AnimationStatesMachineResourceManager.h"

#include "Modules/Physics/CollisionShapesFactory.h"
#include "Modules/Physics/Resources/CollisionShapeResourceManager.h"
#include "Modules/Audio/Resources/AudioClipResourceManager.h"

#include "Modules/Math/MathUtils.h"
#include "Utility/files.h"

#include "Exceptions/exceptions.h"

GameObjectsLoader::GameObjectsLoader(std::shared_ptr<GameWorld> gameWorld,
  std::shared_ptr<ResourcesManager> resourceManager)
  : m_gameWorld(std::move(gameWorld)),
    m_resourceManager(std::move(resourceManager))
{
  registerComponentName<TransformComponent>("transform");
  registerComponentName<MeshRendererComponent>("visual");
  registerComponentName<RigidBodyComponent>("rigid_body");
  registerComponentName<EnvironmentComponent>("environment");
  registerComponentName<AudioSourceComponent>("audio_source");
  registerComponentName<CameraComponent>("camera");
  registerComponentName<SkeletalAnimationComponent>("animation");
  registerComponentName<KinematicCharacterComponent>("kinematic_character");

  registerGenericComponentLoader<TransformComponent>(
    [this](const pugi::xml_node& objectNode) {
      return loadTransformData(objectNode);
    });

  registerGenericComponentLoader<MeshRendererComponent>(
    [this](const pugi::xml_node& objectNode) {
      return loadVisualData(objectNode);
    });

  registerGenericComponentLoader<RigidBodyComponent>(
    [this](const pugi::xml_node& objectNode) {
      return loadRigidBodyData(objectNode);
    });

  registerGenericComponentLoader<EnvironmentComponent>(
    [this](const pugi::xml_node& objectNode) {
      return loadEnvironmentData(objectNode);
    });

  registerGenericComponentLoader<AudioSourceComponent>(
    [this](const pugi::xml_node& objectNode) {
      return loadAudioSourceData(objectNode);
    });

  registerGenericComponentLoader<CameraComponent>(
    [this](const pugi::xml_node& objectNode) {
      return loadCameraData(objectNode);
    });

  registerGenericComponentLoader<SkeletalAnimationComponent>(
    [this](const pugi::xml_node& objectNode) {
      return loadAnimationData(objectNode);
    });

  registerGenericComponentLoader<KinematicCharacterComponent>(
    [this](const pugi::xml_node& objectNode) {
      return loadKinematicCharacterData(objectNode);
    });
}

GameObjectsLoader::~GameObjectsLoader() = default;

void GameObjectsLoader::registerClassLoader(const std::string& className,
  std::unique_ptr<GameObjectsClassLoader> loader)
{
  SW_ASSERT(!m_classesLoaders.contains(className));

  m_classesLoaders.insert({className, std::move(loader)});
}

std::unique_ptr<BaseGameObjectsComponentBinder> GameObjectsLoader::loadTransformData(const pugi::xml_node& data)
{
  TransformComponentBindingParameters bindingParameters;

  bool isStatic = data.attribute("static").as_bool(false);
  bindingParameters.isStatic = isStatic;

  auto positionAttr = data.attribute("position");

  if (positionAttr) {
    bindingParameters.position = StringUtils::stringToVec3(positionAttr.as_string());
  }

  auto directionAttr = data.attribute("direction");

  if (directionAttr) {
    glm::vec3 forwardDirection = StringUtils::stringToVec3(directionAttr.as_string());

    bindingParameters.frontDirection = forwardDirection;
  }

  auto scaleAttr = data.attribute("scale");

  if (scaleAttr) {
    bindingParameters.scale = StringUtils::stringToVec3(scaleAttr.as_string());
  }

  return std::make_unique<TransformComponentBinder>(bindingParameters);
}

std::unique_ptr<BaseGameObjectsComponentBinder> GameObjectsLoader::loadVisualData(const pugi::xml_node& data)
{
  MeshRendererComponentBindingParameters bindingParameters;

  auto meshName = data.attribute("mesh").as_string();
  bindingParameters.meshResourceName = meshName;

  auto skeletonAttr = data.attribute("skeleton");

  if (skeletonAttr) {
    bindingParameters.skeletonResourceName = skeletonAttr.as_string();
  }

  auto materialsData = data.child("materials");

  for (auto materialData : materialsData.children("material")) {
    auto materialName = materialData.attribute("id").as_string();
    int materialSubMeshIndex = materialData.attribute("index").as_int();

    bindingParameters.materials.emplace_back(materialName, materialSubMeshIndex);
  }

  return std::make_unique<MeshRendererComponentBinder>(bindingParameters, m_resourceManager);
}

std::unique_ptr<BaseGameObjectsComponentBinder> GameObjectsLoader::loadRigidBodyData(const pugi::xml_node& data)
{
  RigidBodyComponentBindingParameters bindingParameters;

  std::string collisionModelName = data.attribute("collision_model").as_string();
  bindingParameters.collisionModelResourceName = collisionModelName;
  bindingParameters.mass = data.attribute("mass").as_float();

  return std::make_unique<RigidBodyComponentBinder>(bindingParameters, m_resourceManager);
}

void GameObjectsLoader::registerGenericComponentLoader(const std::string& componentName,
  const GameObjectsLoader::ComponentLoaderCallback& loader)
{
  SW_ASSERT(!m_componentsLoaders.contains(componentName));

  m_componentsLoaders.insert({componentName, loader});
  m_registeredComponentsList.push_back(componentName);
}

GameObjectsLoader::ComponentLoaderCallback GameObjectsLoader::getComponentLoader(const std::string& componentName) const
{
  if (!m_componentsLoaders.contains(componentName)) {
    THROW_EXCEPTION(EngineRuntimeException,
      fmt::format("Resource loader for component {} is not found", componentName));
  }

  return m_componentsLoaders.at(componentName);
}

ResourcesManager& GameObjectsLoader::getResourceManager()
{
  return *m_resourceManager;
}

std::string GameObjectsLoader::loadGameObject(const pugi::xml_node& objectNode)
{
  std::string objectClassName = objectNode.attribute("class").as_string();
  auto& classLoader = m_classesLoaders.at(objectClassName);

  auto spawnNameAttr = objectNode.attribute("spawn_name");

  if (!spawnNameAttr) {
    THROW_EXCEPTION(EngineRuntimeException, "Game object should have spawn_name attribute");
  }

  std::string spawnName = spawnNameAttr.value();

  if (m_gameObjectsComponentsFactories.contains(spawnName)) {
    THROW_EXCEPTION(EngineRuntimeException,
      fmt::format("Object with spawn name \"{}\" already exists", spawnName));
  }

  std::optional<std::string> gameObjectId;

  auto objectIdAttr = objectNode.attribute("id");

  if (objectIdAttr) {
    gameObjectId = objectIdAttr.as_string();
  }

  auto componentsBinders = classLoader->loadGameObject(objectNode);

  m_gameObjectsComponentsFactories.insert({spawnName,
    GameObjectFactoryData(std::move(gameObjectId), std::move(componentsBinders))});

  return spawnName;
}

std::unique_ptr<BaseGameObjectsComponentBinder> GameObjectsLoader::loadEnvironmentData(const pugi::xml_node& data)
{
  EnvironmentComponentBindingParameters bindingParameters;

  auto materialName = data.attribute("material").as_string();
  bindingParameters.materialResourceName = materialName;

  return std::make_unique<EnvironmentComponentBinder>(bindingParameters, m_resourceManager);
}

std::unique_ptr<BaseGameObjectsComponentBinder> GameObjectsLoader::loadAudioSourceData(const pugi::xml_node& data)
{
  AudioSourceComponentBindingParameters bindingParameters;

  auto audioClipName = data.attribute("clip").as_string();
  bindingParameters.clipResourceName = audioClipName;

  float volume = data.attribute("volume").as_float(1.0f);
  bindingParameters.volume = volume;

  float pitch = data.attribute("pitch").as_float(1.0f);
  bindingParameters.pitch = pitch;

  bool isRelativeToListener = data.attribute("camera_relative").as_bool(false);
  bindingParameters.cameraRelative = isRelativeToListener;

  bool isLooped = data.attribute("looped").as_bool(false);
  bindingParameters.isLooped = isLooped;

  auto positionAttr = data.attribute("position").as_string("0 0 0");
  bindingParameters.position = StringUtils::stringToVec3(positionAttr);

  return std::make_unique<AudioSourceComponentBinder>(bindingParameters, m_resourceManager);
}

std::unique_ptr<BaseGameObjectsComponentBinder> GameObjectsLoader::loadCameraData(
  const pugi::xml_node& data)
{
  CameraComponentBindingParameters bindingParameters;

  auto transformNode = data.child("transform");

  if (transformNode) {
    glm::vec3 position = StringUtils::stringToVec3(transformNode.attribute("position").as_string("0 0 0"));
    bindingParameters.position = position;

    auto lookAtAttr = transformNode.attribute("look_at");

    if (lookAtAttr) {
      glm::vec3 lookAt = StringUtils::stringToVec3(lookAtAttr.as_string("0 0 0"));
      bindingParameters.lookAtPoint = lookAt;
    }
  }

  auto projectionNode = data.child("projection");

  if (projectionNode) {
    float nearDistance = projectionNode.attribute("near_dist").as_float(0.1f);
    bindingParameters.nearDistance = nearDistance;

    float farDistance = projectionNode.attribute("far_dist").as_float(100.0f);
    bindingParameters.farDistance = farDistance;

    float fov = projectionNode.attribute("fov").as_float(45.0f);
    bindingParameters.fov = fov;
  }

  return std::make_unique<CameraComponentBinder>(bindingParameters);
}

std::unique_ptr<BaseGameObjectsComponentBinder> GameObjectsLoader::loadAnimationData(
  const pugi::xml_node& data)
{
  AnimationComponentBindingParameters bindingParameters;

  std::string skeletonName = data.attribute("skeleton").as_string();
  bindingParameters.skeletonResourceName = skeletonName;

  std::string stateMachineName = data.attribute("state_machine").as_string();
  bindingParameters.stateMachineResourceName = stateMachineName;

  /*bool sharedStatesMachine = data.attribute("shared_state").as_bool(false);
  std::shared_ptr<AnimationStatesMachine> statesMachineInstance;

  if (sharedStatesMachine) {
    statesMachineInstance = sharedStatesMachineInstance;
  }
  else {
    statesMachineInstance =
      std::make_shared<AnimationStatesMachine>(AnimationStatesMachine(*sharedStatesMachineInstance));
  }*/

  auto startStateAttribute = data.attribute("start_state");

  if (startStateAttribute) {
    bindingParameters.stateMachineInitialState = startStateAttribute.as_string();
  }

  return std::make_unique<AnimationComponentBinder>(bindingParameters, m_resourceManager);
}

std::unique_ptr<BaseGameObjectsComponentBinder> GameObjectsLoader::loadKinematicCharacterData(
  const pugi::xml_node& data)
{
  KinematicCharacterComponentBindingParameters bindingParameters;

  auto collisionModelNode = data.child("collision_model");

  auto capsuleCollisionShapeNode = collisionModelNode.child("capsule");

  if (capsuleCollisionShapeNode) {
    float capsuleRadius = capsuleCollisionShapeNode.attribute("radius").as_float();
    bindingParameters.capsuleRadius = capsuleRadius;

    float capsuleHeight = capsuleCollisionShapeNode.attribute("height").as_float();
    bindingParameters.capsuleHeight = capsuleHeight;
  }
  else {
    SW_ASSERT(false && "Only capsule collision shape is supported now");
  }

  auto originOffsetAttr = collisionModelNode.attribute("origin_offset");

  if (originOffsetAttr) {
    glm::vec3 originOffset = StringUtils::stringToVec3(originOffsetAttr.as_string());
    bindingParameters.originOffset = originOffset;
  }

  return std::make_unique<KinematicCharacterComponentBinder>(bindingParameters, m_resourceManager);
}

GameObject GameObjectsLoader::buildGameObject(const std::string& spawnName,
  const std::optional<std::string>& objectName)
{
  if (!m_gameObjectsComponentsFactories.contains(spawnName)) {
    THROW_EXCEPTION(EngineRuntimeException,
      fmt::format("Failed to spawn object: object \"{}\" does not exists", spawnName));
  }

  auto& gameObjectFactoryData = m_gameObjectsComponentsFactories.at(spawnName);

  std::string gameObjectName;

  if (objectName.has_value()) {
    gameObjectName = objectName.value();
  }
  else {
    if (gameObjectFactoryData.gameObjectId.has_value()) {
      gameObjectName = gameObjectFactoryData.gameObjectId.value();
    }
    else {
      gameObjectName = fmt::format("spawn_{}_{}", spawnName, m_freeSpawnNameIndex++);
    }
  }

  GameObject gameObject = m_gameWorld->createGameObject(gameObjectName);

  auto spawnComponent = gameObject.addComponent<GameObjectSpawnComponent>();
  spawnComponent->setSpawnName(spawnName);

  // Iterate over components according to their loaders registration order
  // to preserve specified initialization order
  for (auto& componentName : m_registeredComponentsList) {
    auto componentBuilderIt = gameObjectFactoryData.componentsFactories.find(componentName);

    if (componentBuilderIt != gameObjectFactoryData.componentsFactories.end()) {
      componentBuilderIt->second->bindToObject(gameObject);
    }
  }

  return gameObject;
}
