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
        { -1.0f,  1.0f, 0.0f },
        { -1.0f, -1.0f, 0.0f },
        { 1.0f,  1.0f, 0.0f },
        { 1.0f, -1.0f, 0.0f },
    });

    m_guiNDCQuad->setUV({
        { 0.0f, 1.0f },
        { 0.0f, 0.0f },
        { 1.0f, 1.0f },
        { 1.0f, 0.0f },
    });

    m_guiNDCQuad->addSubMesh({ 0, 2, 1, 1, 2, 3 });
}

void GUISystem::configure(GameWorld* gameWorld)
{
    gameWorld->subscribeEventsListener<MouseButtonEvent>(this);

    m_guiProjectionMatrix = glm::ortho(0.0f, static_cast<float>(m_graphicsContext->getBufferWidth()),
                                       static_cast<float>(m_graphicsContext->getBufferHeight()), 0.0f, -1.0f, 1.0f);
}

void GUISystem::unconfigure(GameWorld* gameWorld)
{
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
    glm::vec3 backgroundColor = widget->getBackgroundColor();
    GLTexture* backgroundTexture = widget->getBackgroundImage().get();

    if (widget->isHovered()) {
        backgroundColor = widget->getHoverBackgroundColor();

        if (widget->getHoverBackgroundImage() != nullptr) {
            backgroundTexture = widget->getHoverBackgroundImage().get();
        }
    }

    fragmentShader->setParameter("widget.backgroundColor", backgroundColor);
    fragmentShader->setParameter("widget.useBackgroundTexture", backgroundTexture != nullptr);

    if (widget->getBackgroundImage() != nullptr) {
        fragmentShader->setParameter("widget.backgroundTexture", *backgroundTexture, 0);
    }

    // Border
    // TODO: implement border filling
    //fragmentShader->setParameter("widget.borderWidth", widget->getBorderWidth());

    //glm::vec3 borderColor = widget->getBorderColor();

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

void GUISystem::receiveEvent(GameWorld* gameWorld, const MouseButtonEvent& event)
{
    ARG_UNUSED(gameWorld);
    ARG_UNUSED(event);
}

void GUISystem::updateGUIWidget(GUIWidget* widget)
{
    if (widget->isShown()) {
        if (isMouseInWidgetArea(widget)) {
            if (!widget->m_isHovered) {
                widget->m_isHovered = true;

                GUIMouseEnterEvent event;
                event.component = widget;

                m_gameWorld->emitEvent<GUIMouseEnterEvent>(event);
            }
        }
        else {
            if (widget->m_isHovered) {
                widget->m_isHovered = false;

                GUIMouseLeaveEvent event;
                event.component = widget;

                m_gameWorld->emitEvent<GUIMouseLeaveEvent>(event);
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

            processGUIWidgetMouseButtonEvent(widget, event);
        }
    }

    if (isMouseInWidgetArea(widget)) {
        GUIMouseButtonEvent mouseEvent;
        mouseEvent.component = widget;
        mouseEvent.type = event.type;
        mouseEvent.button = event.button;
        mouseEvent.isExclusive = isExclusive;

        m_gameWorld->emitEvent<GUIMouseButtonEvent>(mouseEvent);
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
