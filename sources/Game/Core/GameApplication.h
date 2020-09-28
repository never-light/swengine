#pragma once

#include <Engine/Modules/Application/BaseGameApplication.h>
#include <Engine/Modules/Graphics/OpenGL/GLGeometryStore.h>
#include <Engine/Modules/Graphics/OpenGL/GLShadersPipeline.h>
#include <Engine/Modules/Graphics/GraphicsSystem/Mesh.h>

#include <Engine/Modules/ScreenManagement/ScreenManager.h>

#include "Game/ComponentsLoader/GameComponentsLoader.h"
#include "Game/Dynamic/DialoguesManager.h"

class GameApplication : public BaseGameApplication,
                        public EventsListener<ScreenSwitchEvent> {
 public:
  GameApplication(int argc, char* argv[]);
  ~GameApplication() override;

  void render() override;

  void load() override;
  void unload() override;

 public:
  EventProcessStatus receiveEvent(GameWorld* gameWorld, const ScreenSwitchEvent& event) override;

 private:
  std::unique_ptr<GameComponentsLoader> m_componentsLoader;
  std::shared_ptr<DialoguesManager> m_dialoguesManager;
};

