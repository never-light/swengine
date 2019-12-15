#include "GUISystem.h"

#include <spdlog/spdlog.h>

GUISystem::GUISystem(std::shared_ptr<GameWorld> gameWorld, std::shared_ptr<InputModule> inputModule,
                     std::shared_ptr<GLGraphicsContext> graphicsContext,
                     std::shared_ptr<GLShadersPipeline> guiShadersPipeline)
    : m_gameWorld(gameWorld),
      m_inputModule(inputModule),
      m_graphicsContext(graphicsContext),
      m_guiShadersPipeline(guiShadersPipeline)
{
    m_guiNDCQuad = std::make_unique<Mesh>();

    m_guiNDCQuad->setVertices({
        { 0.0f,  1.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f },
        { 1.0f,  1.0f, 0.0f },
        { 1.0f, 0.0f, 0.0f },
    });

    m_guiNDCQuad->setUV({
        { 0.0f, 1.0f },
        { 0.0f, 0.0f },
        { 1.0f, 1.0f },
        { 1.0f, 0.0f },
    });

    m_guiNDCQuad->setNormals({
        { 0.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f },
    });

    m_guiNDCQuad->addSubMesh({ 0, 2, 1, 1, 2, 3 });
}

void GUISystem::configure(GameWorld* gameWorld)
{
    gameWorld->subscribeEventsListener<MouseButtonEvent>(this);
    gameWorld->subscribeEventsListener<KeyboardEvent>(this);

    m_guiProjectionMatrix = glm::ortho(0.0f, static_cast<float>(m_graphicsContext->getBufferWidth()),
                                       static_cast<float>(m_graphicsContext->getBufferHeight()), 0.0f, -1.0f, 1.0f);
}

void GUISystem::unconfigure(GameWorld* gameWorld)
{
    gameWorld->unsubscribeEventsListener<KeyboardEvent>(this);
    gameWorld->unsubscribeEventsListener<MouseButtonEvent>(this);
}

void GUISystem::update(GameWorld* gameWorld, float delta)
{
    ARG_UNUSED(gameWorld);
    ARG_UNUSED(delta);

    if (m_activeLayout != nullptr) {
        updateGUIWidget(m_activeLayout.get());
    }
}

void GUISystem::render(GameWorld* gameWorld)
{
    ARG_UNUSED(gameWorld);

    m_graphicsContext->disableDepthTest();
    m_graphicsContext->enableBlending();
    m_graphicsContext->setBlendingMode(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLShader* vertexShader = m_guiShadersPipeline->getShader(GL_VERTEX_SHADER);
    vertexShader->setParameter("scene.projection", m_guiProjectionMatrix);

    if (m_activeLayout != nullptr) {
        renderGUIWidget(m_activeLayout.get());
    }
}

void GUISystem::setActiveLayout(std::shared_ptr<GUILayout> layout)
{
    m_activeLayout = layout;
}

std::shared_ptr<GUILayout> GUISystem::getActiveLayout()
{
    return m_activeLayout;
}

void GUISystem::setDefaultFont(std::shared_ptr<BitmapFont> font)
{
    m_defaultFont = font;
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
    glm::vec4 backgroundColor = widget->getBackgroundColor();
    GLTexture* backgroundTexture = widget->getBackgroundImage().get();

    if (widget->isHovered()) {
        backgroundColor = widget->getHoverBackgroundColor();

        if (widget->getHoverBackgroundImage() != nullptr) {
            backgroundTexture = widget->getHoverBackgroundImage().get();
        }
    }

    if (widget->hasFocus()) {
        backgroundColor = widget->getFocusBackgroundColor();
    }

    fragmentShader->setParameter("widget.backgroundColor", backgroundColor);
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
    task.shadersPipeline = m_guiShadersPipeline.get();
    task.sharedGraphicsState = nullptr;

    return task;
}

int GUISystem::getScreenWidth() const
{
    return m_graphicsContext->getBufferWidth();
}

int GUISystem::getScreenHeight() const
{
    return m_graphicsContext->getBufferHeight();
}

EventProcessStatus GUISystem::receiveEvent(GameWorld* gameWorld, const MouseButtonEvent& event)
{
    ARG_UNUSED(gameWorld);

    if (m_activeLayout != nullptr) {
        processGUIWidgetMouseButtonEvent(m_activeLayout.get(), event);
    }

    return EventProcessStatus::Processed;
}

EventProcessStatus GUISystem::receiveEvent(GameWorld* gameWorld, const KeyboardEvent& event)
{
    ARG_UNUSED(gameWorld);

    if (m_focusedWidget != nullptr) {
        GUIKeyboardEvent guiEvent;
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
    if (widget->isShown()) {
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
    }

    for (auto childWidget : widget->getChildrenWidgets()) {
        updateGUIWidget(childWidget.get());
    }
}

void GUISystem::processGUIWidgetMouseButtonEvent(GUIWidget* widget, const MouseButtonEvent& event)
{
    bool isExclusive = true;

    for (auto childWidget : widget->getChildrenWidgets()) {
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
    else {
        if (widget->hasFocus()) {
            m_focusedWidget = nullptr;
            widget->resetFocus();
        }
    }
}

bool GUISystem::isMouseInWidgetArea(const GUIWidget* widget) const
{
    MousePosition mousePosition = m_inputModule->getMousePosition();

    return widget->m_origin.x <= mousePosition.x && mousePosition.x <= (widget->m_origin.x + widget->m_size.x) &&
            widget->m_origin.y <= mousePosition.y && mousePosition.y <= (widget->m_origin.y + widget->m_size.y);
}

void GUISystem::renderGUIWidget(GUIWidget* widget)
{
    if (!widget->isShown()) {
        return;
    }

    widget->render(*this);

    for (auto childWidget : widget->getChildrenWidgets()) {
        renderGUIWidget(childWidget.get());
    }
}
