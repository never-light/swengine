#include "precompiled.h"

#pragma hdrstop

#include "GUIWidgetStylesheet.h"
#include <algorithm>
#include <utility>

#include "GUISystem.h"

const GUIWidgetStylesheetProperty::Value& GUIWidgetStylesheetProperty::getValue() const
{
  return m_value;
}

void GUIWidgetStylesheetProperty::setValue(const GUIWidgetStylesheetProperty::Value& value)
{
  m_value = value;
}

GUIWidgetStylesheetProperty::GUIWidgetStylesheetProperty(GUIWidgetStylesheetProperty::Value value)

  : m_value(std::move(value))
{

}

GUIWidgetStylesheetRule::GUIWidgetStylesheetRule(
  GUIWidgetVisualState visualState,
  std::vector<GUIWidgetStylesheetSelectorPart> selector)
  : m_visualState(visualState),
  m_selector(std::move(selector))
{

}

GUIWidgetVisualState GUIWidgetStylesheetRule::getVisualState() const
{
  return m_visualState;
}

const std::vector<GUIWidgetStylesheetSelectorPart>& GUIWidgetStylesheetRule::getSelector() const
{
  return m_selector;
}

const GUIWidgetStylesheetSelectorPart& GUIWidgetStylesheetRule::getSelectorPart(size_t index) const
{
  return m_selector[index];
}

void GUIWidgetStylesheetRule::setProperty(const std::string& propertyName, const GUIWidgetStylesheetProperty& property)
{
  m_properties[propertyName] = property;
}

void GUIWidgetStylesheetRule::setPropertyValue(
  const std::string& propertyName,
  const GUIWidgetStylesheetProperty::Value& value)
{
  m_properties[propertyName].setValue(value);
}

const GUIWidgetStylesheetProperty::Value& GUIWidgetStylesheetRule::getPropertyValue(const std::string& propertyName) const
{
  return m_properties.at(propertyName).getValue();
}

void GUIWidgetStylesheetRule::visit(const PropertiesVisitor& visitor) const
{
  GUIWidgetVisualState visualState = getVisualState();

  for (const auto& [propertyName, property] : m_properties) {
    visitor(propertyName, property, visualState);
  }
}

bool GUIWidgetStylesheetRule::isSelectionCompleted(size_t selectorPartIndex) const
{
  return selectorPartIndex >= m_selector.size();
}

void GUIWidgetStylesheet::addRule(const GUIWidgetStylesheetRule& rule)
{
  m_rules.push_back(rule);
}

const std::vector<GUIWidgetStylesheetRule>& GUIWidgetStylesheet::getRules() const
{
  return m_rules;
}
