#include "SceneManager.h"
#include <Engine/assertions.h>

SceneManager::SceneManager() 
	: m_maxSceneId(0), m_activeScene(nullptr)
{
	
}

SceneManager::~SceneManager() {

}

void SceneManager::update()
{
	if (m_activeScene != nullptr)
		m_activeScene->update();
}

void SceneManager::render()
{
	if (m_activeScene != nullptr)
		m_activeScene->render();
}

SceneId SceneManager::registerScene(Scene* scene) {
	_assert(scene != nullptr && scene->getId() == 0);

	scene->onRegister(generateSceneId());
	m_scenes.insert({ scene->getId(), scene });

	return scene->getId();
}

SceneId SceneManager::generateSceneId()
{
	m_maxSceneId++;

	return m_maxSceneId;
}

Scene* SceneManager::getScene(SceneId id) const {
	return m_scenes.at(id);
}

void SceneManager::setActiveScene(SceneId id)
{
	if (m_activeScene != nullptr)
		m_activeScene->deactivate();

	if (id == 0)
		m_activeScene = nullptr;
	else {
		m_activeScene = m_scenes.at(id);
		m_activeScene->activate();
	}
}

Scene * SceneManager::getActiveScene() const
{
	return m_activeScene;
}
