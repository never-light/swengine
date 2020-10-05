#pragma once

#include <string>
#include <memory>

#include "GUIWidget.h"

class GUIProgressBar : public GUIWidgetRect {
 public:
  explicit GUIProgressBar();

  void setMaxValue(int value);
  [[nodiscard]] int getMaxValue() const;

  void setValue(int value);
  [[nodiscard]] int getValue() const;

  void setBorderPadding(const glm::ivec2& padding);
  [[nodiscard]] const glm::ivec2& getBorderPadding() const;

  void applyStylesheetRule(const GUIWidgetStylesheetRule& stylesheetRule) override;
  void applyStylesheetRuleToChildren(const GUIWidgetStylesheetRule& stylesheetRule,
    const std::vector<GUIWidgetStylesheetSelectorPart>& currentPath) override;

 private:
  void updateScaleSize();

 private:
  std::shared_ptr<GUIWidgetRect> m_scaleBox;

  glm::ivec2 m_borderPadding{};

  int m_maxValue = 0;
  int m_currentValue = 100;
};
