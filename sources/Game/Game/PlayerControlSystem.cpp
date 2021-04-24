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
#include "Game/Dynamic/InteractiveObjectComponent.h"

// TODO: PlayerControlSystem class methods uses some magic numbers.
// Consider replacing them with constants or moving to configuration files

// TODO: separate player movement and player UI controlling to different classes and
//  use it here.

PlayerControlSystem::PlayerControlSystem(std::shared_ptr<InputModule> inputModule,
  std::shared_ptr<GraphicsScene> graphicsScene,
  PlayerUILayout uiLayout)
  : m_inputModule(std::move(inputModule)),
    m_graphicsScene(std::move(graphicsScene)),
    m_uiLayout(std::move(uiLayout))
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

    m_uiLayout.inventoryUI->setInventoryOwner(m_playerObject);
  }

}

void PlayerControlSystem::unconfigure()
{
}

void PlayerControlSystem::activate()
{
  m_uiLayout.playerUILayout->addChildWidget(m_uiLayout.inventoryUI);
  m_uiLayout.inventoryUI->hide();

  m_uiLayout.playerUILayout->addChildWidget(m_uiLayout.interactionUI);
  m_uiLayout.interactionUI->hide();

  m_uiLayout.playerUILayout->addChildWidget(m_uiLayout.dialoguesUI);
  m_uiLayout.dialoguesUI->hide();

  m_uiLayout.playerUILayout->addChildWidget(m_uiLayout.hudLayout);
  m_uiLayout.hudLayout->show();

  m_inputModule->enableGlobalTracking();
  m_inputModule->setMouseMovementMode(MouseMovementMode::Relative);

  m_graphicsScene->setActiveCamera(m_playerObject.getComponent<CameraComponent>()->getCamera());

  getGameWorld()->subscribeEventsListener<InputActionToggleEvent>(this);
  getGameWorld()->subscribeEventsListener<KeyboardEvent>(this);
  getGameWorld()->subscribeEventsListener<ActorDialogueStartCommandEvent>(this);
  getGameWorld()->subscribeEventsListener<StopDialogueCommandEvent>(this);

  m_questInfoLayout = std::dynamic_pointer_cast<GUILayout>(
    m_uiLayout.hudLayout->findChildByName("game_ui_actor_hud_layout_quest_info"));
  SW_ASSERT(m_questInfoLayout != nullptr);

  m_questInfoTitle = std::dynamic_pointer_cast<GUIText>(m_questInfoLayout
    ->findChildByName("game_ui_actor_hud_layout_quest_info_title"));
  m_questInfoTaskTitle = std::dynamic_pointer_cast<GUIText>(m_questInfoLayout
    ->findChildByName("game_ui_actor_hud_layout_quest_info_task_title"));

  SW_ASSERT(m_questInfoTitle != nullptr && m_questInfoTaskTitle != nullptr);

  m_healthInfoLayout = std::dynamic_pointer_cast<GUILayout>(
    m_uiLayout.hudLayout->findChildByName("game_ui_actor_hud_layout_health_info"));
  SW_ASSERT(m_questInfoLayout != nullptr);

  m_healthProgressBar = std::dynamic_pointer_cast<GUIProgressBar>(m_healthInfoLayout
    ->findChildByName("game_ui_actor_hud_layout_health_info_health"));

  SW_ASSERT(m_healthProgressBar != nullptr);

  enableMovementControl();
}

void PlayerControlSystem::deactivate()
{
  if (isGUIWindowModeActive()) {
    hideGUIWindow();
  }

  if (m_isMovementControlEnabled) {
    disableMovementControl();
  }

  getGameWorld()->unsubscribeEventsListener<StopDialogueCommandEvent>(this);
  getGameWorld()->unsubscribeEventsListener<ActorDialogueStartCommandEvent>(this);
  getGameWorld()->unsubscribeEventsListener<KeyboardEvent>(this);
  getGameWorld()->unsubscribeEventsListener<InputActionToggleEvent>(this);

  m_inputModule->setMouseMovementMode(MouseMovementMode::Absolute);

  m_uiLayout.playerUILayout->removeChildWidget(m_uiLayout.interactionUI);
  m_uiLayout.playerUILayout->removeChildWidget(m_uiLayout.inventoryUI);
  m_uiLayout.playerUILayout->removeChildWidget(m_uiLayout.dialoguesUI);
  m_uiLayout.playerUILayout->removeChildWidget(m_uiLayout.hudLayout);

  // TODO: Reset active camera here, add default camera and switch to it in upper layers in this case
  // m_graphicsScene->setActiveCamera(nullptr);

}

void PlayerControlSystem::update(float delta)
{
  // TODO: getMouseDelta usage is unobvious and could lead to side effects. Fix it.
  auto mouseDeltaTemp = m_inputModule->getMouseDelta();

  if (isGUIWindowModeActive()) {
    if (m_activeGUIWindow == m_uiLayout.dialoguesUI) {
      m_uiLayout.dialoguesUI->updatePendingResponses();
    }
  }

  // TODO: what if player is not alive here? Fix it.
  updateHUD();

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
    glm::vec3(0.0f, playerComponent.getPlayerHeight(), 0.0f);

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

  processNearestInteractiveObjects(playerTransform);
}

void PlayerControlSystem::processNearestInteractiveObjects(const Transform& playerTransform)
{
  if (isGUIWindowModeActive()) {
    return;
  }

  GameObject interactiveObject = findNearestInteractiveObject(playerTransform);

  if (!interactiveObject.isFormed()) {
    if (m_uiLayout.interactionUI->isShown()) {
      m_uiLayout.interactionUI->hide();
    }

    return;
  }

  auto& interactiveObjectComponent = *interactiveObject.getComponent<InteractiveObjectComponent>().get();

  // TODO: don't hardcode the UI strings, use localization files instead
  if (interactiveObjectComponent.isUsable()) {
    m_uiLayout.interactionUIText->setText(
      fmt::format("{} - press F to interact", interactiveObjectComponent.getName()));
  }
  else if (interactiveObjectComponent.isTakeable()) {
    m_uiLayout.interactionUIText->setText(
      fmt::format("{} - press F to take", interactiveObjectComponent.getName()));
  }
  else if (interactiveObjectComponent.isTalkable()) {
    m_uiLayout.interactionUIText->setText(
      fmt::format("{} - press F to talk with", interactiveObjectComponent.getName()));
  }

  if (!m_uiLayout.interactionUI->isShown()) {
    m_uiLayout.interactionUI->show();
  }
}

Camera& PlayerControlSystem::getPlayerCamera()
{
  return *m_playerObject.getComponent<CameraComponent>()->getCamera();
}

void PlayerControlSystem::render()
{
  //DebugPainter::renderAABB(m_playerObject.getComponent<MeshRendererComponent>().getAABB());
}

EventProcessStatus PlayerControlSystem::receiveEvent(const MouseWheelEvent& event)
{
  auto& playerComponent = *m_playerObject.getComponent<PlayerComponent>().get();
  playerComponent.increaseDistanceToPlayer(event.wheelDelta * 0.2f);

  return EventProcessStatus::Processed;
}

EventProcessStatus PlayerControlSystem::receiveEvent(const InputActionToggleEvent& event)
{
  ARG_UNUSED(event);

  return EventProcessStatus::Processed;
}

void PlayerControlSystem::fixedUpdate(float delta)
{
  if (!m_isMovementControlEnabled || !m_playerObject.isAlive()) {
    return;
  }

  auto playerComponent = m_playerObject.getComponent<PlayerComponent>();

  auto& playerCameraTransform = *getPlayerCamera().getTransform();
  // auto& playerTransform = m_playerObject.getComponent<TransformComponent>().getTransform();

  glm::vec3 playerCameraFrontDirection = glm::normalize(playerCameraTransform.getFrontDirection() *
    glm::vec3{1.0f, 0.0f, 1.0f});
  glm::vec3 playerCameraRightDirection = glm::normalize(playerCameraTransform.getRightDirection() *
    glm::vec3{1.0f, 0.0f, 1.0f});

  float playerMovementSpeed = playerComponent->getMovementSpeed();

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

  m_inputModule->registerAction("hud_quests", KeyboardInputAction(SDLK_TAB));

  getGameWorld()->subscribeEventsListener<MouseWheelEvent>(this);
}

void PlayerControlSystem::disableMovementControl()
{
  m_isMovementControlEnabled = false;

  getGameWorld()->unsubscribeEventsListener<MouseWheelEvent>(this);

  m_inputModule->unregisterAction("hud_quests");
  m_inputModule->unregisterAction("forward");
  m_inputModule->unregisterAction("backward");
  m_inputModule->unregisterAction("left");
  m_inputModule->unregisterAction("right");

  auto playerKinematicCharacterComponent = m_playerObject.getComponent<KinematicCharacterComponent>();
  playerKinematicCharacterComponent->setPositionIncrement(glm::vec3(0.0f));

  auto playerAnimationComponent = m_playerObject.getComponent<SkeletalAnimationComponent>();
  playerAnimationComponent->getAnimationStatesMachineRef().setActiveState(m_idleAnimationStateId);
}

void PlayerControlSystem::showGUIWindow(const std::shared_ptr<GUILayout>& window)
{
  SW_ASSERT(m_activeGUIWindow == nullptr);

  if (m_uiLayout.interactionUI->isShown()) {
    m_uiLayout.interactionUI->hide();
  }

  disableMovementControl();
  m_inputModule->setMouseMovementMode(MouseMovementMode::Absolute);

  m_activeGUIWindow = window;

  window->show();
}

void PlayerControlSystem::hideGUIWindow()
{
  SW_ASSERT(m_activeGUIWindow != nullptr);

  m_activeGUIWindow->hide();

  m_inputModule->setMouseMovementMode(MouseMovementMode::Relative);
  enableMovementControl();

  m_activeGUIWindow.reset();
}

EventProcessStatus PlayerControlSystem::receiveEvent(const KeyboardEvent& event)
{
  if (event.type == KeyboardEventType::KeyDown) {
    if (event.keyCode == SDLK_i) {
      if (isGUIWindowModeActive() && m_activeGUIWindow == m_uiLayout.inventoryUI) {
        hideGUIWindow();
      }
      else {
        if (!isGUIWindowModeActive()) {
          showGUIWindow(m_uiLayout.inventoryUI);
        }
      }
    }
    else if (event.keyCode == SDLK_f) {
      if (!isGUIWindowModeActive()) {
        performInteractiveAction();
      }
    }
  }

  return EventProcessStatus::Processed;
}

GameObject PlayerControlSystem::findNearestInteractiveObject(const Transform& playerTransform)
{
  static std::vector<GameObject> nearestNeighbors;
  nearestNeighbors.reserve(64);
  nearestNeighbors.clear();

  m_graphicsScene->queryNearestDynamicNeighbors(playerTransform.getPosition(), 1.5f, nearestNeighbors);

  for (GameObject& object : nearestNeighbors) {
    if (object.hasComponent<InteractiveObjectComponent>()) {
      auto& interactiveObjectComponent = *object.getComponent<InteractiveObjectComponent>().get();

      if (interactiveObjectComponent.isUsable() || interactiveObjectComponent.isTakeable() ||
        interactiveObjectComponent.isTalkable()) {
        return object;
      }
    }
  }

  return GameObject();
}

void PlayerControlSystem::performInteractiveAction()
{
  GameObject interactiveObject = findNearestInteractiveObject(
    m_playerObject.getComponent<TransformComponent>()->getTransform());

  if (!interactiveObject.isFormed()) {
    return;
  }

  auto& interactiveObjectComponent = *interactiveObject.getComponent<InteractiveObjectComponent>().get();

  if (interactiveObjectComponent.isUsable()) {
    getGameWorld()->emitEvent(InteractiveObjectActionTriggeredEvent(
      m_playerObject, interactiveObject, InteractiveObjectActionType::Use));
  }
  else if (interactiveObjectComponent.isTakeable()) {
    getGameWorld()->emitEvent(InteractiveObjectActionTriggeredEvent(
      m_playerObject, interactiveObject, InteractiveObjectActionType::Take));
  }
  else if (interactiveObjectComponent.isTalkable()) {
    getGameWorld()->emitEvent(InteractiveObjectActionTriggeredEvent(
      m_playerObject, interactiveObject, InteractiveObjectActionType::Talk));
  }
}

bool PlayerControlSystem::isGUIWindowModeActive() const
{
  return m_activeGUIWindow != nullptr;
}

EventProcessStatus PlayerControlSystem::receiveEvent(
  const ActorDialogueStartCommandEvent& event)
{
  SW_ASSERT(!isGUIWindowModeActive());

  showGUIWindow(m_uiLayout.dialoguesUI);
  m_uiLayout.dialoguesUI->startDialogue(event.initiator, event.target);

  return EventProcessStatus::Processed;
}

EventProcessStatus PlayerControlSystem::receiveEvent(const StopDialogueCommandEvent& event)
{
  ARG_UNUSED(event);

  if (m_activeGUIWindow == m_uiLayout.dialoguesUI) {
    hideGUIWindow();
  }

  return EventProcessStatus::Processed;
}

void PlayerControlSystem::updateHUD()
{
//  if (m_inputModule->isActionActive("hud_quests")) {
  std::shared_ptr<QuestsStorage> questsStorage = m_uiLayout.dialoguesUI->getQuestsStorage();
  const auto* activeQuest = m_playerObject.getComponent<ActorComponent>()->getAnyActiveQuest();

  // TODO: do not update these texts each frame. Handle appropriate signals and
  //  update texts there

  if (activeQuest) {
    m_questInfoTitle->setText(questsStorage->getQuest(activeQuest->getQuestId()).getName());
    m_questInfoTaskTitle
      ->setText(questsStorage->getQuest(activeQuest->getQuestId()).getTask(activeQuest->getCurrentTaskId())
        .getName());
  }
  else {
    m_questInfoTitle->setText("No active quest");
    m_questInfoTaskTitle->setText("No active quest task");
  }

  if (!m_questInfoLayout->isShown()) {
    m_questInfoLayout->show();
  }

  auto playerActor = m_playerObject.getComponent<ActorComponent>();

  m_healthProgressBar->setValue(static_cast<int>(
    MathUtils::fractionToPercents(playerActor->getHealth(), playerActor->getHealthLimit())));

//  }
//  else {
//    if (!m_questInfoLayout->isShown()) {
//      m_questInfoLayout->show();
//    }
//  }

}
