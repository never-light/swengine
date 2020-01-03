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

const FrameStats& SharedGraphicsState::getFrameStats() const
{
    return m_frameStats;
}

FrameStats& SharedGraphicsState::getFrameStats()
{
    return m_frameStats;
}

void FrameStats::reset()
{
    m_primitivesCount = 0;
    m_subMeshesCount = 0;
    m_culledSubMeshesCount = 0;
}

void FrameStats::increasePrimitivesCount(size_t count)
{
    m_primitivesCount += count;
}

void FrameStats::increaseSubMeshesCount(size_t count)
{
    m_subMeshesCount += count;
}

void FrameStats::increaseCulledSubMeshesCount(size_t count)
{
    m_culledSubMeshesCount += count;
}

size_t FrameStats::getPrimivitesCount() const
{
    return m_primitivesCount;
}

size_t FrameStats::getSubMeshesCount() const
{
    return m_subMeshesCount;
}

size_t FrameStats::getCulledSubMeshesCount() const
{
    return m_culledSubMeshesCount;
}
