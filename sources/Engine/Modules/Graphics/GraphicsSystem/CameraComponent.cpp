#include "precompiled.h"

#pragma hdrstop

#include "CameraComponent.h"

#include <utility>

#include "Modules/ECS/ECS.h"

CameraComponent::CameraComponent()
{

}

void CameraComponent::setCamera(std::shared_ptr<Camera> camera)
{
  m_camera = std::move(camera);
}

std::shared_ptr<Camera> CameraComponent::getCamera() const
{
  return m_camera;
}

CameraComponent::BindingParameters CameraComponent::getBindingParameters() const
{
  return CameraComponent::BindingParameters{
    .position = m_camera->getTransform()->getPosition(),
    .lookAtPoint = m_camera->getTransform()->getPosition() + m_camera->getTransform()->getFrontDirection(),
    .nearDistance = m_camera->getNearClipDistance(),
    .farDistance = m_camera->getFarClipDistance(),
    .fov = glm::degrees(m_camera->getFOVy()),
  };
}

CameraComponentBinder::CameraComponentBinder(const ComponentBindingParameters& componentParameters)
  : m_bindingParameters(componentParameters)
{

}

void CameraComponentBinder::bindToObject(GameObject& gameObject)
{
  std::shared_ptr<Camera> camera = std::make_shared<Camera>();

  camera->getTransform()->setPosition(m_bindingParameters.position);
  camera->getTransform()->lookAt(m_bindingParameters.lookAtPoint);

  camera->setNearClipDistance(m_bindingParameters.nearDistance);
  camera->setFarClipDistance(m_bindingParameters.farDistance);
  camera->setFOVy(glm::radians(m_bindingParameters.fov));

  auto& cameraComponent = *gameObject.addComponent<CameraComponent>().get();
  cameraComponent.setCamera(camera);
}
