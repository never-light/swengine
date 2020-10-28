#pragma once

#include <SDL.h>
#include <string>
#include <memory>

#include "Modules/Graphics/GraphicsModule.h"
#include "Modules/ResourceManagement/ResourceManagementModule.h"
#include "Modules/Input/InputSystem.h"
#include "Modules/ECS/ECS.h"

#include "Modules/Graphics/GraphicsSystem/TransformComponent.h"
#include "Modules/Graphics/GraphicsSystem/MeshRendererComponent.h"
#include "Modules/Graphics/GraphicsSystem/FrameStats.h"
#include "Modules/Graphics/GraphicsSystem/DebugPainter.h"

#include "Modules/Graphics/GraphicsSystem/Animation/SkeletalAnimationSystem.h"

#include "Modules/Graphics/GraphicsSystem/RenderingSystemsPipeline.h"
#include "Modules/Graphics/GraphicsSystem/MeshRenderingSystem.h"
#include "Modules/Graphics/GraphicsSystem/GraphicsSceneManagementSystem.h"
#include "Modules/Graphics/GraphicsSystem/EnvironmentRenderingSystem.h"

#include "Modules/Physics/PhysicsSystem.h"
#include "Modules/Audio/AudioSystem.h"

#include "Modules/Graphics/GUI/GUISystem.h"
#include "Modules/ScreenManagement/ScreenManager.h"
#include "Modules/LevelsManagement/LevelsManager.h"

#include "GameConsole.h"

class BaseGameApplication : public EventsListener<GameConsoleCommandEvent>,
                            public EventsListener<InputActionToggleEvent> {
 public:
  BaseGameApplication(int argc, char* argv[], const std::string& windowTitle);
  ~BaseGameApplication() override;

  virtual void load();
  virtual void unload();

  virtual void update(float delta);
  virtual void render();

  virtual int execute();

  EventProcessStatus receiveEvent(const GameConsoleCommandEvent& event) override;
  EventProcessStatus receiveEvent(const InputActionToggleEvent& event) override;

  void shutdown();

 protected:
  [[nodiscard]] std::shared_ptr<GameSystemsGroup> getGameApplicationSystemsGroup() const;

 private:
  void initializePlatform(int argc, char* argv[], const std::string& windowTitle);
  void initializeEngine();
  void initializeEngineSystems();

  void performLoad();
  void performUnload();

  void performUpdate(float delta);
  void performRender();

  static void handleAppTerminate();

 protected:
  SDL_Window* m_mainWindow;

  std::shared_ptr<GraphicsModule> m_graphicsModule;
  std::shared_ptr<ResourceManagementModule> m_resourceManagementModule;
  std::shared_ptr<InputModule> m_inputModule;
  std::shared_ptr<InputSystem> m_inputSystem;

  std::shared_ptr<GraphicsScene> m_graphicsScene;

  std::shared_ptr<GameWorld> m_gameWorld;
  std::shared_ptr<RenderingSystemsPipeline> m_renderingSystemsPipeline;
  std::shared_ptr<MeshRenderingSystem> m_meshRenderingSystem;

  std::shared_ptr<GUISystem> m_guiSystem;

  std::shared_ptr<PhysicsSystem> m_physicsSystem;
  std::shared_ptr<AudioSystem> m_audioSystem;

  std::shared_ptr<GameSystemsGroup> m_engineGameSystems;
  std::shared_ptr<GameSystemsGroup> m_gameApplicationSystems;

  std::shared_ptr<ScreenManager> m_screenManager;
  std::shared_ptr<LevelsManager> m_levelsManager;

  std::shared_ptr<GameConsole> m_gameConsole;

  bool m_isMainLoopActive = false;
};
