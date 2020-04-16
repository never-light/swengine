#include "precompiled.h"

#pragma hdrstop

#include "CameraComponent.h"

CameraComponent::CameraComponent() {

}

void CameraComponent::setCamera(std::shared_ptr<Camera> camera) {
  m_camera = camera;
}

std::shared_ptr<Camera> CameraComponent::getCamera() const {
  return m_camera;
}
