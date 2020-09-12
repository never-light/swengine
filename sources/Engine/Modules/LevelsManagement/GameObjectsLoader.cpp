#include "precompiled.h"

#pragma hdrstop

#include "GameObjectsLoader.h"

#include <utility>

#include "Modules/Graphics/GraphicsSystem/TransformComponent.h"
#include "Modules/Graphics/GraphicsSystem/MeshRendererComponent.h"
#include "Modules/Graphics/GraphicsSystem/CameraComponent.h"
#include "Modules/Graphics/GraphicsSystem/EnvironmentRenderingSystem.h"
#include "Modules/Graphics/GraphicsSystem/Animation/AnimationStatesMachine.h"
#include "Modules/Graphics/GraphicsSystem/Animation/SkeletalAnimationComponent.h"

#include "Modules/Graphics/Resources/MaterialResource.h"
#include "Modules/Graphics/Resources/SkeletonResource.h"
#include "Modules/Graphics/Resources/AnimationStatesMachineResource.h"

#include "Modules/Physics/RigidBodyComponent.h"
#include "Modules/Physics/KinematicCharacterComponent.h"
#include "Modules/Physics/CollisionShapesFactory.h"

#include "Modules/Physics/Resources/CollisionDataResource.h"

#include "Modules/Audio/AudioSourceComponent.h"
#include "Modules/Audio/Resources/AudioClipResource.h"

#include "Modules/Math/MathUtils.h"
#include "Utility/files.h"

#include "Exceptions/exceptions.h"

GameObjectsLoader::GameObjectsLoader(std::shared_ptr<GameWorld> gameWorld,
  std::shared_ptr<ResourceManager> resourceManager)
  : m_gameWorld(std::move(gameWorld)),
    m_resourceManager(std::move(resourceManager))
{
  registerGenericComponentLoader("transform",
    [this](GameObject& object, const pugi::xml_node& objectNode) {
      loadTransformData(object, objectNode);
    });

  registerGenericComponentLoader("visual",
    [this](GameObject& object, const pugi::xml_node& objectNode) {
      loadVisualData(object, objectNode);
    });

  registerGenericComponentLoader("rigid_body",
    [this](GameObject& object, const pugi::xml_node& objectNode) {
      loadRigidBodyData(object, objectNode);
    });

  registerGenericComponentLoader("environment",
    [this](GameObject& object, const pugi::xml_node& objectNode) {
      loadEnvironmentData(object, objectNode);
    });

  registerGenericComponentLoader("audio_source",
    [this](GameObject& object, const pugi::xml_node& objectNode) {
      loadAudioSourceData(object, objectNode);
    });

  registerGenericComponentLoader("camera",
    [this](GameObject& object, const pugi::xml_node& objectNode) {
      loadCameraData(object, objectNode);
    });

  registerGenericComponentLoader("animation",
    [this](GameObject& object, const pugi::xml_node& objectNode) {
      loadAnimationData(object, objectNode);
    });

  registerGenericComponentLoader("kinematic_character",
    [this](GameObject& object, const pugi::xml_node& objectNode) {
      loadKinematicCharacterData(object, objectNode);
    });
}

GameObjectsLoader::~GameObjectsLoader() = default;

void GameObjectsLoader::registerClassLoader(const std::string& className,
  std::unique_ptr<GameObjectsClassLoader> loader)
{
  SW_ASSERT(!m_classesLoaders.contains(className));

  m_classesLoaders.insert({className, std::move(loader)});
}

void GameObjectsLoader::loadTransformData(GameObject& gameObject, const pugi::xml_node& data)
{
  auto& transformComponent = gameObject.addComponent<TransformComponent>();

  bool isStatic = data.attribute("static").as_bool(false);
  transformComponent.setStaticMode(isStatic);

  auto positionAttr = data.attribute("position");

  if (positionAttr) {
    transformComponent.getTransform().setPosition(StringUtils::stringToVec3(positionAttr.as_string()));
  }

  auto directionAttr = data.attribute("direction");

  if (directionAttr) {
    glm::vec3 forwardDirection = StringUtils::stringToVec3(directionAttr.as_string());

    transformComponent.getTransform().setOrientation(
      MathUtils::forwardUpToQuat(forwardDirection, MathUtils::AXIS_UP));
  }

  auto scaleAttr = data.attribute("scale");

  if (scaleAttr) {
    transformComponent.getTransform().setScale(StringUtils::stringToVec3(scaleAttr.as_string()));
  }
}

void GameObjectsLoader::loadVisualData(GameObject& gameObject, const pugi::xml_node& data)
{
  auto& transformComponent = gameObject.getComponent<TransformComponent>();

  auto& meshRendererComponent = gameObject.addComponent<MeshRendererComponent>();

  // TODO: remove isStatic option and use isStatic from TransformComponent
  meshRendererComponent.getAttributes().isStatic = transformComponent.isStatic();

  auto meshName = data.attribute("mesh").as_string();

  std::shared_ptr<Mesh> meshInstance =
    m_resourceManager->getResourceFromInstance<MeshResource>(meshName)->getMesh();

  auto skeletonAttr = data.attribute("skeleton");

  if (skeletonAttr) {
    std::shared_ptr<Skeleton> skeletonInstance =
      m_resourceManager->getResourceFromInstance<SkeletonResource>(skeletonAttr.as_string())->getSkeleton();

    meshInstance->setSkeleton(skeletonInstance);
  }

  meshRendererComponent.setMeshInstance(meshInstance);

  meshRendererComponent.updateBounds(transformComponent.getTransform().getTransformationMatrix());

  auto materialsData = data.child("materials");

  for (auto materialData : materialsData.children("material")) {
    auto materialName = materialData.attribute("id").as_string();
    int materialSubMeshIndex = materialData.attribute("index").as_int();

    std::shared_ptr<Material> materialInstance =
      m_resourceManager->getResourceFromInstance<MaterialResource>(materialName)->getMaterial();

    meshRendererComponent.setMaterialInstance(materialSubMeshIndex, materialInstance);
  }
}

void GameObjectsLoader::loadRigidBodyData(GameObject& gameObject, const pugi::xml_node& data)
{
  auto collisionModelName = data.attribute("collision_model").as_string();

  std::shared_ptr<CollisionShape> collisionShape =
    m_resourceManager->getResourceFromInstance<CollisionDataResource>(collisionModelName)->getCollisionShape();

  // Set zero mass to mark rigid body as static body
  auto& rigidBodyComponent = gameObject.addComponent<RigidBodyComponent>(0.0f, collisionShape);
  LOCAL_VALUE_UNUSED(rigidBodyComponent);
}

void GameObjectsLoader::registerGenericComponentLoader(const std::string& componentName,
  const GameObjectsLoader::ComponentLoaderCallback& loader)
{
  SW_ASSERT(!m_componentsLoaders.contains(componentName));

  m_componentsLoaders.insert({componentName, loader});
}

GameObjectsLoader::ComponentLoaderCallback GameObjectsLoader::getComponentLoader(const std::string& componentName) const
{
  return m_componentsLoaders.at(componentName);
}

ResourceManager& GameObjectsLoader::getResourceManager()
{
  return *m_resourceManager;
}

GameObject& GameObjectsLoader::loadGameObject(const pugi::xml_node& objectNode)
{
  std::string objectClassName = objectNode.attribute("class").as_string();
  auto& classLoader = m_classesLoaders.at(objectClassName);

  auto nameAttr = objectNode.attribute("id");

  std::shared_ptr<GameObject> gameObject;

  if (nameAttr) {
    if (m_gameWorld->findGameObject(nameAttr.as_string())) {
      THROW_EXCEPTION(EngineRuntimeException,
        fmt::format("Game object \"{}\" already exists", nameAttr.as_string()));
    }

    gameObject = m_gameWorld->createGameObject(nameAttr.as_string());
  }
  else {
    gameObject = m_gameWorld->createGameObject();
  }

  classLoader->loadGameObject(*gameObject, objectNode);

  return *gameObject;
}

void GameObjectsLoader::loadEnvironmentData(GameObject& gameObject, const pugi::xml_node& data)
{
  auto& environmentComponent = gameObject.addComponent<EnvironmentComponent>();

  auto materialName = data.attribute("material").as_string();

  std::shared_ptr<Material> materialInstance =
    m_resourceManager->getResourceFromInstance<MaterialResource>(materialName)->getMaterial();

  environmentComponent.setEnvironmentMaterial(materialInstance);
}

void GameObjectsLoader::loadAudioSourceData(GameObject& gameObject, const pugi::xml_node& data)
{
  auto audioClipName = data.attribute("clip").as_string();

  std::shared_ptr<AudioClip> audioClipInstance =
    m_resourceManager->getResourceFromInstance<AudioClipResource>(audioClipName)->getAudioClip();

  auto& audioSourceComponent = gameObject.addComponent<AudioSourceComponent>(audioClipInstance);
  auto& audioSource = audioSourceComponent.getSource();

  float volume = data.attribute("volume").as_float(1.0f);
  audioSource.setVolume(volume);

  float pitch = data.attribute("pitch").as_float(1.0f);
  audioSource.setPitch(pitch);

  bool isRelativeToListener = data.attribute("camera_relative").as_bool(false);
  audioSource.setRelativeToListenerMode(isRelativeToListener);

  bool isLooped = data.attribute("looped").as_bool(false);
  audioSource.setLooped(isLooped);

  auto positionAttr = data.attribute("position").as_string("0 0 0");
  audioSource.setPosition(StringUtils::stringToVec3(positionAttr));
}

void GameObjectsLoader::loadCameraData(GameObject& gameObject, const pugi::xml_node& data)
{
  std::shared_ptr<Camera> camera = std::make_shared<Camera>();

  auto transformNode = data.child("transform");

  if (transformNode) {
    glm::vec3 position = StringUtils::stringToVec3(transformNode.attribute("position").as_string("0 0 0"));
    camera->getTransform()->setPosition(position);

    auto lookAtAttr = transformNode.attribute("look_at");

    if (lookAtAttr) {
      glm::vec3 lookAt = StringUtils::stringToVec3(lookAtAttr.as_string("0 0 0"));
      camera->getTransform()->lookAt(lookAt);
    }
  }

  auto projectionNode = data.child("projection");

  if (projectionNode) {
    float nearDistance = projectionNode.attribute("near_dist").as_float(0.1f);
    camera->setNearClipDistance(nearDistance);

    float farDistance = projectionNode.attribute("far_dist").as_float(100.0f);
    camera->setFarClipDistance(farDistance);

    float fov = projectionNode.attribute("fov").as_float(45.0f);
    camera->setFOVy(glm::radians(fov));
  }

  auto& cameraComponent = gameObject.addComponent<CameraComponent>();
  cameraComponent.setCamera(camera);
}

void GameObjectsLoader::loadAnimationData(GameObject& gameObject, const pugi::xml_node& data)
{
  std::string skeletonName = data.attribute("skeleton").as_string();

  std::shared_ptr<Skeleton> skeletonInstance =
    m_resourceManager->getResourceFromInstance<SkeletonResource>(skeletonName)->getSkeleton();

  std::string stateMachineName = data.attribute("state_machine").as_string();

  auto animationStatesMachineInstance =
    m_resourceManager->getResourceFromInstance<AnimationStatesMachineResource>(stateMachineName)->
      getMachine();

  auto& animationComponent = gameObject.addComponent<SkeletalAnimationComponent>(skeletonInstance);
  animationComponent.setAnimationStatesMachine(animationStatesMachineInstance);
}

void GameObjectsLoader::loadKinematicCharacterData(GameObject& gameObject, const pugi::xml_node& data)
{
  auto collisionModelNode = data.child("collision_model");

  auto capsuleCollisionShapeNode = collisionModelNode.child("capsule");
  std::shared_ptr<CollisionShape> collisionShape;

  if (capsuleCollisionShapeNode) {
    float capsuleRadius = capsuleCollisionShapeNode.attribute("radius").as_float();
    float capsuleHeight = capsuleCollisionShapeNode.attribute("height").as_float();

    collisionShape = CollisionShapesFactory::createCapsule(capsuleRadius, capsuleHeight);
  }
  else {
    SW_ASSERT(false && "Only capsule collision shape is supported now");
  }

  auto& kinematicCharacterComponent =
    gameObject.addComponent<KinematicCharacterComponent>(collisionShape);

  auto originOffsetAttr = collisionModelNode.attribute("origin_offset");

  if (originOffsetAttr) {
    glm::vec3 originOffset = StringUtils::stringToVec3(originOffsetAttr.as_string());

    kinematicCharacterComponent.setOriginOffset(originOffset);
  }

  auto& objectTransform = gameObject.getComponent<TransformComponent>();
  kinematicCharacterComponent.setTransform(objectTransform.getTransform());
}
