#pragma once

#include <string>
#include <unordered_map>

#include <Engine\types.h>
#include <Engine\Components\Math\Math.h>
#include <Engine\Components\Graphics\RenderSystem\Light.h>
#include <Engine\Components\Graphics\RenderSystem\Camera.h>
#include <Engine\Components\Graphics\RenderSystem\Model.h>

#include <Engine\Components\ResourceManager\ResourceManager.h>

#include "SceneNode.h"

class Scene {
public:
	Scene();
	~Scene();

	Light* createLight(const std::string& name, LightType type);
	Light* getLight(const std::string& name);

	void setActiveCamera(const std::string& name);
	void setActiveCamera(Camera* camera);
	Camera* getActiveCamera() const;

	Camera* createCamera(const std::string&);
	Camera* getCamera(const std::string&);

	Model* createModel(const std::string& filename, const std::string& name);
	Model* createModel(Mesh* mesh, const std::string& name);
	Model* getModel(const std::string& name);

	SceneNode* getRootSceneNode() const;
private:
	Camera* m_activeCamera;

	std::unordered_map<std::string, Light*> m_lightsMap;
	std::unordered_map<std::string, Camera*> m_camerasMap;
	std::unordered_map<std::string, Model*> m_modelsMap;

	SceneNode* m_rootSceneNode;
};