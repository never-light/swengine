#pragma once

#include <string>
#include <unordered_map>

#include <Engine\types.h>
#include <Engine\Components\Math\Math.h>
#include <Engine\Components\Graphics\GraphicsResourceFactory.h>
#include <Engine\Components\Graphics\RenderSystem\Light.h>
#include <Engine\Components\Graphics\RenderSystem\Camera.h>

#include "SceneObject.h"

class Scene {
public:
	Scene(GraphicsResourceFactory* graphicsResourceFactory);
	~Scene();

	void render();
	void update();

	Light* createLight(const std::string& name, Light::Type type);
	Light* getLight(const std::string& name);

	void setActiveCamera(Camera* camera);
	Camera* getActiveCamera() const;

	Camera* createCamera(const std::string&);
	Camera* getCamera(const std::string&);

	void registerSceneObject(SceneObject* sceneObject);

	SceneObject* findSceneObject(SceneObjectId id);
	SceneObject* findSceneObject(const std::string& name);

	void removeSceneObject(SceneObject* object);

private:
	SceneObjectId generateSceneObjectId();

private:
	SceneObjectId m_maxSceneObjectId;

	Camera* m_activeCamera;

	std::unordered_map<std::string, Light*> m_lights;
	std::unordered_map<std::string, Camera*> m_cameras;

	std::unordered_map<SceneObjectId, SceneObject*> m_objects;

private:
	GraphicsResourceFactory* m_graphicsResourceFactory;
};