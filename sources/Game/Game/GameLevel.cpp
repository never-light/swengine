#include "GameLevel.h"

#include <Engine/Modules/Graphics/GraphicsSystem/Mesh.h>
#include <Engine/Modules/Graphics/GraphicsSystem/Animation/Skeleton.h>

#include <Engine/Modules/Graphics/GraphicsSystem/TransformComponent.h>
#include <Engine/Modules/Graphics/GraphicsSystem/CameraComponent.h>
#include <Engine/Modules/Graphics/GraphicsSystem/MeshRendererComponent.h>
#include <Engine/Modules/Graphics/GraphicsSystem/Animation/SkeletalAnimationComponent.h>

#include <Engine/Modules/Graphics/GraphicsSystem/EnvironmentRenderingSystem.h>

#include <Engine/Modules/Graphics/Resources/MaterialResource.h>
#include <Engine/Modules/Graphics/Resources/SkeletonResource.h>
#include <Engine/Modules/Graphics/Resources/SkeletalAnimationResource.h>
#include <Engine/Modules/Graphics/Resources/AnimationStatesMachineResource.h>

#include <Engine/Modules/Physics/PhysicsSystem.h>
#include <Engine/Modules/Physics/Resources/CollisionDataResource.h>

#include <Engine/Modules/Audio/AudioSourceComponent.h>
#include <Engine/Modules/Audio/Resources/AudioClipResource.h>

#include <utility>

#include "Game/PlayerComponent.h"

GameLevel::GameLevel(std::shared_ptr<GameWorld> gameWorld,
  std::shared_ptr<GLGraphicsContext> graphicsContext,
  std::shared_ptr<ResourceManager> resourceManager)
  : m_gameWorld(std::move(gameWorld)),
    m_graphicsContext(std::move(graphicsContext)),
    m_resourceManager(std::move(resourceManager))
{
  m_player = m_gameWorld->createGameObject();
  auto& playerTransformComponent = m_player->addComponent<TransformComponent>();
  playerTransformComponent.getTransform().move(0.0f, 0.0f, 0.0f);

  std::shared_ptr<Camera> camera = std::make_shared<Camera>();
  camera->setNearClipDistance(0.1f);
  camera->setFarClipDistance(100.0f);
  camera->setAspectRatio(m_graphicsContext->getDefaultFramebuffer().getAspectRatio());
  camera->setFOVy(glm::pi<float>() / 4);

  camera->getTransform()->setPosition(0, 0, 0);
  camera->getTransform()->lookAt(0, 0, 1);

  auto& playerCameraComponent = m_player->addComponent<CameraComponent>();
  playerCameraComponent.setCamera(camera);

  std::shared_ptr<Skeleton> playerSkeleton =
    m_resourceManager->getResourceFromInstance<SkeletonResource>("player_skeleton")->getSkeleton();

  std::shared_ptr<Mesh> playerMesh =
    m_resourceManager->getResourceFromInstance<MeshResource>("player_mesh")->getMesh();

  playerMesh->setSkeleton(playerSkeleton);

  std::shared_ptr<Material> playerMaterial =
    m_resourceManager->getResourceFromInstance<MaterialResource>("player_material")->getMaterial();

  auto& playerMeshRendererComponent = m_player->addComponent<MeshRendererComponent>();
  playerMeshRendererComponent.setMeshInstance(playerMesh);
  playerMeshRendererComponent.setMaterialInstance(0, playerMaterial);
  playerMeshRendererComponent.getAttributes().isStatic = false;
  playerMeshRendererComponent.updateBounds(playerTransformComponent.getTransform().getTransformationMatrix());

  auto playerAnimationStatesMachine =
    m_resourceManager->getResourceFromInstance<AnimationStatesMachineResource>("player_animation_machine")->
      getMachine();

  auto& playerAnimationComponent = m_player->addComponent<SkeletalAnimationComponent>(playerSkeleton);
  playerAnimationComponent.setAnimationStatesMachine(playerAnimationStatesMachine);

  auto& playerComponent = m_player->addComponent<PlayerComponent>(playerMesh->getAABB().getSize().y);

  auto& playerKinematicCharacterComponent = m_player->addComponent<KinematicCharacterComponent>(
    CollisionShapesFactory::createCapsule(0.32f, playerComponent.getPlayerHeight() - 0.32f * 2));

  playerKinematicCharacterComponent.setOriginOffset({0.0f,
    playerComponent.getPlayerHeight() / 2.0f - 0.015f,
    0.0f});

  playerKinematicCharacterComponent.setTransform(playerTransformComponent.getTransform());

  auto& playerAudioSourceComponent = m_player->addComponent<AudioSourceComponent>(
    m_resourceManager->getResourceFromInstance<AudioClipResource>("audio_actions_footsteps_1")->getAudioClip()
  );

  playerAudioSourceComponent.getSource().setRelativeToListenerMode(false);
  playerAudioSourceComponent.getSource().setLooped(true);

  // Game objects
  std::shared_ptr<Material>
    material = m_resourceManager->getResourceFromInstance<MaterialResource>("deferred_gpass_brick")->
    getMaterial();

  {
    std::shared_ptr<GameObject> obj = m_gameWorld->createGameObject();
    auto& transformHandle = obj->addComponent<TransformComponent>();

    //transformHandle.getTransform().move(0.0f, 0.0f, 0.0f);
    //transformHandle.getTransform().setScale(0.01f, 0.01f, 0.01f);

    std::shared_ptr<Mesh>
      cubeGeometry = m_resourceManager->getResourceFromInstance<MeshResource>("ground_mesh")->getMesh();

    auto& componentHandle = obj->addComponent<MeshRendererComponent>();
    componentHandle.setMeshInstance(cubeGeometry);
    componentHandle.setMaterialsInstances({material});

    componentHandle.updateBounds(transformHandle.getTransform().getTransformationMatrix());

    std::shared_ptr<CollisionShape> groundCollisionShape =
      m_resourceManager->getResourceFromInstance<CollisionDataResource>("ground_mesh_collision")->getCollisionShape();

    obj->addComponent<RigidBodyComponent>(0.0f, groundCollisionShape);
  }

  // Environment
  {
    std::shared_ptr<GameObject> environmentObj = m_gameWorld->createGameObject();
    auto& environment = environmentObj->addComponent<EnvironmentComponent>();

    environment.setEnvironmentMaterial(
      m_resourceManager->getResourceFromInstance<MaterialResource>("test_scene_environment")->getMaterial());
  }

  // Background music

  {
    std::shared_ptr<GameObject> backgroundSoundSource = m_gameWorld->createGameObject();

    RETURN_VALUE_UNUSED(backgroundSoundSource->addComponent<TransformComponent>());

    auto& audioSourceComponent = backgroundSoundSource->addComponent<AudioSourceComponent>(
      m_resourceManager->getResourceFromInstance<AudioClipResource>("audio_background_1")->getAudioClip()
    );

    audioSourceComponent.getSource().setRelativeToListenerMode(true);
    audioSourceComponent.getSource().setVolume(0.2f);
    audioSourceComponent.getSource().play();
    audioSourceComponent.getSource().playOnce(m_resourceManager->getResourceFromInstance<AudioClipResource>("audio_actions_footsteps_1")->getAudioClip());
  }
}

std::shared_ptr<GameObject> GameLevel::getPlayer() const
{
  return m_player;
}
