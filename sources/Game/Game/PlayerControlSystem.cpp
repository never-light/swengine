#include "PlayerControlSystem.h"

#include <Engine/Modules/Graphics/GraphicsSystem/TransformComponent.h>
#include <Engine/Modules/Graphics/GraphicsSystem/MeshRendererComponent.h>
#include <Engine/Modules/Graphics/GraphicsSystem/Animation/SkeletalAnimationComponent.h>
#include <Engine/Modules/Graphics/GraphicsSystem/DebugPainter.h>
#include <Engine/swdebug.h>

#include <Engine/Modules/Physics/PhysicsSystem.h>
#include <Engine/Modules/Physics/RigidBodyComponent.h>

#include <Engine/Modules/Audio/AudioSourceComponent.h>

#include <Engine/Modules/Math/MathUtils.h>

#include <utility>

#include "PlayerComponent.h"

PlayerControlSystem::PlayerControlSystem(std::shared_ptr<InputModule> inputModule,
  std::shared_ptr<SharedGraphicsState> sharedGraphicsState)
  : m_inputModule(std::move(inputModule)),
    m_sharedGraphicsState(std::move(sharedGraphicsState))
{

}

void PlayerControlSystem::configure(GameWorld* gameWorld)
{
  if (m_playerObject == nullptr) {
    m_playerObject = gameWorld->findGameObject([](const GameObject& obj) {
      return obj.hasComponent<PlayerComponent>();
    });

    SW_ASSERT(m_playerObject->hasComponent<CameraComponent>() &&
      m_playerObject->hasComponent<TransformComponent>() &&
      m_playerObject->hasComponent<SkeletalAnimationComponent>() &&
      m_playerObject->hasComponent<KinematicCharacterComponent>());

    auto& skeletalAnimationComponent = m_playerObject->getComponent<SkeletalAnimationComponent>();

    m_walkAnimationStateId =
      skeletalAnimationComponent.getAnimationStatesMachineRef().getStateIdByName("walk");

    m_idleAnimationStateId =
      skeletalAnimationComponent.getAnimationStatesMachineRef().getStateIdByName("idle");

    skeletalAnimationComponent.getAnimationStatesMachineRef().setActiveState(m_walkAnimationStateId);
  }
}

void PlayerControlSystem::unconfigure(GameWorld* gameWorld)
{
  ARG_UNUSED(gameWorld);
}

void PlayerControlSystem::activate()
{
  m_inputModule->registerAction("forward", KeyboardInputAction(SDLK_w));
  m_inputModule->registerAction("backward", KeyboardInputAction(SDLK_s));
  m_inputModule->registerAction("left", KeyboardInputAction(SDLK_a));
  m_inputModule->registerAction("right", KeyboardInputAction(SDLK_d));

  m_inputModule->enableGlobalTracking();
  m_inputModule->setMouseMovementMode(MouseMovementMode::Relative);

  getGameWorld().subscribeEventsListener<MouseWheelEvent>(this);
  getGameWorld().subscribeEventsListener<InputActionToggleEvent>(this);

  m_sharedGraphicsState->setActiveCamera(m_playerObject->getComponent<CameraComponent>().getCamera());
}

void PlayerControlSystem::deactivate()
{
  // TODO: Reset active camera here, add default camera and switch to it in upper layers in this case
  // m_sharedGraphicsState->setActiveCamera(nullptr);

  getGameWorld().unsubscribeEventsListener<InputActionToggleEvent>(this);
  getGameWorld().unsubscribeEventsListener<MouseWheelEvent>(this);

  m_inputModule->unregisterAction("forward");
  m_inputModule->unregisterAction("backward");
  m_inputModule->unregisterAction("left");
  m_inputModule->unregisterAction("right");

  m_inputModule->setMouseMovementMode(MouseMovementMode::Absolute);
}

void PlayerControlSystem::update(GameWorld* gameWorld, float delta)
{
  ARG_UNUSED(gameWorld);

  auto mouseDeltaTemp = m_inputModule->getMouseDelta();
  glm::vec2 mouseDelta(mouseDeltaTemp.x, mouseDeltaTemp.y);

  mouseDelta *= -0.25;

  auto& playerComponent = m_playerObject->getComponent<PlayerComponent>();

  playerComponent.increaseThirdPersonViewPitch(mouseDelta.y * 6.0f * delta);
  playerComponent.increaseThirdPersonViewYaw(mouseDelta.x * 6.0f * delta);

  float cameraYawAngle = playerComponent.getThirdPersonViewYaw();

  float horizontalOffset = playerComponent.getDistanceToPlayer() *
    glm::cos(glm::radians(playerComponent.getThirdPersonViewPitch()));

  float horizontalOffsetX = horizontalOffset * glm::sin(glm::radians(cameraYawAngle));
  float horizontalOffsetZ = horizontalOffset * glm::cos(glm::radians(cameraYawAngle));

  float verticalOffset = playerComponent.getDistanceToPlayer() *
    glm::sin(glm::radians(playerComponent.getThirdPersonViewPitch()));

  auto& playerCameraTransform = *m_playerObject->getComponent<CameraComponent>().getCamera()->getTransform();
  auto& playerTransform = m_playerObject->getComponent<TransformComponent>().getTransform();

  glm::vec3 playerOrigin = playerTransform.getPosition() +
    glm::vec3(0.0f, playerComponent.getPlayerHeight() / 2.0f, 0.0f);

  playerCameraTransform.setPosition(playerOrigin.x - horizontalOffsetX,
    playerOrigin.y + verticalOffset,
    playerOrigin.z - horizontalOffsetZ);

  auto& animationStatesMachine =
    m_playerObject->getComponent<SkeletalAnimationComponent>().getAnimationStatesMachineRef();

  //auto& animationVariablesSet = animationStatesMachine.getVariablesSet();

  bool playerIsWalking = m_inputModule->isActionActive("forward") ||
    m_inputModule->isActionActive("backward") ||
    m_inputModule->isActionActive("right") ||
    m_inputModule->isActionActive("left");

  glm::vec3 playerCameraFrontDirection = glm::normalize(playerCameraTransform.getFrontDirection() *
    glm::vec3{1.0f, 0.0f, 1.0f});

  playerTransform.lookAt(playerTransform.getPosition() - playerCameraFrontDirection);

  auto& playerAudioSource = m_playerObject->getComponent<AudioSourceComponent>().getSource();

  if (playerIsWalking) {
    if (animationStatesMachine.getActiveStateId() != m_walkAnimationStateId) {
      animationStatesMachine.switchToNextState(m_walkAnimationStateId);
    }

    if (!playerAudioSource.isPlaying()) {
      playerAudioSource.play();
    }
  }
  else {
    if (animationStatesMachine.getActiveStateId() == m_walkAnimationStateId) {
      animationStatesMachine.switchToNextState(m_idleAnimationStateId);
    }

    playerAudioSource.stop();
  }

  playerCameraTransform.lookAt(playerOrigin);
  m_playerObject->getComponent<KinematicCharacterComponent>().setTransform(playerTransform);
}

Camera& PlayerControlSystem::getPlayerCamera() const
{
  return *m_playerObject->getComponent<CameraComponent>().getCamera();
}

void PlayerControlSystem::render(GameWorld* gameWorld)
{
  ARG_UNUSED(gameWorld);

  //DebugPainter::renderAABB(m_playerObject->getComponent<MeshRendererComponent>().getAABB());
}

EventProcessStatus PlayerControlSystem::receiveEvent(GameWorld* gameWorld, const MouseWheelEvent& event)
{
  ARG_UNUSED(gameWorld);

  auto& playerComponent = m_playerObject->getComponent<PlayerComponent>();
  playerComponent.increaseDistanceToPlayer(event.wheelDelta * 0.2f);

  return EventProcessStatus::Processed;
}

EventProcessStatus PlayerControlSystem::receiveEvent(GameWorld* gameWorld, const InputActionToggleEvent& event)
{
  ARG_UNUSED(gameWorld);
  ARG_UNUSED(event);

  return EventProcessStatus::Processed;
}

void PlayerControlSystem::fixedUpdate(GameWorld* gameWorld, float delta)
{
  ARG_UNUSED(gameWorld);

  const auto& playerComponent = m_playerObject->getComponent<PlayerComponent>();
  auto& playerCameraTransform = *getPlayerCamera().getTransform();
  // auto& playerTransform = m_playerObject->getComponent<TransformComponent>().getTransform();

  glm::vec3 playerCameraFrontDirection = glm::normalize(playerCameraTransform.getFrontDirection() *
    glm::vec3{1.0f, 0.0f, 1.0f});
  glm::vec3 playerCameraRightDirection = glm::normalize(playerCameraTransform.getRightDirection() *
    glm::vec3{1.0f, 0.0f, 1.0f});

  float playerMovementSpeed = playerComponent.getMovementSpeed();

  bool playerIsWalking = m_inputModule->isActionActive("forward") ||
    m_inputModule->isActionActive("backward") ||
    m_inputModule->isActionActive("right") ||
    m_inputModule->isActionActive("left");

  glm::vec3 playerMovementDirection{};

  if (m_inputModule->isActionActive("forward")) {
    playerMovementDirection += playerCameraFrontDirection;
  }

  if (m_inputModule->isActionActive("backward")) {
    playerMovementDirection += playerCameraFrontDirection * (-1.0f);
  }

  if (m_inputModule->isActionActive("right")) {
    playerMovementDirection += playerCameraRightDirection;
  }

  if (m_inputModule->isActionActive("left")) {
    playerMovementDirection += playerCameraRightDirection * (-1.0f);
  }

  auto& playerKinematicCharacterComponent = m_playerObject->getComponent<KinematicCharacterComponent>();

  if (playerIsWalking) {
    if (!MathUtils::isEqual(playerMovementDirection, MathUtils::ZERO_VEC3, 1e-2f)) {
      playerMovementDirection = glm::normalize(playerMovementDirection);
    }

    playerKinematicCharacterComponent.setPositionIncrement(playerMovementDirection * playerMovementSpeed * delta);
  }
  else {
    playerKinematicCharacterComponent.setPositionIncrement(glm::vec3(0.0f));
  }
}
