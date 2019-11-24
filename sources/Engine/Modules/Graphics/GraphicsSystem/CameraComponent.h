#pragma once

#include <memory>
#include "Camera.h"

class CameraComponent
{
public:
    CameraComponent();

    void setCamera(std::shared_ptr<Camera> camera);
    std::shared_ptr<Camera> getCamera() const;

private:
    std::shared_ptr<Camera> m_camera;
};

