#pragma once

#include <memory>
#include <string>

#include "Modules/Graphics/GUI/GUISystem.h"
#include "Modules/Graphics/GUI/GUILayout.h"
#include "Modules/Graphics/GraphicsModule.h"
#include "Modules/Graphics/GraphicsSystem/SharedGraphicsState.h"
#include "Modules/ECS/ECS.h"
#include "Modules/ResourceManagement/ResourceManager.h"

class ScreenManager;

class Screen {
 public:
  explicit Screen(const std::string& name);
  virtual ~Screen();

  [[nodiscard]] std::string getName() const;

  virtual void load();
  virtual void unload();

  virtual void activate();
  virtual void deactivate();

  virtual void update(float delta);
  virtual void render();

  [[nodiscard]] std::shared_ptr<GUILayout> getGUILayout() const;

  [[nodiscard]] bool isActive() const;

 protected:
  void activateNextScreen(const std::string& name);

 private:
  void performInternalInitialization(std::shared_ptr<ScreenManager> screenManager,
                                     std::shared_ptr<GameWorld> gameWorld,
                                     std::shared_ptr<GraphicsModule> graphicsModule,
                                     std::shared_ptr<SharedGraphicsState> sharedGraphicsState,
                                     std::shared_ptr<ResourceManager> resourceManager);

  void performActivate();
  void performDeactivate();

 protected:
  std::shared_ptr<GameWorld> m_gameWorld;
  std::shared_ptr<GraphicsModule> m_graphicsModule;
  std::shared_ptr<SharedGraphicsState> m_sharedGraphicsState;
  std::shared_ptr<ResourceManager> m_resourceManager;

 private:
  std::string m_name;

  std::weak_ptr<ScreenManager> m_screenManager;
  std::shared_ptr<GUILayout> m_guiLayout;

  bool m_isActive = false;

 private:
  friend class ScreenManager;
};
