#include "SharedGraphicsState.h"

SharedGraphicsState::SharedGraphicsState()
{

}

void SharedGraphicsState::setActiveCamera(std::shared_ptr<Camera> camera)
{
    m_activeCamera = camera;
}

std::shared_ptr<Camera> SharedGraphicsState::getActiveCamera() const
{
    return m_activeCamera;
}
