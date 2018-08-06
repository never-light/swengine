#pragma once

#include <Engine\Components\SceneManager\Scene.h>
#include <Engine\Components\InputManager\InputManager.h>

#include <Game\Graphics\Primitives\BoxPrimitive.h>

#include "SolidGameObject.h"
#include "PlayerController.h"

class StartScene : public Scene, public InputEventsListener {
public:
	StartScene(GraphicsContext* graphicsContext, GraphicsResourceFactory* graphicsResourceFactory, ResourceManager* resourceManager, InputManager* inputManager);
	virtual ~StartScene();

	virtual void update() override;
	virtual void render() override;

protected:
	void loadResources();
	void initializeSceneObjects();
	void initializePrimitives();

protected:
	SolidGameObject* m_level;

	Player* m_player;
	PlayerController* m_playerController;
	Camera* m_playerCamera; 

	SolidMesh* m_levelMesh;
	SolidMesh* m_playerMesh;
protected:
	GpuProgram* m_lightingGpuProgram;
	GpuProgram* m_boundingVolumeGpuProgram;

	bool m_isCollision = false;
protected:
	BoxPrimitive* m_boxPrimitive;

protected:
	InputManager* m_inputManager;
	GraphicsResourceFactory* m_graphicsResourceFactory;
};