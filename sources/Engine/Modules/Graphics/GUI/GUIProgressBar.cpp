#include "precompiled.h"

#pragma hdrstop

#include "GUIProgressBar.h"
#include "GUISystem.h"

GUIProgressBar::GUIProgressBar()
  : GUIWidgetRect("progress_bar"),
    m_scaleBox(std::make_unique<GUIWidgetRect>("scale"))
{
}

void GUIProgressBar::applyStylesheetRule(const GUIWidgetStylesheetRule& stylesheetRule)
{
  GUIWidgetRect::applyStylesheetRule(stylesheetRule);

  stylesheetRule.visit([this](auto propertyName, auto property, GUIWidgetVisualState visualState) {
    if (propertyName == "border-padding") {
      SW_ASSERT(std::holds_alternative<glm::ivec2>(property.getValue()));
      SW_ASSERT(visualState == GUIWidgetVisualState::Default &&
        "Border padding is supported only for default state");
      this->setBorderPadding(std::get<glm::ivec2>(property.getValue()));
    }
    else if (propertyName == "max-value") {
      SW_ASSERT(std::holds_alternative<int>(property.getValue()));
      SW_ASSERT(visualState == GUIWidgetVisualState::Default &&
        "Max value is supported only for default state");
      this->setMaxValue(std::get<int>(property.getValue()));
    }
    else if (propertyName == "value") {
      SW_ASSERT(std::holds_alternative<int>(property.getValue()));
      SW_ASSERT(visualState == GUIWidgetVisualState::Default &&
        "Value is supported only for default state");
      this->setValue(std::get<int>(property.getValue()));
    }
    else if (propertyName == "background") {
      // Do nothing as property should be already processed by GUILayout
    }
    else {
      SW_ASSERT(false);
    }
  });

}

void GUIProgressBar::applyStylesheetRuleToChildren(
  const GUIWidgetStylesheetRule& stylesheetRule,
  const std::vector<GUIWidgetStylesheetSelectorPart>& currentPath)
{
  GUIWidget::applyStylesheetRuleToChildren(stylesheetRule, currentPath);

  m_scaleBox->applyStylesheetRuleWithSelector(stylesheetRule, currentPath);
}

void GUIProgressBar::setMaxValue(int value)
{
  m_maxValue = value;
  updateScaleSize();
}

int GUIProgressBar::getMaxValue() const
{
  return m_maxValue;
}

void GUIProgressBar::setValue(int value)
{
  m_currentValue = value;
  updateScaleSize();
}

int GUIProgressBar::getValue() const
{
  return m_currentValue;
}

void GUIProgressBar::setBorderPadding(const glm::ivec2& padding)
{
  m_borderPadding = padding;
  updateScaleSize();
}

const glm::ivec2& GUIProgressBar::getBorderPadding() const
{
  return m_borderPadding;
}

void GUIProgressBar::updateScaleSize()
{
 if (m_scaleBox->getParent() == nullptr) {
   addChildWidget(m_scaleBox);
 }

  m_scaleBox->setOrigin(m_borderPadding);

  float scaleWidthFactor = static_cast<float>(m_currentValue) / static_cast<float>(m_maxValue);
  int scaleWidth = static_cast<int>(static_cast<float>(getSize().x - m_borderPadding.x * 2) * scaleWidthFactor);
  int scaleHeight = getSize().y - m_borderPadding.y * 2;

  m_scaleBox->setSize({ scaleWidth, scaleHeight });
}
