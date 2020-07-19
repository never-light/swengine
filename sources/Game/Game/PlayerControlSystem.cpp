#include "PlayerControlSystem.h"

#include <Engine/Modules/Graphics/GraphicsSystem/TransformComponent.h>
#include <Engine/Modules/Graphics/GraphicsSystem/MeshRendererComponent.h>
#include <Engine/Modules/Graphics/GraphicsSystem/Animation/SkeletalAnimationComponent.h>
#include <Engine/Modules/Graphics/GraphicsSystem/DebugPainter.h>
#include <Engine/swdebug.h>

#include "PlayerComponent.h"

PlayerControlSystem::PlayerControlSystem(std::shared_ptr<InputModule> inputModule,
  std::shared_ptr<SharedGraphicsState> sharedGraphicsState)
  : m_inputModule(inputModule),
    m_sharedGraphicsState(sharedGraphicsState)
{

}

void PlayerControlSystem::configure(GameWorld* gameWorld)
{
  m_inputModule->registerAction("forward", KeyboardInputAction(SDLK_w));
  m_inputModule->registerAction("backward", KeyboardInputAction(SDLK_s));
  m_inputModule->registerAction("left", KeyboardInputAction(SDLK_a));
  m_inputModule->registerAction("right", KeyboardInputAction(SDLK_d));
  m_inputModule->registerAction("look_around", KeyboardInputAction(SDLK_f));
  m_inputModule->registerAction("look_around_left", KeyboardInputAction(SDLK_l));
  m_inputModule->registerAction("look_around_right", KeyboardInputAction(SDLK_k));

  m_inputModule->enableGlobalTracking();
  m_inputModule->setMouseMovementMode(MouseMovementMode::Relative);

  if (m_playerObject == nullptr) {
    const auto& playerObjectsView = gameWorld->allWith<PlayerComponent, CameraComponent, TransformComponent>();

    SW_ASSERT(!playerObjectsView.begin().isEnd());

    m_playerObject = playerObjectsView.begin().getGameObject();
    m_playerObject->getComponent<TransformComponent>().getTransform().move({0.0f, 0.35f, 0.0f});

    auto& skeletalAnimationComponent = m_playerObject->getComponent<SkeletalAnimationComponent>();

    m_runningAnimationStateId =
      skeletalAnimationComponent.getAnimationStatesMachineRef().getStateIdByName("running");

    m_idleAnimationStateId =
      skeletalAnimationComponent.getAnimationStatesMachineRef().getStateIdByName("idle");

    skeletalAnimationComponent.getAnimationStatesMachineRef().setActiveState(m_runningAnimationStateId);
  }

  gameWorld->subscribeEventsListener<MouseWheelEvent>(this);
  gameWorld->subscribeEventsListener<InputActionToggleEvent>(this);

  m_sharedGraphicsState->setActiveCamera(m_playerObject->getComponent<CameraComponent>().getCamera());
}

void PlayerControlSystem::unconfigure(GameWorld* gameWorld)
{
  ARG_UNUSED(gameWorld);

  // TODO: Reset active camera here, add default camera and switch to it in upper layers in this case
  // m_sharedGraphicsState->setActiveCamera(nullptr);

  gameWorld->unsubscribeEventsListener<InputActionToggleEvent>(this);
  gameWorld->unsubscribeEventsListener<MouseWheelEvent>(this);

  m_inputModule->unregisterAction("forward");
  m_inputModule->unregisterAction("backward");
  m_inputModule->unregisterAction("left");
  m_inputModule->unregisterAction("right");
  m_inputModule->unregisterAction("look_around");
  m_inputModule->unregisterAction("look_around_left");
  m_inputModule->unregisterAction("look_around_right");

  m_inputModule->setMouseMovementMode(MouseMovementMode::Absolute);
}

void PlayerControlSystem::update(GameWorld* gameWorld, float delta)
{
  ARG_UNUSED(gameWorld);
  ARG_UNUSED(delta);

  auto mouseDeltaTemp = m_inputModule->getMouseDelta();
  glm::vec2 mouseDelta(mouseDeltaTemp.x, mouseDeltaTemp.y);

  mouseDelta *= -0.25;

  updateViewParameters(mouseDelta, delta);
  updatePlayerAndCameraPosition(delta);
}

Camera& PlayerControlSystem::getPlayerCamera() const
{
  return *m_playerObject->getComponent<CameraComponent>().getCamera();
}

void PlayerControlSystem::render(GameWorld* gameWorld)
{
  ARG_UNUSED(gameWorld);

  DebugPainter::renderAABB(m_playerObject->getComponent<MeshRendererComponent>().getAABB());
}

EventProcessStatus PlayerControlSystem::receiveEvent(GameWorld* gameWorld, const MouseWheelEvent& event)
{
  ARG_UNUSED(gameWorld);

  auto& playerComponent = m_playerObject->getComponent<PlayerComponent>();
  playerComponent.increaseDistanceToPlayer(event.wheelDelta * 0.2f);

  return EventProcessStatus::Processed;
}

void PlayerControlSystem::updateViewParameters(const glm::vec2& mouseDelta, float delta)
{
  auto& playerComponent = m_playerObject->getComponent<PlayerComponent>();

  playerComponent.increaseThirdPersonViewPitch(mouseDelta.y * 6.0f * delta);
  playerComponent.increaseThirdPersonViewYaw(mouseDelta.x * 6.0f * delta);
}

void PlayerControlSystem::updatePlayerAndCameraPosition(float delta)
{
  const auto& playerComponent = m_playerObject->getComponent<PlayerComponent>();
  auto& playerCameraTransform = *getPlayerCamera().getTransform();
  auto& playerTransform = m_playerObject->getComponent<TransformComponent>().getTransform();

  float cameraYawAngle = playerComponent.getThirdPersonViewYaw();

  float horizontalOffset = playerComponent.getDistanceToPlayer() *
    glm::cos(glm::radians(playerComponent.getThirdPersonViewPitch()));

  float horizontalOffsetX = horizontalOffset * glm::sin(glm::radians(cameraYawAngle));
  float horizontalOffsetZ = horizontalOffset * glm::cos(glm::radians(cameraYawAngle));

  float verticalOffset = playerComponent.getDistanceToPlayer() *
    glm::sin(glm::radians(playerComponent.getThirdPersonViewPitch()));

  playerCameraTransform.setPosition(playerTransform.getPosition().x - horizontalOffsetX,
    playerTransform.getPosition().y + verticalOffset,
    playerTransform.getPosition().z - horizontalOffsetZ);

  glm::vec3 playerCameraFrontDirection = glm::normalize(playerCameraTransform.getFrontDirection() *
    glm::vec3{1.0f, 0.0f, 1.0f});
  glm::vec3 playerCameraRightDirection = glm::normalize(playerCameraTransform.getRightDirection() *
    glm::vec3{1.0f, 0.0f, 1.0f});

  float playerMovementSpeed = playerComponent.getMovementSpeed();

  bool playerIsRunning = m_inputModule->isActionActive("forward") ||
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

  if (playerIsRunning) {
    if (glm::length(playerMovementDirection) >= 1.0f) {
      playerMovementDirection = glm::normalize(playerMovementDirection);
    }

    playerTransform.move(playerMovementDirection * playerMovementSpeed * delta);
  }

  // TODO: that's code for animation demo, remove it and implement real player animations
  auto& animationStatesMachine =
    m_playerObject->getComponent<SkeletalAnimationComponent>().getAnimationStatesMachineRef();

  auto& animationVariablesSet = animationStatesMachine.getVariablesSet();
  float lookAroundFactor = animationVariablesSet.getVariableValue("look_around_factor");

  if (m_inputModule->isActionActive("look_around_left")) {
    lookAroundFactor = glm::clamp(lookAroundFactor - 1.0f * delta, 0.0f, 1.0f);
  }

  if (m_inputModule->isActionActive("look_around_right")) {
    lookAroundFactor = glm::clamp(lookAroundFactor + 1.0f * delta, 0.0f, 1.0f);
  }

  animationVariablesSet.setVariableValue("look_around_factor", lookAroundFactor);

  if (playerIsRunning) {
    playerTransform.lookAt(playerTransform.getPosition() - playerCameraFrontDirection);
    playerTransform.pitchLocal(-90.0f);

    if (animationStatesMachine.getActiveStateId() != m_runningAnimationStateId) {
      animationStatesMachine.switchToNextState(m_runningAnimationStateId);
    }
  }
  else {
    if (animationStatesMachine.getActiveStateId() == m_runningAnimationStateId) {
      animationStatesMachine.switchToNextState(m_idleAnimationStateId);
    }
  }

  playerCameraTransform.lookAt(playerTransform.getPosition());
}

EventProcessStatus PlayerControlSystem::receiveEvent(GameWorld* gameWorld, const InputActionToggleEvent& event)
{
  ARG_UNUSED(gameWorld);

  // TODO: that's code for animation demo, remove it and implement real player animations
  if (event.actionName == "look_around" && event.newState == InputActionState::Active) {
    auto& animationStatesMachine =
      m_playerObject->getComponent<SkeletalAnimationComponent>().getAnimationStatesMachineRef();

    int16_t lookAroundStateId = animationStatesMachine.getStateIdByName("look_around");

    if (animationStatesMachine.getActiveStateId() != lookAroundStateId) {
      animationStatesMachine.switchToNextState(lookAroundStateId);
    }
  }

  return EventProcessStatus::Processed;
}
