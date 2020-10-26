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
  std::shared_ptr<ResourcesManager> resourceManager,
  std::shared_ptr<LevelsManager> levelsManager)
  : m_gameWorld(std::move(gameWorld)),
    m_graphicsContext(std::move(graphicsContext)),
    m_resourceManager(std::move(resourceManager)),
    m_levelsManager(std::move(levelsManager))
{
  m_levelsManager->loadLevel("city");

  m_player = m_gameWorld->findGameObject("player");

  m_player.getComponent<CameraComponent>()->getCamera()->setAspectRatio(
    m_graphicsContext->getDefaultFramebuffer().getAspectRatio());

  auto environmentObject = m_gameWorld->findGameObject("city_environment");
  environmentObject.getComponent<AudioSourceComponent>()->getSource().play();
}

GameObject GameLevel::getPlayer() const
{
  return m_player;
}
