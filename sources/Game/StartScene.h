#pragma once

#include <Engine\Components\SceneManager\Scene.h>
#include <Engine\Components\InputManager\InputManager.h>

#include <Game\Graphics\Primitives\BoxPrimitive.h>
#include <Game\Graphics\Animation\Animation.h>
#include <Game\Graphics\Animation\Animator.h>
#include <Game\Console\Console.h>

#include <Game\Graphics\LevelRenderer.h>
#include <Game\Graphics\Materials\PhongLightingMaterial.h>

#include "SolidGameObject.h"
#include "PlayerController.h"
#include "FreeCameraController.h"

class StartScene : public Scene, public InputEventsListener {
public:
	StartScene(GraphicsContext* graphicsContext, 
		GraphicsResourceFactory* graphicsResourceFactory,
		ResourceManager* resourceManager, 
		InputManager* inputManager,
		Console* console);

	virtual ~StartScene();

	virtual void update() override;
	virtual void render() override;

	virtual void setActiveCamera(Camera* camera);
protected:
	void loadResources();
	void initializeSceneObjects();
	void initializePrimitives();

private:
	void changeCameraCommandHandler(Console* console, const std::vector<std::string>& args);
	void changeGammaCorrectionCommandHandler(Console* console, const std::vector<std::string>& args);
	void pickPositionCommandHandler(Console* console, const std::vector<std::string>& args);
	void pickDirectionCommandHandler(Console* console, const std::vector<std::string>& args);

protected:
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
	GpuProgram * m_deferredLightingProgram;
	GpuProgram* m_lightingGpuProgram;
	GpuProgram* m_boundingVolumeGpuProgram;

	bool m_isCollision = false;
protected:
	BoxPrimitive* m_boxPrimitive;

protected:
	InputManager* m_inputManager;
	GraphicsResourceFactory* m_graphicsResourceFactory;
};