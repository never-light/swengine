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

// TODO: PlayerControlSystem class methods uses some magic numbers.
// Consider replacing them with constants or moving to configuration files

PlayerControlSystem::PlayerControlSystem(std::shared_ptr<InputModule> inputModule,
  std::shared_ptr<GraphicsScene> graphicsScene,
  std::shared_ptr<GUILayout> playerUILayout,
  std::shared_ptr<InventoryUI> inventoryUILayout)
  : m_inputModule(std::move(inputModule)),
    m_graphicsScene(std::move(graphicsScene)),
    m_uiLayout(std::move(playerUILayout)),
    m_inventoryUI(std::move(inventoryUILayout))
{

}

void PlayerControlSystem::configure()
{
  if (!m_playerObject.isAlive()) {
    m_playerObject = *getGameWorld()->allWith<PlayerComponent>().begin();

    SW_ASSERT(m_playerObject.isAlive());

    SW_ASSERT(m_playerObject.hasComponent<CameraComponent>() &&
      m_playerObject.hasComponent<TransformComponent>() &&
      m_playerObject.hasComponent<SkeletalAnimationComponent>() &&
      m_playerObject.hasComponent<KinematicCharacterComponent>());

    auto& skeletalAnimationComponent = *m_playerObject.getComponent<SkeletalAnimationComponent>().get();

    m_walkAnimationStateId =
      skeletalAnimationComponent.getAnimationStatesMachineRef().getStateIdByName("walk");

    m_idleAnimationStateId =
      skeletalAnimationComponent.getAnimationStatesMachineRef().getStateIdByName("idle");

    skeletalAnimationComponent.getAnimationStatesMachineRef().setActiveState(m_walkAnimationStateId);

    m_inventoryUI->setInventoryOwner(m_playerObject);
  }

}

void PlayerControlSystem::unconfigure()
{
}

void PlayerControlSystem::activate()
{
  m_uiLayout->addChildWidget(m_inventoryUI);
  m_inventoryUI->hide();

  m_inputModule->enableGlobalTracking();
  m_inputModule->setMouseMovementMode(MouseMovementMode::Relative);

  m_graphicsScene->setActiveCamera(m_playerObject.getComponent<CameraComponent>()->getCamera());

  getGameWorld()->subscribeEventsListener<InputActionToggleEvent>(this);
  getGameWorld()->subscribeEventsListener<KeyboardEvent>(this);

  enableMovementControl();
}

void PlayerControlSystem::deactivate()
{
  if (m_isMovementControlEnabled) {
    disableMovementControl();
  }

  getGameWorld()->unsubscribeEventsListener<KeyboardEvent>(this);
  getGameWorld()->unsubscribeEventsListener<InputActionToggleEvent>(this);

  m_inputModule->setMouseMovementMode(MouseMovementMode::Absolute);

  m_uiLayout->removeChildWidget(m_inventoryUI);

  // TODO: Reset active camera here, add default camera and switch to it in upper layers in this case
  // m_graphicsScene->setActiveCamera(nullptr);

}

void PlayerControlSystem::update(float delta)
{
  // TODO: getMouseDelta usage is unobvious and could lead to side effects. Fix it.
  auto mouseDeltaTemp = m_inputModule->getMouseDelta();

  if (!m_isMovementControlEnabled) {
    return;
  }

  glm::vec2 mouseDelta(mouseDeltaTemp.x, mouseDeltaTemp.y);

  mouseDelta *= -0.25;

  auto& playerComponent = *m_playerObject.getComponent<PlayerComponent>().get();

  playerComponent.increaseThirdPersonViewPitch(mouseDelta.y * 6.0f * delta);
  playerComponent.increaseThirdPersonViewYaw(mouseDelta.x * 6.0f * delta);

  float cameraYawAngle = playerComponent.getThirdPersonViewYaw();

  float horizontalOffset = playerComponent.getDistanceToPlayer() *
    glm::cos(glm::radians(playerComponent.getThirdPersonViewPitch()));

  float horizontalOffsetX = horizontalOffset * glm::sin(glm::radians(cameraYawAngle));
  float horizontalOffsetZ = horizontalOffset * glm::cos(glm::radians(cameraYawAngle));

  float verticalOffset = playerComponent.getDistanceToPlayer() *
    glm::sin(glm::radians(playerComponent.getThirdPersonViewPitch()));

  auto& playerCameraTransform = *m_playerObject.getComponent<CameraComponent>()->getCamera()->getTransform();
  auto& playerTransform = m_playerObject.getComponent<TransformComponent>()->getTransform();

  glm::vec3 playerOrigin = playerTransform.getPosition() +
    glm::vec3(0.0f, playerComponent.getPlayerHeight() / 2.0f, 0.0f);

  playerCameraTransform.setPosition(playerOrigin.x - horizontalOffsetX,
    playerOrigin.y + verticalOffset,
    playerOrigin.z - horizontalOffsetZ);

  auto& animationStatesMachine =
    m_playerObject.getComponent<SkeletalAnimationComponent>()->getAnimationStatesMachineRef();

  //auto& animationVariablesSet = animationStatesMachine.getVariablesSet();

  bool playerIsWalking = m_inputModule->isActionActive("forward") ||
    m_inputModule->isActionActive("backward") ||
    m_inputModule->isActionActive("right") ||
    m_inputModule->isActionActive("left");

  glm::vec3 playerCameraFrontDirection = glm::normalize(playerCameraTransform.getFrontDirection() *
    glm::vec3{1.0f, 0.0f, 1.0f});

  playerTransform.lookAt(playerTransform.getPosition() - playerCameraFrontDirection);

  auto& playerAudioSource = m_playerObject.getComponent<AudioSourceComponent>()->getSource();

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
  m_playerObject.getComponent<KinematicCharacterComponent>()->setTransform(playerTransform);
}

Camera& PlayerControlSystem::getPlayerCamera()
{
  return *m_playerObject.getComponent<CameraComponent>()->getCamera();
}

void PlayerControlSystem::render()
{
  //DebugPainter::renderAABB(m_playerObject.getComponent<MeshRendererComponent>().getAABB());
}

EventProcessStatus PlayerControlSystem::receiveEvent(GameWorld* gameWorld, const MouseWheelEvent& event)
{
  ARG_UNUSED(gameWorld);

  auto& playerComponent = *m_playerObject.getComponent<PlayerComponent>().get();
  playerComponent.increaseDistanceToPlayer(event.wheelDelta * 0.2f);

  return EventProcessStatus::Processed;
}

EventProcessStatus PlayerControlSystem::receiveEvent(GameWorld* gameWorld, const InputActionToggleEvent& event)
{
  ARG_UNUSED(gameWorld);
  ARG_UNUSED(event);

  return EventProcessStatus::Processed;
}

void PlayerControlSystem::fixedUpdate(float delta)
{
  if (!m_isMovementControlEnabled) {
    return;
  }

  auto& playerComponent = *m_playerObject.getComponent<PlayerComponent>().get();
  auto& playerCameraTransform = *getPlayerCamera().getTransform();
  // auto& playerTransform = m_playerObject.getComponent<TransformComponent>().getTransform();

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

  auto& playerKinematicCharacterComponent = *m_playerObject.getComponent<KinematicCharacterComponent>().get();

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

void PlayerControlSystem::enableMovementControl()
{
  m_isMovementControlEnabled = true;

  m_inputModule->registerAction("forward", KeyboardInputAction(SDLK_w));
  m_inputModule->registerAction("backward", KeyboardInputAction(SDLK_s));
  m_inputModule->registerAction("left", KeyboardInputAction(SDLK_a));
  m_inputModule->registerAction("right", KeyboardInputAction(SDLK_d));

  getGameWorld()->subscribeEventsListener<MouseWheelEvent>(this);
}

void PlayerControlSystem::disableMovementControl()
{
  m_isMovementControlEnabled = false;

  getGameWorld()->unsubscribeEventsListener<MouseWheelEvent>(this);

  m_inputModule->unregisterAction("forward");
  m_inputModule->unregisterAction("backward");
  m_inputModule->unregisterAction("left");
  m_inputModule->unregisterAction("right");

  auto& playerKinematicCharacterComponent = *m_playerObject.getComponent<KinematicCharacterComponent>().get();
  playerKinematicCharacterComponent.setPositionIncrement(glm::vec3(0.0f));
}

void PlayerControlSystem::showGUIWindow(std::shared_ptr<GUILayout> window)
{
  disableMovementControl();
  m_inputModule->setMouseMovementMode(MouseMovementMode::Absolute);

  m_isUIModeActive = true;

  window->show();
}

void PlayerControlSystem::hideGUIWindow(std::shared_ptr<GUILayout> window)
{
  window->hide();

  m_inputModule->setMouseMovementMode(MouseMovementMode::Relative);
  enableMovementControl();

  m_isUIModeActive = false;
}

EventProcessStatus PlayerControlSystem::receiveEvent(GameWorld* gameWorld, const KeyboardEvent& event)
{
  ARG_UNUSED(gameWorld);

  if (event.type == KeyboardEventType::KeyDown) {
    if (event.keyCode == SDLK_i) {
      if (m_inventoryUI->isShown()) {
        hideGUIWindow(m_inventoryUI);
      }
      else {
        showGUIWindow(m_inventoryUI);
      }
    }
  }

  return EventProcessStatus::Processed;
}
