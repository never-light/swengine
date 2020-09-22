#include "precompiled.h"

#pragma hdrstop

#include "GUIWidget.h"
#include <algorithm>
#include <utility>

#include "GUISystem.h"

GUIWidget::GUIWidget(std::string className)
  : m_className(std::move(className))
{
  auto& defaultVisualParameters = getVisualParameters(GUIWidgetVisualState::Default);
  defaultVisualParameters.setBackgroundImage(nullptr);
  defaultVisualParameters.setBackgroundColor(glm::vec4(0.0f));
  defaultVisualParameters.setBorderColor(glm::vec4(0.0f));
  defaultVisualParameters.setBorderWidth(0);
}

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
    origin = origin + parent->getAbsoluteOrigin();
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
  showChildren(this);
}

void GUIWidget::hide()
{
  hideChildren(this);
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
    m_transformationMatrixCache = updateTransformationMatrix();

    m_needTransformationMatrixCacheUpdate = false;
  }

  return m_transformationMatrixCache;
}

void GUIWidget::setMouseButtonCallback(EventCallback<GUIMouseButtonEvent> callback)
{
  m_mouseButtonCallback = std::move(callback);
}

void GUIWidget::setMouseEnterCallback(EventCallback<GUIMouseEnterEvent> callback)
{
  m_mouseEnterCallback = std::move(callback);
}

void GUIWidget::setMouseLeaveCallback(EventCallback<GUIMouseLeaveEvent> callback)
{
  m_mouseLeaveCallback = std::move(callback);
}

void GUIWidget::setKeyboardEventCallback(EventCallback<GUIKeyboardEvent> callback)
{
  m_keyboardEventCallback = std::move(callback);
}

void GUIWidget::resetTransformationCache()
{
  m_needTransformationMatrixCacheUpdate = true;

  for (const auto& childWidget : m_widgets) {
    childWidget->resetTransformationCache();
  }
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
  m_parent = std::move(parent);
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
  std::sort(m_widgets.begin(),
    m_widgets.end(),
    [](std::shared_ptr<GUIWidget> widget1, std::shared_ptr<GUIWidget> widget2) {
      return widget1->getZIndex() < widget2->getZIndex();
    });
}

std::shared_ptr<GUIWidget> GUIWidget::getParent() const
{
  return m_parent.lock();
}

RectI GUIWidget::getRect() const
{
  return RectI(getAbsoluteOrigin(), m_size);
}

bool GUIWidget::isPointInside(const glm::ivec2& point) const
{
  return getRect().isPointInRect(point);
}

void GUIWidget::hideChildren(GUIWidget* parent)
{
  parent->m_isShown = false;

  for (auto& childWidget : parent->getChildrenWidgets()) {
    hideChildren(childWidget.get());
  }

  parent->onHide();
}

void GUIWidget::showChildren(GUIWidget* parent)
{
  parent->m_isShown = true;

  for (auto& childWidget : parent->getChildrenWidgets()) {
    showChildren(childWidget.get());
  }

  parent->onShow();
}

glm::mat4 GUIWidget::updateTransformationMatrix()
{
  return glm::translate(glm::identity<glm::mat4x4>(),
    glm::vec3(getAbsoluteOrigin(), 0.0f)) *
    glm::scale(glm::identity<glm::mat4x4>(), glm::vec3(m_size, 1.0f));
}

const GUIWidgetVisualParameters& GUIWidget::getVisualParameters(GUIWidgetVisualState state) const
{
  return m_visualParameters[static_cast<size_t>(state)];
}

GUIWidgetVisualParameters& GUIWidget::getVisualParameters(GUIWidgetVisualState state)
{
  return m_visualParameters[static_cast<size_t>(state)];
}

void GUIWidget::setName(const std::string& name)
{
  m_name = name;
}

const std::string& GUIWidget::getName() const
{
  return m_name;
}

std::shared_ptr<GUIWidget> GUIWidget::findChildByName(const std::string& name) const
{
  for (auto& child : m_widgets) {
    if (child->getName() == name) {
      return child;
    }

    auto foundChild = child->findChildByName(name);

    if (foundChild != nullptr) {
      return foundChild;
    }
  }

  return nullptr;
}

void GUIWidget::applyStylesheetRuleWithSelector(
  const GUIWidgetStylesheetRule& stylesheetRule,
  std::vector<GUIWidgetStylesheetSelectorPart> currentPath)
{
  // Increase current path
  currentPath.emplace_back(m_className, m_name);

  // Check that the current widget should be selected
  if (isPathSatisfiesSelector(currentPath, stylesheetRule.getSelector())) {
    applyStylesheetRule(stylesheetRule);
  }

  // Apply the rule for children
  applyStylesheetRuleToChildren(stylesheetRule, currentPath);
}

void GUIWidget::applyStylesheetRule(const GUIWidgetStylesheetRule& stylesheetRule)
{
  ARG_UNUSED(stylesheetRule);
}

void GUIWidget::applyStylesheetRuleToChildren(
  const GUIWidgetStylesheetRule& stylesheetRule,
  const std::vector<GUIWidgetStylesheetSelectorPart>& currentPath)
{
  ARG_UNUSED(stylesheetRule);
  ARG_UNUSED(currentPath);
}

void GUIWidget::applyStylesheet(const GUIWidgetStylesheet& stylesheet)
{
  for (const GUIWidgetStylesheetRule& rule : stylesheet.getRules()) {
    applyStylesheetRuleWithSelector(rule, {});
  }
}

bool GUIWidget::isPathSatisfiesSelector(
  const std::vector<GUIWidgetStylesheetSelectorPart>& path,
  const std::vector<GUIWidgetStylesheetSelectorPart>& selector)
{
  size_t selectorPartIndex = 0;
  size_t pathPartIndex = 0;

  for (const auto& pathPart : path) {
    auto& selectorPart = selector[selectorPartIndex];

    bool isClassFilterPassed = selectorPart.getClassFilter() == pathPart.getClassFilter();
    bool isNameFilterPassed = selectorPart.getNameFilter().empty() ||
      selectorPart.getNameFilter() == pathPart.getNameFilter();

    if (isClassFilterPassed && isNameFilterPassed) {
      selectorPartIndex++;
      if (selectorPartIndex == selector.size()) {
        return pathPartIndex == (path.size() - 1);
      }
    }

    pathPartIndex++;
  }

  return false;
}

void GUIWidget::onShow()
{

}

void GUIWidget::onHide()
{

}
