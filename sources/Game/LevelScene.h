#pragma once

#include <Engine/Components/SceneManager/Scene.h>
#include <Engine/Components/InputManager/InputManager.h>
#include <Engine/Components/GUI/GUIManager.h>

#include <Engine/Components/Graphics/Animation/Animation.h>
#include <Engine/Components/Graphics/Animation/Animator.h>
#include <Engine/Components/Graphics/RenderLayout/Layout/MeshRenderingSystem.h>
#include <Engine/Components/Graphics/RenderLayout/Layout/PostProcessingSystem.h>
#include <Engine/Components/Graphics/RenderLayout/Layout/MeshComponent.h>

#include <Engine/Components/ECS/ECS.h>


#include <Game/Console/Console.h>

#include <Game/Game/Inventory/InventoryViewer.h>

#include <Game/Game/InfoportionsStore.h>
#include <Game/Game/Tasks/TaskManager.h>
#include <Game/Game/Time/TimeManager.h>

#include <Game/Game/HUD/GameHUD.h>

class LevelScene : public Scene, public InputEventsListener {
public:
	LevelScene(GraphicsContext* graphicsContext, 
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

	void initializeInfoportions();
	void initializeTasks();
	void initializeGUI();

	void startGame();
private:
	void changeCurrentTaskCallback(const Task* newCurrentTask);

	void changeCameraCommandHandler(Console* console, const std::vector<std::string>& args);
	void changeGammaCorrectionCommandHandler(Console* console, const std::vector<std::string>& args);
	void pickPositionCommandHandler(Console* console, const std::vector<std::string>& args);
	void pickDirectionCommandHandler(Console* console, const std::vector<std::string>& args);

protected:
	GUILayout * m_levelGUILayout;

protected:
	SceneEnvironment* m_sceneEnvironment;

	GameWorld* m_gameWorld;
	GraphicsPipeline* m_graphicsPipeline;
	MeshRenderingSystem* m_meshRenderingSystem;
	PostProcessingSystem* m_postProcessingSystem;

	PBRMaterial* m_testMaterial;

protected:
	InfoportionsStore * m_infoportionsStore;
	TaskManager* m_taskManager;
	TimeManager* m_timeManager;

	GameHUD* m_hud;

	GUIText* m_winText;

protected:
	Camera* m_playerCamera;
	Camera* m_freeCamera;

protected:
	GpuProgram* m_deferredGeometryPassProgram;
	GpuProgram* m_deferredLigthingPassProgram;

	GpuProgram* m_boundingVolumeGpuProgram;

	GpuProgram* m_brightFilterProgram;
	GpuProgram* m_blurProgram;
	GpuProgram* m_postProcessingProgram;

protected:
	InputManager* m_inputManager;
	GUIManager* m_guiManager;
};