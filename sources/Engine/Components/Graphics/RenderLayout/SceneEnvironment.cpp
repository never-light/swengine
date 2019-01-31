#include "SceneEnvironment.h"

#include <algorithm>

SceneEnvironment::SceneEnvironment()
	: m_activeCamera(nullptr)
{
}

SceneEnvironment::~SceneEnvironment()
{
}

void SceneEnvironment::setActiveCamera(Camera * camera)
{
	m_activeCamera = camera;
}

Camera * SceneEnvironment::getActiveCamera() const
{
	return m_activeCamera;
}

void SceneEnvironment::addLightSource(Light * lightSource)
{
	m_lightSources.push_back(lightSource);
}

void SceneEnvironment::removeLightSource(Light * lightSource)
{
	m_lightSources.erase(std::remove(m_lightSources.begin(), 
		m_lightSources.end(), lightSource));
}

const std::vector<Light*>& SceneEnvironment::getLightSources() const
{
	return m_lightSources;
}
