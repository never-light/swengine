#include "precompiled.h"

#pragma hdrstop

#include "GUISystem.h"

#include <spdlog/spdlog.h>

#include <utility>

GUISystem::GUISystem(
  std::shared_ptr<InputModule> inputModule,
  std::shared_ptr<ResourceManager> resourceManager,
  std::shared_ptr<GLGraphicsContext> graphicsContext,
  std::shared_ptr<GLShadersPipeline> guiShadersPipeline)
  : m_inputModule(std::move(inputModule)),
    m_resourceManager(std::move(resourceManager)),
    m_graphicsContext(std::move(graphicsContext)),
    m_guiShadersPipeline(std::move(guiShadersPipeline))
{
  m_guiNDCQuad = std::make_unique<Mesh>();

  m_guiNDCQuad->setVertices({
    {0.0f, 1.0f, 1.0f},
    {0.0f, 0.0f, 1.0f},
    {1.0f, 1.0f, 1.0f},
    {1.0f, 0.0f, 1.0f},
  });

  m_guiNDCQuad->setUV({
    {0.0f, 1.0f},
    {0.0f, 0.0f},
    {1.0f, 1.0f},
    {1.0f, 0.0f},
  });

  m_guiNDCQuad->setNormals({
    {0.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
  });

  RETURN_VALUE_UNUSED(m_guiNDCQuad->addSubMesh({0, 2, 1, 1, 2, 3}));

  m_guiMaterial = std::make_unique<GLMaterial>();

  m_guiMaterial->setShadersPipeline(m_guiShadersPipeline);
  m_guiMaterial->setDepthTestMode(DepthTestMode::Disabled);
  m_guiMaterial->setBlendingMode(BlendingMode::Alpha_OneMinusAlpha);
  m_guiMaterial->setPolygonFillingMode(PolygonFillingMode::Fill);
}

void GUISystem::configure()
{
  GameWorld* gameWorld = getGameWorld();

  gameWorld->subscribeEventsListener<MouseButtonEvent>(this);
  gameWorld->subscribeEventsListener<KeyboardEvent>(this);

  m_guiProjectionMatrix = glm::ortho(0.0f, static_cast<float>(m_graphicsContext->getDefaultFramebuffer().getWidth()),
    static_cast<float>(m_graphicsContext->getDefaultFramebuffer().getHeight()),
    0.0f, -1.0f, 1.0f);

  m_widgetsLoader = std::make_unique<GUIWidgetsLoader>(weak_from_this(), m_resourceManager);
}

void GUISystem::unconfigure()
{
  m_widgetsLoader.reset();

  GameWorld* gameWorld = getGameWorld();

  gameWorld->unsubscribeEventsListener<KeyboardEvent>(this);
  gameWorld->unsubscribeEventsListener<MouseButtonEvent>(this);
}

void GUISystem::update(float delta)
{
  ARG_UNUSED(delta);

  if (m_activeLayout != nullptr) {
    updateGUIWidget(m_activeLayout.get());
  }
}

void GUISystem::render()
{
  GLShader* vertexShader = m_guiShadersPipeline->getShader(GL_VERTEX_SHADER);
  vertexShader->setParameter("scene.projection", m_guiProjectionMatrix);

  if (m_activeLayout != nullptr) {
    renderGUIWidget(m_activeLayout.get());
  }
}

void GUISystem::setActiveLayout(std::shared_ptr<GUILayout> layout)
{
  m_activeLayout = std::move(layout);
}

std::shared_ptr<GUILayout> GUISystem::getActiveLayout()
{
  return m_activeLayout;
}

void GUISystem::setDefaultFont(std::shared_ptr<BitmapFont> font)
{
  m_defaultFont = std::move(font);
}

std::shared_ptr<BitmapFont> GUISystem::getDefaultFont() const
{
  return m_defaultFont;
}

std::shared_ptr<GLGraphicsContext> GUISystem::getGraphicsContext() const
{
  return m_graphicsContext;
}

RenderTask GUISystem::getRenderTaskTemplate(GUIWidget* widget) const
{
  // Transformation
  GLShader* vertexShader = m_guiShadersPipeline->getShader(GL_VERTEX_SHADER);
  vertexShader->setParameter("transform.localToScreen", widget->getTransformationMatrix());

  // Visual parameters
  GLShader* fragmentShader = m_guiShadersPipeline->getShader(GL_FRAGMENT_SHADER);

  // Background
  GUIWidgetVisualState visualState = GUIWidgetVisualState::Default;

  if (widget->isHovered()) {
    visualState = GUIWidgetVisualState::Hover;
  }

  if (widget->hasFocus()) {
    visualState = GUIWidgetVisualState::Focus;
  }

  auto& currentVisualParameters = widget->getVisualParameters(visualState);
  auto& defaultVisualParameters = widget->getVisualParameters(GUIWidgetVisualState::Default);

  auto backgroundColor = currentVisualParameters.getBackgroundColor();

  if (!backgroundColor.has_value()) {
    backgroundColor = defaultVisualParameters.getBackgroundColor();
  }

  auto backgroundTexture = currentVisualParameters.getBackgroundImage();

  if (backgroundTexture == nullptr) {
    backgroundTexture = defaultVisualParameters.getBackgroundImage();
  }

  fragmentShader->setParameter("widget.backgroundColor", backgroundColor.value());
  fragmentShader->setParameter("widget.useBackgroundTexture", backgroundTexture != nullptr);

  if (backgroundTexture != nullptr) {
    fragmentShader->setParameter("widget.backgroundTexture", *backgroundTexture, 0);
  }

  fragmentShader->setParameter("widget.useColorAlphaTexture", false);

  // Border
  // TODO: implement border filling
  //fragmentShader->setParameter("widget.borderWidth", widget->getBorderWidth());

  //glm::vec4 borderColor = widget->getBorderColor();

  //if (widget->isHovered()) {
  //borderColor = widget->getHoverBorderColor();
  //}

  //fragmentShader->setParameter("widget.borderColor", borderColor);

  // Render task
  RenderTask task;
  task.geometryStore = m_guiNDCQuad->getGeometryStore();
  task.startOffset = m_guiNDCQuad->getSubMeshIndicesOffset(0);
  task.partsCount = m_guiNDCQuad->getSubMeshIndicesCount(0);
  task.material = m_guiMaterial.get();

  return task;
}

int GUISystem::getScreenWidth() const
{
  return m_graphicsContext->getDefaultFramebuffer().getWidth();
}

int GUISystem::getScreenHeight() const
{
  return m_graphicsContext->getDefaultFramebuffer().getHeight();
}

EventProcessStatus GUISystem::receiveEvent(GameWorld* gameWorld, const MouseButtonEvent& event)
{
  ARG_UNUSED(gameWorld);

  if (m_activeLayout != nullptr) {
    processGUIWidgetMouseButtonEvent(m_activeLayout.get(), event);

    if (m_focusedWidget != nullptr) {
      if (!isMouseInWidgetArea(m_focusedWidget.get())) {
        m_focusedWidget->resetFocus();
        m_focusedWidget = nullptr;
      }
    }
  }

  return EventProcessStatus::Processed;
}

EventProcessStatus GUISystem::receiveEvent(GameWorld* gameWorld, const KeyboardEvent& event)
{
  ARG_UNUSED(gameWorld);

  if (m_focusedWidget != nullptr && m_focusedWidget->isShown()) {
    GUIKeyboardEvent guiEvent{};
    guiEvent.type = event.type;
    guiEvent.keyCode = event.keyCode;
    guiEvent.repeated = event.repeated;
    guiEvent.keyModifiers = event.keyModifiers;

    m_focusedWidget->triggerKeyboardEvent(guiEvent);

    return EventProcessStatus::Prevented;
  }

  return EventProcessStatus::Processed;
}

void GUISystem::updateGUIWidget(GUIWidget* widget)
{
  if (!widget->isShown()) {
    return;
  }

  if (isMouseInWidgetArea(widget)) {
    if (!widget->m_isHovered) {
      widget->m_isHovered = true;

      GUIMouseEnterEvent event;
      widget->triggerMouseEnterEvent(event);
    }
  }
  else {
    if (widget->m_isHovered) {
      widget->m_isHovered = false;

      GUIMouseLeaveEvent event;
      widget->triggerMouseLeaveEvent(event);
    }
  }

  for (const auto& childWidget : widget->getChildrenWidgets()) {
    updateGUIWidget(childWidget.get());
  }
}

void GUISystem::processGUIWidgetMouseButtonEvent(GUIWidget* widget, const MouseButtonEvent& event)
{
  if (!widget->isShown()) {
    return;
  }

  bool isExclusive = true;

  for (const auto& childWidget : widget->getChildrenWidgets()) {
    if (isMouseInWidgetArea(widget)) {
      isExclusive = false;

      processGUIWidgetMouseButtonEvent(childWidget.get(), event);
    }
  }

  if (isMouseInWidgetArea(widget)) {
    if (widget->canHaveFocus()) {
      if (m_focusedWidget != nullptr) {
        m_focusedWidget->resetFocus();
      }

      widget->setFocus();

      m_focusedWidget = widget->shared_from_this();
    }

    GUIMouseButtonEvent mouseEvent;
    mouseEvent.type = event.type;
    mouseEvent.button = event.button;
    mouseEvent.isExclusive = isExclusive;

    widget->triggerMouseButtonEvent(mouseEvent);
  }
}

bool GUISystem::isMouseInWidgetArea(const GUIWidget* widget) const
{
  MousePosition mousePosition = m_inputModule->getMousePosition();
  return widget->isPointInside({mousePosition.x, mousePosition.y});
}

void GUISystem::renderGUIWidget(GUIWidget* widget)
{
  if (!widget->isShown()) {
    return;
  }

  widget->render(*this);

  for (const auto& childWidget : widget->getChildrenWidgets()) {
    renderGUIWidget(childWidget.get());
  }
}

std::shared_ptr<GUILayout> GUISystem::loadScheme(const std::string& schemePath)
{
  SW_ASSERT(m_widgetsLoader != nullptr);

  spdlog::debug("Load GUI scheme from file {}", schemePath);

  return m_widgetsLoader->loadScheme(schemePath);
}

GUIWidgetStylesheet GUISystem::loadStylesheet(const std::string& stylesheetPath)
{
  SW_ASSERT(m_widgetsLoader != nullptr);

  spdlog::debug("Load GUI stylesheet from file {}", stylesheetPath);

  return m_widgetsLoader->loadStylesheet(stylesheetPath);
}

GUIWidgetsLoader* GUISystem::getWidgetsLoader() const
{
  return m_widgetsLoader.get();
}
