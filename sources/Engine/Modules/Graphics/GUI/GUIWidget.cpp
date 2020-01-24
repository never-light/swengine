#include "precompiled.h"
#pragma hdrstop

#include "GUIWidget.h"
#include <algorithm>

#include "GUISystem.h"

void GUIWidget::setOrigin(const glm::ivec2& origin)
{
    m_origin = origin;
    resetTransformationCache();
}

glm::ivec2 GUIWidget::getOrigin() const
{
    return m_origin;
}

glm::ivec2 GUIWidget::getAbsoluteOrigin() const
{
    glm::ivec2 origin = m_origin;

    std::shared_ptr<GUIWidget> parent = m_parent.lock();

    if (parent != nullptr) {
        origin = origin + parent->getOrigin();
    }

    return origin;
}

void GUIWidget::setSize(const glm::ivec2& size)
{
    m_size = size;
    resetTransformationCache();
}

glm::ivec2 GUIWidget::getSize() const
{
    return m_size;
}

void GUIWidget::setWidth(int width)
{
    m_size.x = width;
}

void GUIWidget::setHeight(int height)
{
    m_size.y = height;
}

void GUIWidget::addChildWidget(std::shared_ptr<GUIWidget> widget)
{
    m_widgets.push_back(widget);
    widget->setParent(weak_from_this());

    orderChildrenByZIndex();
}

void GUIWidget::removeChildWidget(const std::shared_ptr<GUIWidget>& widget)
{
    widget->setParent({});
    m_widgets.erase(std::remove(m_widgets.begin(), m_widgets.end(), widget), m_widgets.end());
}

const std::vector<std::shared_ptr<GUIWidget>>& GUIWidget::getChildrenWidgets() const
{
    return m_widgets;
}

void GUIWidget::removeChildren()
{
    m_widgets.clear();
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

void GUIWidget::setFocusBackgroundColor(const glm::vec4& color)
{
    m_focusBackgroundColor = color;
}

glm::vec4 GUIWidget::getFocusBackgroundColor() const
{
    return m_focusBackgroundColor;
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

void GUIWidget::setFocusBorderColor(const glm::vec4& color)
{
    m_focusBorderColor = color;
}

glm::vec4 GUIWidget::getFocusBorderColor() const
{
    return m_focusBorderColor;
}

void GUIWidget::setZIndex(int zIndex)
{
    m_zIndex = zIndex;

    if (m_parent.lock() != nullptr) {
        m_parent.lock()->orderChildrenByZIndex();
    }
}

int GUIWidget::getZIndex() const
{
    return m_zIndex;
}

const glm::mat4x4& GUIWidget::getTransformationMatrix()
{
    if (m_needTransformationMatrixCacheUpdate) {
        m_transformationMatrixCache = glm::translate(glm::identity<glm::mat4x4>(),
                                                     glm::vec3(getAbsoluteOrigin(), 0.0f));

        if (m_isScaleTransformEnabled) {
            m_transformationMatrixCache = glm::scale(m_transformationMatrixCache, glm::vec3(m_size, 1.0f));
        }

        transformationCacheUpdate();

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

void GUIWidget::setKeyboardEventCallback(EventCallback<GUIKeyboardEvent> callback)
{
    m_keyboardEventCallback = callback;
}

void GUIWidget::enableScaleTransform()
{
    m_isScaleTransformEnabled = true;
    resetTransformationCache();
}

void GUIWidget::disableScaleTransform()
{
    m_isScaleTransformEnabled = false;
    resetTransformationCache();
}

void GUIWidget::resetTransformationCache()
{
    m_needTransformationMatrixCacheUpdate = true;

    for (auto childWidget : m_widgets) {
        childWidget->resetTransformationCache();
    }
}

void GUIWidget::transformationCacheUpdate()
{

}

void GUIWidget::processKeyboardEvent(const GUIKeyboardEvent& event)
{
    ARG_UNUSED(event);
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

void GUIWidget::triggerKeyboardEvent(const GUIKeyboardEvent& event)
{
    processKeyboardEvent(event);

    if (m_keyboardEventCallback) {
        m_keyboardEventCallback(event);
    }
}

void GUIWidget::setParent(std::weak_ptr<GUIWidget> parent)
{
    m_parent = parent;
}

void GUIWidget::setFocus()
{
    m_hasFocus = true;
}

void GUIWidget::resetFocus()
{
    m_hasFocus = false;
}

void GUIWidget::orderChildrenByZIndex()
{
    std::sort(m_widgets.begin(), m_widgets.end(), [] (std::shared_ptr<GUIWidget> widget1, std::shared_ptr<GUIWidget> widget2) {
        return widget1->getZIndex() < widget2->getZIndex();
    });
}

std::shared_ptr<GUIWidget> GUIWidget::getParent() const
{
    return m_parent.lock();
}
