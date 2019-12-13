#include "GUIWidget.h"
#include <algorithm>

#include "GUISystem.h"

void GUIWidget::setOrigin(const glm::ivec2& origin)
{
    m_origin = origin;
    m_needTransformationMatrixCacheUpdate = true;
}

glm::ivec2 GUIWidget::getOrigin() const
{
    return m_origin;
}

void GUIWidget::setSize(const glm::ivec2& size)
{
    m_size = size;
    m_needTransformationMatrixCacheUpdate = true;
}

glm::ivec2 GUIWidget::getSize() const
{
    return m_size;
}

void GUIWidget::addChildWidget(std::shared_ptr<GUIWidget> widget)
{
    m_widgets.push_back(std::move(widget));
}

void GUIWidget::removeChildWidget(const std::shared_ptr<GUIWidget>& widget)
{
    m_widgets.erase(std::remove(m_widgets.begin(), m_widgets.end(), widget), m_widgets.end());
}

const std::vector<std::shared_ptr<GUIWidget>>& GUIWidget::getChildrenWidgets() const
{
    return m_widgets;
}

void GUIWidget::show()
{
    m_isShown = true;
}

void GUIWidget::hide()
{
    m_isShown = false;
}

bool GUIWidget::isShown() const
{
    return m_isShown;
}

bool GUIWidget::isHovered() const
{
    return m_isHovered;
}

bool GUIWidget::hasFocus() const
{
    return m_hasFocus;
}

bool GUIWidget::canHaveFocus() const
{
    return false;
}

void GUIWidget::update(float delta)
{
    ARG_UNUSED(delta);
}

void GUIWidget::render(GUISystem& guiSystem)
{
    RenderTask task = guiSystem.getRenderTaskTemplate(this);
    guiSystem.getGraphicsContext()->executeRenderTask(task);
}

void GUIWidget::setBackgroundColor(const glm::vec4& color)
{
    m_backgroundColor = color;
}

glm::vec4 GUIWidget::getBackgroundColor() const
{
    return m_backgroundColor;
}

void GUIWidget::setBackgroundImage(std::shared_ptr<GLTexture> image)
{
    m_backgroundImage = image;
}

std::shared_ptr<GLTexture> GUIWidget::getBackgroundImage() const
{
    return m_backgroundImage;
}

void GUIWidget::setHoverBackgroundColor(const glm::vec4& color)
{
    m_hoverBackgroundColor = color;
}

glm::vec4 GUIWidget::getHoverBackgroundColor() const
{
    return m_hoverBackgroundColor;
}

void GUIWidget::setHoverBackgroundImage(std::shared_ptr<GLTexture> image)
{
    m_hoverBackgroundImage = image;
}

std::shared_ptr<GLTexture> GUIWidget::getHoverBackgroundImage() const
{
    return m_hoverBackgroundImage;
}

void GUIWidget::setBorderWidth(int width)
{
    m_borderWidth = width;
}

int GUIWidget::getBorderWidth() const
{
    return m_borderWidth;
}

void GUIWidget::setBorderColor(const glm::vec4& color)
{
    m_borderColor = color;
}

glm::vec4 GUIWidget::getBorderColor() const
{
    return m_borderColor;
}

void GUIWidget::setHoverBorderColor(const glm::vec4& color)
{
    m_hoverBorderColor = color;
}

glm::vec4 GUIWidget::getHoverBorderColor() const
{
    return m_hoverBorderColor;
}

const glm::mat4x4& GUIWidget::getTransformationMatrix()
{
    if (m_needTransformationMatrixCacheUpdate) {
        m_transformationMatrixCache = glm::translate(glm::identity<glm::mat4x4>(),
                                                     glm::vec3(m_origin, 0.0f));

        if (m_isScaleTransformEnabled) {
            m_transformationMatrixCache = glm::scale(m_transformationMatrixCache, glm::vec3(m_size, 1.0f));
        }

        m_needTransformationMatrixCacheUpdate = false;
    }

    return m_transformationMatrixCache;
}

void GUIWidget::setMouseButtonCallback(EventCallback<GUIMouseButtonEvent> callback)
{
    m_mouseButtonCallback = callback;
}

void GUIWidget::setMouseEnterCallback(EventCallback<GUIMouseEnterEvent> callback)
{
    m_mouseEnterCallback = callback;
}

void GUIWidget::setMouseLeaveCallback(EventCallback<GUIMouseLeaveEvent> callback)
{
    m_mouseLeaveCallback = callback;
}

void GUIWidget::enableScaleTransform()
{
    m_isScaleTransformEnabled = true;
    m_needTransformationMatrixCacheUpdate = true;
}

void GUIWidget::disableScaleTransform()
{
    m_isScaleTransformEnabled = false;
    m_needTransformationMatrixCacheUpdate = true;
}

void GUIWidget::triggerMouseButtonEvent(const GUIMouseButtonEvent& event)
{
    if (m_mouseButtonCallback) {
        m_mouseButtonCallback(event);
    }
}

void GUIWidget::triggerMouseEnterEvent(const GUIMouseEnterEvent& event)
{
    if (m_mouseEnterCallback) {
        m_mouseEnterCallback(event);
    }
}

void GUIWidget::triggerMouseLeaveEvent(const GUIMouseLeaveEvent& event)
{
    if (m_mouseLeaveCallback) {
        m_mouseLeaveCallback(event);
    }
}
