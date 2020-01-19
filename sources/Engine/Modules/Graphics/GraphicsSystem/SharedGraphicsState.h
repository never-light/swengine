#pragma once

#include "Modules/Graphics/OpenGL/GLFramebuffer.h"
#include "Camera.h"

struct FrameStats {
public:
    FrameStats() = default;

    void reset();

    void increasePrimitivesCount(size_t count);
    void increaseSubMeshesCount(size_t count);
    void increaseCulledSubMeshesCount(size_t count);

    size_t getPrimivitesCount() const;
    size_t getSubMeshesCount() const;
    size_t getCulledSubMeshesCount() const;

private:
    size_t m_primitivesCount = 0;

    size_t m_subMeshesCount = 0;
    size_t m_culledSubMeshesCount = 0;
};

class SharedGraphicsState
{
public:
    SharedGraphicsState();

    void setActiveCamera(std::shared_ptr<Camera> camera);
    std::shared_ptr<Camera> getActiveCamera() const;

    const FrameStats& getFrameStats() const;
    FrameStats& getFrameStats();

private:
    std::shared_ptr<Camera> m_activeCamera;

    FrameStats m_frameStats;

    std::unique_ptr<GLFramebuffer> m_deferredFramebuffer;
    std::unique_ptr<GLFramebuffer> m_forwardFramebuffer;
};

