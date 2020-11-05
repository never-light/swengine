#include "precompiled.h"

#pragma hdrstop

#include "GameObjectsComponentsBinders.h"

#include <utility>

#include "Modules/Math/MathUtils.h"

#include "Modules/Graphics/GraphicsSystem/TransformComponent.h"
#include "Modules/Graphics/GraphicsSystem/CameraComponent.h"
#include "Modules/Graphics/GraphicsSystem/MeshRendererComponent.h"
#include "Modules/Graphics/GraphicsSystem/Animation/SkeletalAnimationComponent.h"
#include "Modules/Graphics/GraphicsSystem/EnvironmentRenderingSystem.h"
#include "Modules/Physics/KinematicCharacterComponent.h"
#include "Modules/Physics/RigidBodyComponent.h"
#include "Modules/Audio/AudioSourceComponent.h"

TransformComponentBinder::TransformComponentBinder(const TransformComponentBindingParameters& componentParameters)
  : GameObjectsComponentParametricBinder<TransformComponentBindingParameters>(componentParameters)
{

}

void TransformComponentBinder::bindToObject(GameObject& gameObject)
{
  auto& bindingParameters = getComponentParameters();
  auto& transformComponent = *gameObject.addComponent<TransformComponent>().get();

  transformComponent.setStaticMode(bindingParameters.isStatic());
  transformComponent.getTransform().setPosition(bindingParameters.getPosition());
  transformComponent.getTransform().setOrientation(
    MathUtils::forwardUpToQuat(bindingParameters.getDirection(), MathUtils::AXIS_UP));
  transformComponent.getTransform().setScale(bindingParameters.getScale());
}

CameraComponentBinder::CameraComponentBinder(const CameraComponentBindingParameters& componentParameters)
  : GameObjectsComponentParametricBinder<CameraComponentBindingParameters>(componentParameters)
{

}

void CameraComponentBinder::bindToObject(GameObject& gameObject)
{
  auto& bindingParameters = getComponentParameters();
  std::shared_ptr<Camera> camera = std::make_shared<Camera>();

  camera->getTransform()->setPosition(bindingParameters.getPosition());
  camera->getTransform()->lookAt(bindingParameters.getLookAtPoint());

  camera->setNearClipDistance(bindingParameters.getNearDistance());
  camera->setFarClipDistance(bindingParameters.getFarDistance());
  camera->setFOVy(glm::radians(bindingParameters.getFov()));

  auto& cameraComponent = *gameObject.addComponent<CameraComponent>().get();
  cameraComponent.setCamera(camera);
}

MeshRendererComponentBinder::MeshRendererComponentBinder(const MeshRendererComponentBindingParameters& componentParameters,
  std::shared_ptr<ResourcesManager> resourcesManager)
  : GameObjectsComponentParametricBinder<MeshRendererComponentBindingParameters>(componentParameters),
    m_resourcesManager(std::move(resourcesManager))
{

}

void MeshRendererComponentBinder::bindToObject(GameObject& gameObject)
{
  auto& bindingParameters = getComponentParameters();

  auto& transformComponent = *gameObject.getComponent<TransformComponent>().get();
  auto& meshRendererComponent = *gameObject.addComponent<MeshRendererComponent>().get();

  ResourceHandle<Mesh> meshInstance =
    m_resourcesManager->getResource<Mesh>(bindingParameters.getMeshResourceName());

  const std::string& skeletonResourceName = bindingParameters.getSkeletonResourceName();

  if (!skeletonResourceName.empty()) {
    meshInstance->setSkeleton(m_resourcesManager->getResource<Skeleton>(skeletonResourceName));
  }

  meshRendererComponent.setMeshInstance(meshInstance);

  transformComponent.setBounds(meshInstance->getAABB());
  transformComponent.updateBounds(transformComponent.getTransform().getTransformationMatrix());

  for (auto&[materialName, subMeshIndex] : bindingParameters.getMaterials()) {
    ResourceHandle<GLMaterial> materialInstance =
      m_resourcesManager->getResource<GLMaterial>(materialName);

    meshRendererComponent.setMaterialInstance(subMeshIndex, materialInstance);
  }
}

AnimationComponentBinder::AnimationComponentBinder(const AnimationComponentBindingParameters& componentParameters,
  std::shared_ptr<ResourcesManager> resourcesManager)
  : GameObjectsComponentParametricBinder<AnimationComponentBindingParameters>(componentParameters),
    m_resourcesManager(std::move(resourcesManager))
{

}

void AnimationComponentBinder::bindToObject(GameObject& gameObject)
{
  auto& bindingParameters = getComponentParameters();

  auto skeletonInstance = m_resourcesManager->getResource<Skeleton>(bindingParameters.getSkeletonResourceName());
  auto statesMachineInstance = m_resourcesManager->getResource<AnimationStatesMachine>(
    bindingParameters.getStateMachineResourceName());

  gameObject.addComponent<SkeletalAnimationComponent>(statesMachineInstance);

  const std::string& initialStateMachineStateName = bindingParameters.getStateMachineInitialState();

  if (!initialStateMachineStateName.empty()) {
    statesMachineInstance->setActiveState(initialStateMachineStateName);
  }
}

KinematicCharacterComponentBinder::KinematicCharacterComponentBinder(
  const KinematicCharacterComponentBindingParameters& componentParameters,
  std::shared_ptr<ResourcesManager> resourcesManager)
  : GameObjectsComponentParametricBinder<KinematicCharacterComponentBindingParameters>(componentParameters),
    m_resourcesManager(std::move(resourcesManager))
{

}

void KinematicCharacterComponentBinder::bindToObject(GameObject& gameObject)
{
  auto& bindingParameters = getComponentParameters();

  ResourceHandle<CollisionShape> collisionShape = m_resourcesManager->createResourceInPlace<CollisionShape>(
    CollisionShapeCapsule(bindingParameters.getCapsuleRadius(), bindingParameters.getCapsuleHeight()));

  auto& kinematicCharacterComponent =
    *gameObject.addComponent<KinematicCharacterComponent>(collisionShape).get();

  kinematicCharacterComponent.setOriginOffset(bindingParameters.getOriginOffset());

  auto& objectTransform = *gameObject.getComponent<TransformComponent>().get();
  kinematicCharacterComponent.setTransform(objectTransform.getTransform());
}

RigidBodyComponentBinder::RigidBodyComponentBinder(const RigidBodyComponentBindingParameters& componentParameters,
  std::shared_ptr<ResourcesManager> resourcesManager)
  : GameObjectsComponentParametricBinder<RigidBodyComponentBindingParameters>(componentParameters),
    m_resourcesManager(std::move(resourcesManager))
{

}

void RigidBodyComponentBinder::bindToObject(GameObject& gameObject)
{
  auto& bindingParameters = getComponentParameters();

  const std::string& collisionModelName = bindingParameters.getCollisionModelResourceName();

  ResourceHandle<CollisionShape> collisionShape;

  auto transformComponent = gameObject.getComponent<TransformComponent>();

  if (collisionModelName == "visual_aabb") {
    collisionShape = m_resourcesManager->createResourceInPlace<CollisionShape>(
      CollisionShapeBox(transformComponent->getOriginalBounds().getSize() * 0.5f));
  }
  else if (collisionModelName == "visual_sphere") {
    collisionShape = m_resourcesManager->createResourceInPlace<CollisionShape>(
      CollisionShapeSphere(transformComponent->getOriginalBounds().toSphere().getRadius()));
  }
  else {
    collisionShape = m_resourcesManager->getResource<CollisionShape>(collisionModelName);
  }

  RigidBodyComponent* rigidBodyComponent;

  if (transformComponent->isStatic()) {
    SW_ASSERT(MathUtils::isZero(bindingParameters.getMass()));

    // Set zero mass to mark rigid body as static body
    rigidBodyComponent = gameObject.addComponent<RigidBodyComponent>(0.0f, collisionShape).get();
  }
  else {
    rigidBodyComponent = gameObject.addComponent<RigidBodyComponent>(bindingParameters.getMass(), collisionShape).get();
  }

  LOCAL_VALUE_UNUSED(rigidBodyComponent);
}

AudioSourceComponentBinder::AudioSourceComponentBinder(const AudioSourceComponentBindingParameters& componentParameters,
  std::shared_ptr<ResourcesManager> resourcesManager)
  : GameObjectsComponentParametricBinder<AudioSourceComponentBindingParameters>(componentParameters),
    m_resourcesManager(std::move(resourcesManager))
{

}

void AudioSourceComponentBinder::bindToObject(GameObject& gameObject)
{
  auto& bindingParameters = getComponentParameters();

  ResourceHandle<AudioClip> audioClipInstance =
    m_resourcesManager->getResource<AudioClip>(bindingParameters.getClipResourceName());

  auto& audioSourceComponent = *gameObject.addComponent<AudioSourceComponent>(audioClipInstance).get();
  auto& audioSource = audioSourceComponent.getSource();

  audioSource.setVolume(bindingParameters.getVolume());
  audioSource.setPitch(bindingParameters.getPitch());
  audioSource.setRelativeToListenerMode(bindingParameters.isCameraRelative());
  audioSource.setLooped(bindingParameters.isLooped());

  audioSource.setPosition(bindingParameters.getPosition());
}

EnvironmentComponentBinder::EnvironmentComponentBinder(const EnvironmentComponentBindingParameters& componentParameters,
  std::shared_ptr<ResourcesManager> resourcesManager)
  : GameObjectsComponentParametricBinder<EnvironmentComponentBindingParameters>(componentParameters),
    m_resourcesManager(std::move(resourcesManager))
{

}

void EnvironmentComponentBinder::bindToObject(GameObject& gameObject)
{
  auto& bindingParameters = getComponentParameters();

  auto& environmentComponent = *gameObject.addComponent<EnvironmentComponent>().get();

  ResourceHandle<GLMaterial> materialInstance =
    m_resourcesManager->getResource<GLMaterial>(bindingParameters.getMaterialResourceName());

  environmentComponent.setEnvironmentMaterial(materialInstance);
}
