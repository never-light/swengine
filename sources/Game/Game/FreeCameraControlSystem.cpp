#include "FreeCameraControlSystem.h"

#include <utility>

#include <Engine/Modules/Graphics/GraphicsSystem/TransformComponent.h>
#include <Engine/Modules/Graphics/GraphicsSystem/SharedGraphicsState.h>

FreeCameraControlSystem::FreeCameraControlSystem(
  std::shared_ptr<InputModule> inputModule,
  std::shared_ptr<GraphicsScene> graphicsScene)
  : m_inputModule(std::move(inputModule)),
    m_graphicsScene(std::move(graphicsScene)),
    m_freeCamera(std::make_shared<Camera>())
{
  m_freeCamera->setNearClipDistance(0.1f);
  m_freeCamera->setFarClipDistance(100.0f);
  // TODO: replace usage of deferred frame buffer here with something like settings class
  m_freeCamera->setAspectRatio(
    m_graphicsScene->getSharedGraphicsState()->getDeferredFramebuffer().getAspectRatio());
  m_freeCamera->setFOVy(glm::pi<float>() / 4);

  m_freeCamera->getTransform()->setPosition(0, 0, 0);
  m_freeCamera->getTransform()->lookAt(0, 0, 1);

}

void FreeCameraControlSystem::configure()
{
}

void FreeCameraControlSystem::unconfigure()
{
}

void FreeCameraControlSystem::activate()
{
  m_inputModule->registerAction("forward", KeyboardInputAction(SDLK_w));
  m_inputModule->registerAction("backward", KeyboardInputAction(SDLK_s));
  m_inputModule->registerAction("left", KeyboardInputAction(SDLK_a));
  m_inputModule->registerAction("right", KeyboardInputAction(SDLK_d));

  m_inputModule->enableGlobalTracking();
  m_inputModule->setMouseMovementMode(MouseMovementMode::Relative);

  getGameWorld()->subscribeEventsListener<InputActionToggleEvent>(this);

  m_graphicsScene->setActiveCamera(m_freeCamera);

}

void FreeCameraControlSystem::deactivate()
{
  // TODO: Reset active camera here, add default camera and switch to it in upper layers in this case
  // m_sharedGraphicsState->setActiveCamera(nullptr);

  getGameWorld()->unsubscribeEventsListener<InputActionToggleEvent>(this);

  m_inputModule->unregisterAction("forward");
  m_inputModule->unregisterAction("backward");
  m_inputModule->unregisterAction("left");
  m_inputModule->unregisterAction("right");

  m_inputModule->setMouseMovementMode(MouseMovementMode::Absolute);
}

void FreeCameraControlSystem::update(float delta)
{
  auto mouseDeltaTemp = m_inputModule->getMouseDelta();
  glm::vec2 mouseDelta(mouseDeltaTemp.x, mouseDeltaTemp.y);

  mouseDelta *= -0.25;

  m_freeCamera->getTransform()->pitchLocal(mouseDelta.y);
  m_freeCamera->getTransform()->yawGlobal(mouseDelta.x);

  glm::vec3 playerCameraFrontDirection = m_freeCamera->getTransform()->getFrontDirection();
  glm::vec3 playerCameraRightDirection = m_freeCamera->getTransform()->getRightDirection();

  glm::vec3 movementDirection{};

  if (m_inputModule->isActionActive("forward")) {
    movementDirection += playerCameraFrontDirection;
  }

  if (m_inputModule->isActionActive("backward")) {
    movementDirection += playerCameraFrontDirection * (-1.0f);
  }

  if (m_inputModule->isActionActive("right")) {
    movementDirection += playerCameraRightDirection;
  }

  if (m_inputModule->isActionActive("left")) {
    movementDirection += playerCameraRightDirection * (-1.0f);
  }

  if (glm::length(movementDirection) >= 1.0f) {
    movementDirection = glm::normalize(movementDirection);
  }

  m_freeCamera->getTransform()->move(movementDirection * MOVEMENT_SPEED * delta);
}

EventProcessStatus FreeCameraControlSystem::receiveEvent(GameWorld* gameWorld, const InputActionToggleEvent& event)
{
  ARG_UNUSED(gameWorld);
  ARG_UNUSED(event);

  return EventProcessStatus::Processed;
}
