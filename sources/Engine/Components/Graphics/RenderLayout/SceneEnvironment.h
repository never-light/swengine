#pragma once

#include <vector>

#include "Camera.h"
#include "Light.h"

class SceneEnvironment {
public:
	SceneEnvironment();
	~SceneEnvironment();

	void setActiveCamera(Camera* camera);
	Camera* getActiveCamera() const;

	void addLightSource(Light* lightSource);
	void removeLightSource(Light* lightSource);

	const std::vector<Light*>& getLightSources() const;
	size_t getLightSourcesCount() const;

private:
	Camera* m_activeCamera;
	std::vector<Light*> m_lightSources;
};