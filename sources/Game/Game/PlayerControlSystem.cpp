#include "PlayerControlSystem.h"
#include <Engine/Modules/ECS/GameWorld.h>

#include <spdlog/spdlog.h>

#include <Engine/Modules/Graphics/GraphicsSystem/CameraComponent.h>
#include <Engine/Modules/Graphics/GraphicsSystem/TransformComponent.h>
#include <Engine/Modules/Graphics/GraphicsSystem/MeshRendererComponent.h>
#include <Engine/Modules/Graphics/GraphicsSystem/Animation/SkeletalAnimationComponent.h>
#include <Engine/Modules/Graphics/GraphicsSystem/DebugPainter.h>
#include <Engine/swdebug.h>

#include "PlayerComponent.h"

PlayerControlSystem::PlayerControlSystem(std::shared_ptr<InputModule> inputModule)
  : m_inputModule(inputModule)
{

}

PlayerControlSystem::~PlayerControlSystem() = default;

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
    m_playerObject->getComponent<TransformComponent>()->getTransform()->move({0.0f, 0.35f, 0.0f});

    const auto& skeletalAnimationComponent = m_playerObject->getComponent<SkeletalAnimationComponent>();

    m_runningAnimationStateId =
      skeletalAnimationComponent->getAnimationStatesMachine().getStateIdByName("running");

    m_idleAnimationStateId =
      skeletalAnimationComponent->getAnimationStatesMachine().getStateIdByName("idle");

    skeletalAnimationComponent->getAnimationStatesMachine().setActiveState(m_runningAnimationStateId);
  }

  gameWorld->subscribeEventsListener<MouseWheelEvent>(this);
  gameWorld->subscribeEventsListener<InputActionToggleEvent>(this);
}

void PlayerControlSystem::unconfigure(GameWorld* gameWorld)
{
  ARG_UNUSED(gameWorld);

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
  ARG_UNUSED(delta);

  auto mouseDeltaTemp = m_inputModule->getMouseDelta();
  glm::vec2 mouseDelta(mouseDeltaTemp.x, mouseDeltaTemp.y);

  mouseDelta *= -0.25;

  updateViewParameters(mouseDelta, delta);
  updatePlayerAndCameraPosition(delta);

//  playerCamera->getTransform()->pitchLocal(mouseDelta.y);
//  playerCamera->getTransform()->yawGlobal(mouseDelta.x);
//
//  if (m_inputModule->isActionActive("forward")) {
//    move(playerCamera->getTransform()->getFrontDirection() * movementSpeed * delta);
//  }
//
//  if (m_inputModule->isActionActive("backward")) {
//    move(playerCamera->getTransform()->getFrontDirection() * movementSpeed * (-1.0f) * delta);
//  }
//
//  if (m_inputModule->isActionActive("right")) {
//    move(playerCamera->getTransform()->getRightDirection() * movementSpeed * delta);
//  }
//
//  if (m_inputModule->isActionActive("left")) {
//    move(playerCamera->getTransform()->getRightDirection() * movementSpeed * (-1.0f) * delta);
//  }
//
  // playerCamera->getTransform()->lookAt(0.0f, 0.0f, 0.0f);
  // playerTransform.lookAt(playerCamera->getTransform()->getPosition());

}

Camera* PlayerControlSystem::getPlayerCamera() const
{
  return m_playerObject->getComponent<CameraComponent>()->getCamera().get();
}

Transform* PlayerControlSystem::getPlayerTransform() const
{
  return m_playerObject->getComponent<TransformComponent>()->getTransform();
}

void PlayerControlSystem::render(GameWorld* gameWorld)
{
  ARG_UNUSED(gameWorld);

  DebugPainter::renderAABB(m_playerObject->getComponent<MeshRendererComponent>()->getAABB());
}

EventProcessStatus PlayerControlSystem::receiveEvent(GameWorld* gameWorld, const MouseWheelEvent& event)
{
  ARG_UNUSED(gameWorld);

  const auto& playerComponent = m_playerObject->getComponent<PlayerComponent>();
  playerComponent->increaseDistanceToPlayer(event.wheelDelta * 0.2f);

  return EventProcessStatus::Processed;
}

void PlayerControlSystem::updateViewParameters(const glm::vec2& mouseDelta, float delta)
{
  const auto& playerComponent = m_playerObject->getComponent<PlayerComponent>();

  //if (m_inputModule->isMouseButtonPressed(SDL_BUTTON_LEFT)) {
  playerComponent->increaseThirdPersonViewPitch(mouseDelta.y * 6.0f * delta);
  //}

  //if (m_inputModule->isMouseButtonPressed(SDL_BUTTON_RIGHT)) {
  playerComponent->increaseThirdPersonViewYaw(mouseDelta.x * 6.0f * delta);
  //}
}

void PlayerControlSystem::updatePlayerAndCameraPosition(float delta)
{
  const auto& playerComponent = m_playerObject->getComponent<PlayerComponent>();
  auto& playerCameraTransform = *getPlayerCamera()->getTransform();
  auto& playerTransform = *m_playerObject->getComponent<TransformComponent>()->getTransform();

  float cameraYawAngle = playerComponent->getThirdPersonViewYaw();

  float horizontalOffset = playerComponent->getDistanceToPlayer() *
    glm::cos(glm::radians(playerComponent->getThirdPersonViewPitch()));

  float horizontalOffsetX = horizontalOffset * glm::sin(glm::radians(cameraYawAngle));
  float horizontalOffsetZ = horizontalOffset * glm::cos(glm::radians(cameraYawAngle));

  float verticalOffset = playerComponent->getDistanceToPlayer() *
    glm::sin(glm::radians(playerComponent->getThirdPersonViewPitch()));

  playerCameraTransform.setPosition(playerTransform.getPosition().x - horizontalOffsetX,
    playerTransform.getPosition().y + verticalOffset,
    playerTransform.getPosition().z - horizontalOffsetZ);

  glm::vec3 playerCameraFrontDirection = glm::normalize(playerCameraTransform.getFrontDirection() *
    glm::vec3{1.0f, 0.0f, 1.0f});
  glm::vec3 playerCameraRightDirection = glm::normalize(playerCameraTransform.getRightDirection() *
    glm::vec3{1.0f, 0.0f, 1.0f});

  float playerMovementSpeed = playerComponent->getMovementSpeed();

  bool playerIsRunning = m_inputModule->isActionActive("forward") ||
    m_inputModule->isActionActive("backward") ||
    m_inputModule->isActionActive("right") ||
    m_inputModule->isActionActive("left");

  if (m_inputModule->isActionActive("forward")) {
    playerTransform.move(playerCameraFrontDirection * playerMovementSpeed * delta);
  }

  if (m_inputModule->isActionActive("backward")) {
    playerTransform.move(playerCameraFrontDirection * playerMovementSpeed * (-1.0f) * delta);
  }

  if (m_inputModule->isActionActive("right")) {
    playerTransform.move(playerCameraRightDirection * playerMovementSpeed * delta);
  }

  if (m_inputModule->isActionActive("left")) {
    playerTransform.move(playerCameraRightDirection * playerMovementSpeed * (-1.0f) * delta);
  }

  auto& animationStatesMachine =
    m_playerObject->getComponent<SkeletalAnimationComponent>()->getAnimationStatesMachine();

  auto& animationVariablesSet = animationStatesMachine.getVariablesSet();
  float lookAroundFactor = animationVariablesSet.getVariableValue("look-around-factor");

  if (m_inputModule->isActionActive("look_around_left")) {
    lookAroundFactor = glm::clamp(lookAroundFactor - 1.0f * delta, 0.0f, 1.0f);
  }

  if (m_inputModule->isActionActive("look_around_right")) {
    lookAroundFactor = glm::clamp(lookAroundFactor + 1.0f * delta, 0.0f, 1.0f);
  }

  animationVariablesSet.setVariableValue("look-around-factor", lookAroundFactor);

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
  if (event.actionName == "look_around" && event.newState == InputActionState::Active) {
    auto& animationStatesMachine =
      m_playerObject->getComponent<SkeletalAnimationComponent>()->getAnimationStatesMachine();

    int16_t lookAroundStateId = animationStatesMachine.getStateIdByName("look-around");

    if (animationStatesMachine.getActiveStateId() != lookAroundStateId) {
      animationStatesMachine.switchToNextState(lookAroundStateId);
    }
  }

  return EventProcessStatus::Processed;
}
