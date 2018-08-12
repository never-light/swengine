#pragma once

#include <string>
#include <unordered_map>

#include <Engine\types.h>
#include <Engine\Components\Math\Math.h>
#include <Engine\Components\Graphics\RenderSystem\Camera.h>
#include <Engine\Components\Graphics\RenderSystem\GraphicsContext.h>
#include <Engine\Components\ResourceManager\ResourceManager.h>

#include "SceneObject.h"

using SceneId = size_t;

class Scene {
public:
	Scene(GraphicsContext* graphicsContext, ResourceManager* resourceManager);
	virtual ~Scene();

	void onRegister(SceneId id);

	virtual void render() = 0;
	virtual void update() = 0;

	virtual void activate();
	virtual void deactivate();

	virtual void setActiveCamera(Camera* camera);
	Camera* getActiveCamera() const;

	Camera* createCamera(const std::string&);
	Camera* getCamera(const std::string&);

	SceneObjectId registerSceneObject(SceneObject* sceneObject);

	SceneObject* findSceneObject(SceneObjectId id);
	SceneObject* findSceneObject(const std::string& name);

	void removeSceneObject(SceneObject* object);

	SceneId getId() const;
private:
	SceneObjectId generateSceneObjectId();

protected:
	GraphicsContext* m_graphicsContext;
	ResourceManager* m_resourceManager;

private:
	SceneId m_id;

	SceneObjectId m_maxSceneObjectId;

	Camera* m_activeCamera;

	std::unordered_map<std::string, Camera*> m_cameras;
	std::unordered_map<SceneObjectId, SceneObject*> m_objects;
};