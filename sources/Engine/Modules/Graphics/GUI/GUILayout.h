#pragma once

#include <utility>

#include "GUIWidget.h"

class GUILayout : public GUIWidgetRect {
 public:
  GUILayout();
  explicit GUILayout(const std::string& className);

  void applyStylesheetRule(const GUIWidgetStylesheetRule& stylesheetRule, size_t selectorPartIndex) override;
  void applyStylesheetRuleToChildren(const GUIWidgetStylesheetRule& stylesheetRule,
    size_t selectorPartIndex) override;
};
