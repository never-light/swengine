#pragma once

#include <Engine\Components\SceneManager\Scene.h>
#include <Engine\Components\InputManager\InputManager.h>
#include <Engine\Components\GUI\GUIManager.h>

#include <Game\Graphics\Primitives\BoxPrimitive.h>
#include <Game\Graphics\Animation\Animation.h>
#include <Game\Graphics\Animation\Animator.h>
#include <Game\Console\Console.h>

#include <Game\Graphics\LevelRenderer.h>
#include <Game\Graphics\Materials\PhongLightingMaterial.h>
#include <Game\Game\GameObjectsStore.h>

#include <Game\Game\Inventory\InventoryViewer.h>

#include "SolidGameObject.h"
#include "PlayerController.h"
#include "FreeCameraController.h"

class LevelScene : public Scene, public InputEventsListener {
public:
	LevelScene(GraphicsContext* graphicsContext, 
		GraphicsResourceFactory* graphicsResourceFactory,
		ResourceManager* resourceManager, 
		InputManager* inputManager,
		GUIManager* guiManager,
		Console* console);

	virtual ~LevelScene();

	virtual void update() override;
	virtual void render() override;

	virtual void setActiveCamera(Camera* camera);

	virtual void activate() override;
	virtual void deactivate() override;
protected:
	void loadResources();
	void initializeSceneObjects();
	void initializePrimitives();

	void initializePlayer();
	void initializeFreeCamera();
private:
	void removeGameObjectCallback(GameObject* object);
	void registerGameObjectCallback(GameObject* object);
	void relocateGameObjectCallback(GameObject* object, GameObject::Location oldLocation, GameObject::Location newLocation);

	void changeCameraCommandHandler(Console* console, const std::vector<std::string>& args);
	void changeGammaCorrectionCommandHandler(Console* console, const std::vector<std::string>& args);
	void pickPositionCommandHandler(Console* console, const std::vector<std::string>& args);
	void pickDirectionCommandHandler(Console* console, const std::vector<std::string>& args);

protected:
	GUILayout * m_levelGUILayout;

protected:
	GameObjectsStore * m_gameObjectsStore;
	PhongLightingMaterial * m_phongLightingBaseMaterial;

	LevelRenderer * m_levelRenderer;

protected:
	std::vector<Light*> m_lights;

	PlayerController* m_playerController;
	Player* m_player;
	Camera* m_playerCamera;
	SolidMesh* m_playerMesh;

	FreeCameraController* m_freeCameraController;
	Camera* m_freeCamera;

	SolidGameObject* m_level;
	SolidMesh* m_levelMesh;

	InputController* m_activeInputController;
protected:
	GpuProgram* m_deferredLightingProgram;
	GpuProgram* m_lightingGpuProgram;
	GpuProgram* m_boundingVolumeGpuProgram;

	bool m_isCollision = false;
protected:
	BoxPrimitive* m_boxPrimitive;

protected:
	InputManager* m_inputManager;
	GraphicsResourceFactory* m_graphicsResourceFactory;
	GUIManager* m_guiManager;
};