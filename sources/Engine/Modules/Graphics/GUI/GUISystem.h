#pragma once

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Modules/ECS/ECS.h"
#include "Modules/Input/InputModule.h"
#include "Modules/Input/InputEvents.h"

#include "Modules/Graphics/OpenGL/GLShadersPipeline.h"
#include "Modules/Graphics/OpenGL/GLGraphicsContext.h"
#include "Modules/Graphics/GraphicsSystem/Mesh.h"

#include "GUILayout.h"
#include "GUIButton.h"
#include "GUIImage.h"
#include "GUIText.h"
#include "GUITextBox.h"
#include "GUIProgressBar.h"

#include "GUIWidgetsLoader.h"

class GUISystem : public std::enable_shared_from_this<GUISystem>,
                  public GameSystem,
                  public EventsListener<MouseButtonEvent>,
                  public EventsListener<KeyboardEvent> {
 public:
  GUISystem(std::shared_ptr<InputModule> inputModule,
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<GLGraphicsContext> graphicsContext,
    std::shared_ptr<GLShadersPipeline> guiShadersPipeline);

  void configure() override;
  void unconfigure() override;

  void update(float delta) override;
  void render() override;

  void setActiveLayout(std::shared_ptr<GUILayout> layout);
  [[nodiscard]] std::shared_ptr<GUILayout> getActiveLayout();

  void setDefaultFont(std::shared_ptr<BitmapFont> font);
  [[nodiscard]] std::shared_ptr<BitmapFont> getDefaultFont() const;

  [[nodiscard]] std::shared_ptr<GLGraphicsContext> getGraphicsContext() const;
  [[nodiscard]] RenderTask getRenderTaskTemplate(GUIWidget* widget) const;

  [[nodiscard]] int getScreenWidth() const;
  [[nodiscard]] int getScreenHeight() const;

  EventProcessStatus receiveEvent(const MouseButtonEvent& event) override;
  EventProcessStatus receiveEvent(const KeyboardEvent& event) override;

  // TODO: the method suppose that default parent of the layout is the root layout, but
  //  it is not common case. Add ability to specify custom default parent layout.
  [[nodiscard]] std::shared_ptr<GUILayout> loadScheme(const std::string& schemePath);

  [[nodiscard]] GUIWidgetStylesheet loadStylesheet(const std::string& stylesheetPath);

  [[nodiscard]] GUIWidgetsLoader* getWidgetsLoader() const;

 private:
  void updateGUIWidget(GUIWidget* widget);
  void processGUIWidgetMouseButtonEvent(GUIWidget* widget, const MouseButtonEvent& event);

  bool isMouseInWidgetArea(const GUIWidget* widget) const;

  void renderGUIWidget(GUIWidget* widget);

  void executeEventsQueue(const std::vector<std::function<void()>>& queue);

 private:
  std::unique_ptr<Mesh> m_guiNDCQuad;

  std::shared_ptr<GUILayout> m_activeLayout;

  std::shared_ptr<InputModule> m_inputModule;
  std::shared_ptr<ResourceManager> m_resourceManager;

  std::shared_ptr<GLGraphicsContext> m_graphicsContext;
  std::shared_ptr<GLShadersPipeline> m_guiShadersPipeline;
  std::shared_ptr<BitmapFont> m_defaultFont;

  std::shared_ptr<GUIWidget> m_focusedWidget;

  glm::mat4x4 m_guiProjectionMatrix;

  std::unique_ptr<GLMaterial> m_guiMaterial;

  std::unique_ptr<GUIWidgetsLoader> m_widgetsLoader;

  std::vector<std::function<void()>> m_eventsQueue;
};

