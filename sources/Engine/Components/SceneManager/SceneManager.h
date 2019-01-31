#pragma once

#include <string>
#include <unordered_map>

#include <Engine/types.h>
#include <Engine/Components/Math/Math.h>
#include <Engine/Components/ResourceManager/ResourceManager.h>
#include "Scene.h"

class SceneManager {
public:
	SceneManager();
	virtual ~SceneManager();

	virtual void update();
	virtual void render();

	SceneId registerScene(Scene* scene);
	Scene* getScene(SceneId id) const;

	void setActiveScene(SceneId id);
	Scene* getActiveScene() const;

private:
	SceneId generateSceneId();

private:
	SceneId m_maxSceneId;

	Scene* m_activeScene;
	std::unordered_map<SceneId, Scene*> m_scenes;
};