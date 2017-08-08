#pragma once

#include <string>
#include <unordered_map>

#include <Engine\types.h>
#include <Engine\Components\Math\Math.h>
#include <Engine\Components\Graphics\Light.h>
#include <Engine\Components\Graphics\Camera.h>
#include <Engine\Components\Graphics\Model.h>

#include <Engine\Components\ResourceManager\ResourceManager.h>

#include "SceneNode.h"

class Scene {
public:
	Scene();
	~Scene();

	void initialize(ResourceManager* resourceManager);
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

	Model* createModel(const std::string& filename, const std::string& name);
	Model* getModel(const std::string& name);

	SceneNode* getRootSceneNode() const;
private:
	std::unordered_map<std::string, Light*> m_lightsMap;
	std::unordered_map<std::string, Camera*> m_camerasMap;
	std::unordered_map<std::string, Model*> m_modelsMap;

	SceneNode* m_rootSceneNode;

private:
	ResourceManager* m_resourceManager;
};