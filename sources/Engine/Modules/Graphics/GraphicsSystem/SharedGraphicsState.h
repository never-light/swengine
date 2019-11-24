#pragma once

#include "Camera.h"

class SharedGraphicsState
{
public:
    SharedGraphicsState();

    void setActiveCamera(std::shared_ptr<Camera> camera);
    std::shared_ptr<Camera> getActiveCamera() const;

private:
    std::shared_ptr<Camera> m_activeCamera;
};

