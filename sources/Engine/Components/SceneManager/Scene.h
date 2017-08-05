#pragma once

#include <string>
#include <unordered_map>

#include <Engine\types.h>
#include <Engine\Components\Math\Math.h>
#include <Engine\Components\Graphics\Light.h>
#include <Engine\Components\Graphics\Camera.h>

#include "SceneNode.h"

class Scene {
public:
	Scene();
	~Scene();

	void initialize();
	void shutdown();

	template<typename LightSourceType>
	LightSourceType* createLightSource(const std::string& name) {
		LightSourceType* lightSource = new LightSourceType();
		m_lightsMap.insert(std::make_pair(name, dynamic_cast<Light*>(lightSource)));

		return lightSource;
	}

	template<typename LightSourceType>
	LightSourceType* getLightSource(const std::string& name) {
		return dynamic_cast<LightSourceType*>(m_lightsMap.at(name));
	}

	Camera* createCamera(const std::string&);
	Camera* getCamera(const std::string&);

	SceneNode* getRootSceneNode() const;
private:
	std::unordered_map<std::string, Light*> m_lightsMap;
	std::unordered_map<std::string, Camera*> m_camerasMap;

	SceneNode* m_rootSceneNode;
};